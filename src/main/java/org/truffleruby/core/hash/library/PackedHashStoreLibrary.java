
/*
 * Copyright (c) 2021, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.core.hash.library;

import com.oracle.truffle.api.CompilerDirectives;
import com.oracle.truffle.api.CompilerDirectives.TruffleBoundary;
import com.oracle.truffle.api.TruffleSafepoint;
import com.oracle.truffle.api.dsl.Cached;
import com.oracle.truffle.api.dsl.Cached.Exclusive;
import com.oracle.truffle.api.dsl.Cached.Shared;
import com.oracle.truffle.api.dsl.GenerateUncached;
import com.oracle.truffle.api.dsl.ImportStatic;
import com.oracle.truffle.api.dsl.Specialization;
import com.oracle.truffle.api.frame.Frame;
import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.library.CachedLibrary;
import com.oracle.truffle.api.library.ExportLibrary;
import com.oracle.truffle.api.library.ExportMessage;
import com.oracle.truffle.api.nodes.ExplodeLoop;
import com.oracle.truffle.api.nodes.ExplodeLoop.LoopExplosionKind;
import com.oracle.truffle.api.profiles.BranchProfile;
import com.oracle.truffle.api.profiles.ConditionProfile;
import com.oracle.truffle.api.profiles.LoopConditionProfile;
import org.truffleruby.RubyContext;
import org.truffleruby.RubyLanguage;
import org.truffleruby.collections.PEBiFunction;
import org.truffleruby.core.array.ArrayHelpers;
import org.truffleruby.core.array.RubyArray;
import org.truffleruby.core.basicobject.BasicObjectNodes.ReferenceEqualNode;
import org.truffleruby.core.cast.BooleanCastNode;
import org.truffleruby.core.hash.CompareHashKeysNode;
import org.truffleruby.core.hash.Entry;
import org.truffleruby.core.hash.FreezeHashKeyIfNeededNode;
import org.truffleruby.core.hash.FreezeHashKeyIfNeededNodeGen;
import org.truffleruby.core.hash.HashGuards;
import org.truffleruby.core.hash.HashLiteralNode;
import org.truffleruby.core.hash.HashingNodes;
import org.truffleruby.core.hash.RubyHash;
import org.truffleruby.core.hash.library.HashStoreLibrary.EachEntryCallback;
import org.truffleruby.language.RubyBaseNode;
import org.truffleruby.language.RubyNode;
import org.truffleruby.language.dispatch.DispatchNode;
import org.truffleruby.language.objects.shared.PropagateSharingNode;
import org.truffleruby.language.objects.shared.SharedObjects;

@ExportLibrary(value = HashStoreLibrary.class, receiverType = Object[].class)
@GenerateUncached
public class PackedHashStoreLibrary {

    /** Maximum numbers of entries to be represented as a packed Hash */
    public static final int MAX_ENTRIES = 3;
    private static final int ELEMENTS_PER_ENTRY = 3;
    public static final int TOTAL_ELEMENTS = MAX_ENTRIES * ELEMENTS_PER_ENTRY;

    // region Utilities

    public static Object[] createStore() {
        return new Object[TOTAL_ELEMENTS];
    }

    private static Object[] copyStore(Object[] store) {
        final Object[] copied = createStore();
        System.arraycopy(store, 0, copied, 0, TOTAL_ELEMENTS);
        return copied;
    }

    private static int getHashed(Object[] store, int n) {
        return (int) store[n * ELEMENTS_PER_ENTRY];
    }

    private static Object getKey(Object[] store, int n) {
        return store[n * ELEMENTS_PER_ENTRY + 1];
    }

    private static Object getValue(Object[] store, int n) {
        return store[n * ELEMENTS_PER_ENTRY + 2];
    }

    private static void setHashed(Object[] store, int n, int hashed) {
        store[n * ELEMENTS_PER_ENTRY] = hashed;
    }

    private static void setKey(Object[] store, int n, Object key) {
        store[n * ELEMENTS_PER_ENTRY + 1] = key;
    }

    private static void setValue(Object[] store, int n, Object value) {
        store[n * ELEMENTS_PER_ENTRY + 2] = value;
    }

    public static void setHashedKeyValue(Object[] store, int n, int hashed, Object key, Object value) {
        setHashed(store, n, hashed);
        setKey(store, n, key);
        setValue(store, n, value);
    }

    private static void removeEntry(Object[] store, int n) {
        assert verifyIntegerHashes(store);

        final int index = n * ELEMENTS_PER_ENTRY;
        System.arraycopy(
                store,
                index + ELEMENTS_PER_ENTRY,
                store,
                index,
                TOTAL_ELEMENTS - ELEMENTS_PER_ENTRY - index);

        assert verifyIntegerHashes(store);
    }

    private static boolean verifyIntegerHashes(Object[] store) {
        for (int i = 0; i < TOTAL_ELEMENTS; i += ELEMENTS_PER_ENTRY) {
            assert store[i] == null || store[i] instanceof Integer;
        }
        return true;
    }

    @TruffleBoundary
    private static void promoteToBuckets(RubyHash hash, Object[] store, int size) {
        final Entry[] buckets = new Entry[BucketsHashStore.growthCapacityGreaterThan(size)];

        Entry firstInSequence = null;
        Entry previousInSequence = null;
        Entry lastInSequence = null;

        for (int n = 0; n < size; n++) {
            final int hashed = getHashed(store, n);
            final Entry entry = new Entry(hashed, getKey(store, n), getValue(store, n));

            if (previousInSequence == null) {
                firstInSequence = entry;
            } else {
                previousInSequence.setNextInSequence(entry);
                entry.setPreviousInSequence(previousInSequence);
            }

            previousInSequence = entry;
            lastInSequence = entry;

            final int bucketIndex = BucketsHashStore.getBucketIndex(hashed, buckets.length);
            BucketsHashStore.appendToLookupChain(buckets, entry, bucketIndex);
        }

        hash.store = new BucketsHashStore(buckets, firstInSequence, lastInSequence);
        hash.size = size;
    }

    // endregion
    // region Messages

    @ExportMessage
    protected static Object lookupOrDefault(
            Object[] store, Frame frame, RubyHash hash, Object key, PEBiFunction defaultNode,
            @Cached LookupPackedEntryNode lookupPackedEntryNode,
            @Cached @Shared HashingNodes.ToHash hashNode) {

        int hashed = hashNode.execute(key, hash.compareByIdentity);
        return lookupPackedEntryNode.execute(frame, hash, key, hashed, defaultNode);
    }

    @ImportStatic(HashGuards.class)
    @ExportMessage
    protected static class Set {
        @Specialization(guards = "hash.size == 0")
        protected static boolean setFirst(Object[] store, RubyHash hash, Object key, Object value, boolean byIdentity,
                @Cached @Shared FreezeHashKeyIfNeededNode freezeHashKeyIfNeeded,
                @Cached @Shared HashingNodes.ToHash hashNode,
                @Cached @Shared PropagateSharingNode propagateSharingKey,
                @Cached @Shared PropagateSharingNode propagateSharingValue) {

            final Object key2 = freezeHashKeyIfNeeded.executeFreezeIfNeeded(key, byIdentity);
            propagateSharingKey.executePropagate(hash, key2);
            propagateSharingValue.executePropagate(hash, value);
            setHashedKeyValue(store, 0, hashNode.execute(key2, byIdentity), key2, value);
            hash.size = 1;
            assert verify(store, hash);
            return true;
        }

        @Specialization(guards = "hash.size > 0")
        protected static boolean set(Object[] store, RubyHash hash, Object key, Object value, boolean byIdentity,
                @Cached @Shared FreezeHashKeyIfNeededNode freezeHashKeyIfNeeded,
                @Cached @Shared HashingNodes.ToHash hashNode,
                @Cached @Shared PropagateSharingNode propagateSharingKey,
                @Cached @Shared PropagateSharingNode propagateSharingValue,
                @Cached @Shared CompareHashKeysNode compareHashKeys,
                @CachedLibrary(limit = "hashStrategyLimit()") HashStoreLibrary hashes,
                @Cached ConditionProfile withinCapacity) {

            assert verify(store, hash);
            final int size = hash.size;
            final Object key2 = freezeHashKeyIfNeeded.executeFreezeIfNeeded(key, byIdentity);
            final int hashed = hashNode.execute(key2, byIdentity);
            propagateSharingKey.executePropagate(hash, key2);
            propagateSharingValue.executePropagate(hash, value);

            // written very carefully to allow PE
            for (int n = 0; n < MAX_ENTRIES; n++) {
                if (n < size) {
                    final int otherHashed = getHashed(store, n);
                    final Object otherKey = getKey(store, n);
                    if (compareHashKeys.execute(byIdentity, key2, hashed, otherKey, otherHashed)) {
                        setValue(store, n, value);
                        return false;
                    }
                }
            }

            if (withinCapacity.profile(size < MAX_ENTRIES)) {
                setHashedKeyValue(store, size, hashed, key2, value);
                hash.size += 1;
                return true;
            }

            promoteToBuckets(hash, store, size);
            hashes.set(hash.store, hash, key2, value, byIdentity);
            return true;
        }
    }

    @ExportMessage
    protected static Object delete(Object[] store, RubyHash hash, Object key,
            @Cached @Shared HashingNodes.ToHash hashNode,
            @Cached @Shared CompareHashKeysNode compareHashKeys) {

        assert verify(store, hash);
        final int hashed = hashNode.execute(key, hash.compareByIdentity);
        final int size = hash.size;
        // written very carefully to allow PE
        for (int n = 0; n < MAX_ENTRIES; n++) {
            if (n < size) {
                final int otherHashed = getHashed(store, n);
                final Object otherKey = getKey(store, n);

                if (compareHashKeys.execute(hash.compareByIdentity, key, hashed, otherKey, otherHashed)) {
                    final Object value = getValue(store, n);
                    removeEntry(store, n);
                    hash.size -= 1;
                    return value;
                }
            }
        }
        assert verify(store, hash);
        return null;
    }

    @ExportMessage
    protected static Object deleteLast(Object[] store, RubyHash hash, Object key) {

        assert verify(store, hash);
        final int n = hash.size - 1;
        final Object lastKey = getKey(store, n);
        if (key != lastKey) {
            CompilerDirectives.transferToInterpreterAndInvalidate();
            throw CompilerDirectives
                    .shouldNotReachHere("The last key was not " + key + " as expected but was " + lastKey);
        }
        final Object value = getValue(store, n);
        removeEntry(store, n);
        hash.size -= 1;
        assert verify(store, hash);
        return value;
    }

    @ExportMessage
    @ImportStatic(HashGuards.class)
    static class EachEntry {

        @Specialization(guards = "hash.size == cachedSize", limit = "packedHashLimit()")
        @ExplodeLoop
        protected static Object eachEntry(Object[] store, RubyHash hash, EachEntryCallback callback, Object state,
                @CachedLibrary("store") HashStoreLibrary hashStoreLibrary,
                @Cached(value = "hash.size", allowUncached = true) int cachedSize,
                @Cached LoopConditionProfile loopProfile) {

            // Don't verify hash here, as `store != hash.store` when calling from `eachEntrySafe`.
            int i = 0;
            try {
                for (; loopProfile.inject(i < cachedSize); i++) {
                    callback.accept(i, getKey(store, i), getValue(store, i), state);
                    TruffleSafepoint.poll(hashStoreLibrary);
                }
            } finally {
                RubyBaseNode.profileAndReportLoopCount(hashStoreLibrary.getNode(), loopProfile, i);
            }
            return state;
        }
    }

    @ExportMessage
    protected static Object eachEntrySafe(Object[] store, RubyHash hash, EachEntryCallback callback, Object state,
            @CachedLibrary("store") HashStoreLibrary self) {

        return self.eachEntry(copyStore(store), hash, callback, state);
    }

    @ExportMessage
    protected static void replace(Object[] store, RubyHash hash, RubyHash dest,
            @Cached @Exclusive PropagateSharingNode propagateSharing) {