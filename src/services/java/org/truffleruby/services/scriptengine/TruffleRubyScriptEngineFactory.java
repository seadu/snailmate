/*
 * Copyright (c) 2018, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.services.scriptengine;

import java.util.Arrays;
import java.util.List;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineFactory;

import org.graalvm.polyglot.Context;

public class TruffleRubyScriptEngineFactory implements ScriptEngineFactory {

    @Override
    public String getEngineName() {
        return "TruffleRuby";

    }

    @Override
    public String getEngineVersion() {
        return query("RUBY_ENGINE_VERSION");
    }

    @Override
    public List<String> getExtensions() {
        return Arrays.asList(".rb");
    }

    @Override
    public List<String> getMimeTypes() {
        return Arrays.asList("application/x-ruby");
    }

    @Override
    public List<String> getNames() {
        return Arrays.asList("ruby", "truffleruby", "TruffleRuby");
    }

    @Override
    public String getLanguageName() {
        return "ruby";
    }

    @Override
    public String getLanguageVersion() {
        return query