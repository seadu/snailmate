/*
 * Copyright (c) 2014, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.core.array;

import com.oracle.truffle.api.dsl.NeverDefault;
import org.truffleruby.core.array.library.ArrayStoreLibrary;
import org.truffleruby.core.array.library.ArrayStoreLibrary.ArrayAllocator;
import org.truffleruby.core.array.ArrayBuilderNodeFactory.AppendArrayNodeGen;
import org.truffleruby.core.array.ArrayBuilderNodeFactory.AppendOneNodeGen;
import org.truffleruby.language.RubyBaseNode;

import com.oracle.truffle.api.CompilerDirectives;
import com.oracle.truffle.api.dsl.Bind;
import com.oracle.truffle.api.dsl.ImportStatic;
import com.oracle.truffle.api.dsl.Specialization;
import com.oracle.truffle.api.library.CachedLibrary;

/** Builds a new Array and learns its storage strategy and its expected length. The storage strategy is generalized as
 * needed and the expected length is increased until all elements fit.
 * <p>
 * Append nodes handle only one strategy, but must still return a valid storage when:
 *
 * <li>The element(s) added do not match the strategy.
 * <li>The being-built storage no longer matches the strategy, due to the node having been replaced by another thread or
 * by another usage (e.g. recursive) of this ArrayBuilderNode. */
public abstract class ArrayBuilderNode extends RubyBaseNode {

    public static class BuilderState {
        protected int capacity;
        protected int nextIndex = 0;
        protected Object store;

        private BuilderState(Object store, int capacity) {
            this.capacity = capacity;
            this.store = store;
        }
    }

    @NeverDefault
    public static ArrayBuilderNode create() {
        return new ArrayBuilderProxyNode();
    }

    public abstract BuilderState start();

    public abstract BuilderState start(int length);

    public abstract void appendArray(BuilderState state, int index, RubyArray array);

    public abstract void appendValue(BuilderState state, int index, Object value);

    public abstract Object finish(BuilderState state, int length);

    private static class ArrayBuilderProxyNode extends ArrayBuilderNode {

        @Child StartNode startNode = new StartNode(ArrayStoreLibrary.initialAllocator(false), 0);
        @Child AppendArrayNode appendArrayNode;
        @Child AppendOneNode appendOneNode;

        @Override
        public BuilderState start() {
            return startNode.start();
        }

        @Override
        public BuilderState start(int length) {
            return startNode.start(length);
        }

        @Override
        public void appendArray(BuilderState state, int index, RubyArray array) {
            getAppendArrayNode().executeAppend(state, index, array);
        }

        @Override
        public void appendValue(BuilderState state, int index, Object value) {
            getAppendOneNode().executeAppend(state, index, value);
        }

        @Override
        public Object finish(BuilderState state, int length) {
            assert length == state.nextIndex;
            return state.store;
        }

        private AppendArrayNode getAppendArrayNode() {
            if (appendArrayNode == null) {
                CompilerDirectives.transferToInterpreterAndInvalidate();
                appendArrayNode = insert(AppendArrayNode.create());
            }
            return appendArrayNode;
        }

        private AppendOneNode getAppendOneNode() {
            if (appendOneNode == null) {
                CompilerDirectives.transferToInterpreterAndInvalidate();
                appendOneNode = insert(AppendOneNode.create());
            }
            return appendOneNode;
        }

        public synchronized ArrayAllocator updateStrategy(ArrayStoreLibrary.ArrayAllocator newStrategy, int newLength) {
            final ArrayStoreLibrary.ArrayAllocator oldStrategy = startNode.allocator;
            final ArrayStoreLibrary.ArrayAllocator updatedAllocator;
            // If two threads have raced to update the strategy then
            // oldStrategy may have been updated while waiting for to
            // claim the lock. We handle this by calculating the new
            // strategy explicitly here and returning it from this
            // function.
            if (oldStrategy != newStrategy) {
                updatedAllocator = ArrayStoreLibrary.getUncached().generalizeForStore(
                        oldStrategy.allocate(0),
                        newStrategy.allocate(0));
            } else {
                updatedAllocator = oldStrategy;
            }

            final int oldLength = startNode.expectedLength;
            final int newExpectedLength = Math.max(oldLength, newLength);

            if (updatedAllocator != oldStrategy || newExpectedLength > oldLength) {
                startNode.replace(new StartNode(updatedAllocator, newExpectedLength));
            }

            if (newStrategy != oldStrategy) {
                if (appendArrayNode != null) {
                