/*
 * Copyright (c) 2015, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.extra;

import com.oracle.truffle.api.CompilerDirectives.TruffleBoundary;
import com.oracle.truffle.api.object.Shape;
import org.truffleruby.core.basicobject.BasicObjectNodes.ReferenceEqualNode;
import org.truffleruby.core.kernel.KernelNodes.SameOrEqlNode;
import org.truffleruby.core.klass.RubyClass;
import org.truffleruby.language.RubyDynamicObject;

import java.util.concurrent.ConcurrentHashMap;

/** In Concurrent::Map, and so TruffleRuby::ConcurrentMap, keys are compared with #hash and #eql?, and values by
 * identity (#equal? in NonConcurrentMapBackend). To use custom code to compare the keys we need a wrapper for keys
 * implementing #hashCode and #equals. For comparing values by identity we use {@link ReferenceEqualNode} if the value
 * is a primitive, otherwise we rely on equals() being == on Ruby objects. */
public class RubyConcurrentMap extends RubyDynamicObject {

    public static class Key {

        public final Object key;
        private final int hashCode;

        public Key(Object key, int hashCode) {
           