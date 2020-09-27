/*
 * Copyright (c) 2018, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.tck;

import static org.graalvm.polyglot.tck.TypeDescriptor.ANY;
import static org.graalvm.polyglot.tck.TypeDescriptor.ARRAY;
import static org.graalvm.polyglot.tck.TypeDescriptor.BOOLEAN;
import static org.graalvm.polyglot.tck.TypeDescriptor.DATE;
import static org.graalvm.polyglot.tck.TypeDescriptor.HASH;
import static org.graalvm.polyglot.tck.TypeDescriptor.ITERABLE;
import static org.graalvm.polyglot.tck.TypeDescriptor.NULL;
import static org.graalvm.polyglot.tck.TypeDescriptor.NUMBER;
import static org.graalvm.polyglot.tck.TypeDescriptor.OBJECT;
import static org.graalvm.polyglot.tck.TypeDescriptor.STRING;
import static org.graalvm.polyglot.tck.TypeDescriptor.TIME;
import static org.graalvm.polyglot.tck.TypeDescriptor.TIME_ZONE;
import static org.graalvm.polyglot.tck.TypeDescriptor.array;
import static org.graalvm.polyglot.tck.TypeDescriptor.executable;
import static org.graalvm.polyglot.tck.TypeDescriptor.intersection;
import static org.graalvm.polyglot.tck.TypeDescriptor.union;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

import org.graalvm.polyglot.Context;
import org.graalvm.polyglot.PolyglotException;
import org.graalvm.polyglot.Source;
import org.graalvm.polyglot.Value;
import org.graalvm.polyglot.tck.InlineSnippet;
import org.graalvm.polyglot.tck.LanguageProvider;
import org.graalvm.polyglot.tck.ResultVerifier;
import org.graalvm.polyglot.tck.Snippet;
import org.graalvm.polyglot.tck.TypeDescriptor;
import org.junit.Assert;

public class RubyTCKLanguageProvider implements LanguageProvider {

    @Override
    public String getId() {
        return "ruby";
    }

    @Override
    public Collection<? extends Source> createInvalidSyntaxScripts(Context context) {
        final List<Source> res = new ArrayList<>();
        res.add(Source.create(getId(), "{"));
        res.add(Source.create(getId(), "("));
        res.add(Source.create(getId(), "if no_end"));
        return Collections.unmodifiableList(res);
    }

    @Override
    public Value createIdentityFunction(Context context) {
        return context.eval(getId(), "-> v { v }");
    }

    @Override
    public Collection<? extends Snippet> createValueConstructors(Context context) {
        final List<Snippet> vals = new ArrayList<>();
        // Interop Primitives
        vals.add(createValueConstructor(context, "nil", NULL)); // should also be OBJECT?
        vals.add(createValueConstructor(context, "false", BOOLEAN));
        // NOTE: NUMBER is only for primitives and types which are instanceof java.lang.Number.
        vals.add(createValueConstructor(context, "7", NUMBER)); // int
        vals.add(createValueConstructor(context, "1 << 42", NUMBER)); // long
        vals.add(createValueConstructor(context, "1 << 84", NUMBER)); // Bignum
        vals.add(createValueConstructor(context, "3.14", NUMBER));
        vals.add(createValueConstructor(context, "'test'", STRING));
        vals.add(createValueConstructor(context, "'0123456789' + '0123456789'", STRING));

        // Everything but interop primitives have members in Ruby, so they are also OBJECT
        vals.add(createValueConstructor(context, "Rational(1, 3)", OBJECT));
        vals.add(createValueConstructor(co