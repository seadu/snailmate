/*
 * Copyright (c) 2021, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.language.arguments;

import org.truffleruby.collections.WeakValueCache;
import org.truffleruby.core.string.StringUtils;

import java.util.Arrays;

public class KeywordArgumentsDescriptorManager {

    public static final KeywordArgumentsDescriptor EMPTY = new KeywordArgumentsDescriptor(
            StringUtils.EMPTY_STRING_ARRAY);

    private final WeakValueCache<Key, KeywordArgumentsDescriptor> CANONICAL_KEYWORD_DESCRIPTORS = new WeakValueCache<>();

    public