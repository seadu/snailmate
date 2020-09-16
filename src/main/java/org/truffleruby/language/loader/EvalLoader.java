/*
 * Copyright (c) 2018, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.language.loader;

import com.oracle.truffle.api.nodes.Node;
import com.oracle.truffle.api.strings.AbstractTruffleString;
import org.truffleruby.RubyContext;
import org.truffleruby.core.encoding.EncodingManager;
import org.truffleruby.core.encoding.Encodings;
import org.truffleruby.core.encoding.RubyEncoding;
import org.truffleruby.core.string.CannotConvertBinaryRubyStringToJavaString;
import org.truffleruby.core.string.TStringWithEncoding;
import org.truffleruby.language.control.RaiseException;
import org.truffleruby.parser.RubySource;
import org.truffleruby.parser.lexer.RubyLexer;
import org.truffleruby.shared.TruffleRuby;

import com.oracle.truffle.api.CompilerDirectives.TruffleBoundary;
import com.oracle.truffle.api.source.Source;

public abstract class EvalLoader {

    @TruffleBoundary
    public static RubySource createEvalSource(RubyContext context, AbstractTruffleString codeTString,
            RubyEncoding encoding, String method, String file, int line, Node currentNode) {
        var code = new TStringWithEncoding(codeTString.asTruffleStringUncached(encoding.tencoding), encoding);

        var sourceTString = createEvalTString(code);
        var sourceEncoding = sourceTString.encoding;

        if (!sourceEncoding.isAsciiCompatible) {
            throw new RaiseException(context, context.getCoreExceptions()
            