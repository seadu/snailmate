/*
 * Copyright (c) 2017, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.processor;

import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.StringJoiner;

import javax.annotation.processing.RoundEnvironment;
import javax.annotation.processing.SupportedAnnotationTypes;
import javax.lang.model.element.Element;
import javax.lang.model.element.ExecutableElement;
import javax.lang.model.element.PackageElement;
import javax.lang.model.element.TypeElement;
import javax.lang.model.element.VariableElement;
import javax.lang.model.type.TypeMirror;
import javax.lang.model.util.Elements;
import javax.tools.FileObject;
import javax.tools.JavaFileObject;
import javax.tools.StandardLocation;

import org.truffleruby.annotations.CoreMethod;
import org.truffleruby.annotations.CoreModule;
import org.truffleruby.annotations.Primitive;

import com.oracle.truffle.api.dsl.Specialization;

@SupportedAnnotationTypes("org.truffleruby.annotations.CoreModule")
public class CoreModuleProcessor extends TruffleRubyProcessor {

    private static final String SUFFIX = "Builtins";
    private static final Set<String> KEYWORDS;
    static {
        KEYWORDS = new HashSet<>();
        KEYWORDS.addAll(Arrays.asList(
                "alias",
                "and",
                "begin",
                "break",
                "case",
                "class",
                "def",
                "defined?",
                "do",
                "else",
                "elsif",
                "end",
                "ensure",
                "false",
                "for",
                "if",
                "in",
                "module",
                "next",
                "nil",
                "not",
                "or",
                "redo",
                "rescue",
                "retry",
                "return",
                "self",
                "super",
                "then",
                "true",
                "undef",
                "unless",
                "until",
                "when",
                "while",
                "yield"));
    }

    private final Set<String> processed = new HashSet<>();

    TypeMirror virtualFrameType;
    TypeMirror objectType;
    TypeMirror nilType;
    TypeMirror notProvidedType;
    TypeMirror rubyProcType;
    TypeMirror rootCallTargetType;
    // node types
    TypeMirror rubyNodeType;
    TypeMirror rubyBaseNodeType;
    TypeMirror primitiveNodeType;
    TypeMirror coreMethodNodeType;
    TypeMirror alwaysInlinedMethodNodeType;
    TypeMirror rubySourceNodeType;

    @Override
    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnvironment) {
        Elements elementUtils = processingEnv.getElementUtils();
        virtualFrameType = elementUtils.getTypeElement("com.oracle.truffle.api.frame.VirtualFrame").asType();
        objectType = elementUtils.getTypeElement("java.lang.Object").asType();
        nilType = elementUtils.getTypeElement("org.truffleruby.language.Nil").asType();
        notProvidedType = elementUtils.getTypeElement("org.truffleruby.language.NotProvided").asType();
        rubyProcType = elementUtils.getTypeElement("org.truffleruby.core.proc.RubyProc").asType();
        rootCallTargetType = elementUtils.getTypeElement("com.oracle.truffle.api.RootCallTarget").asType();
        rubyNodeType = elementUtils.getTypeElement("org.truffleruby.language.RubyNode").asType();
        rubyBaseNodeType = elementUtils.getTypeElement("org.truffleruby.language.RubyBaseNode").asType();
        primitiveNodeType = elementUtils.getTypeElement("org.truffleruby.builtins.PrimitiveNode").asType();
        coreMethodNodeType = elementUtils.getTypeElement("org.truffleruby.builtins.CoreMethodNode").asType();
        alwaysInlinedMethodNodeType = elementUtils
                .getTypeElement("org.truffleruby.core.inlined.AlwaysInlinedMethodNode").asType();
        rubySourceNodeType = elementUtils.getTypeElement("org.truffleruby.language.RubySourceNode").asType();


        if (!annotations.isEmpty()) {
            for (Element element : roundEnvironment.getElementsAnnotatedWith(CoreModule.class)) {
                try {
                    processCoreModule((TypeElement) element);
                } catch (IOException e) {
                    error(e.getClass() + " " + e.getMessage(), element);
                }
            }
        }

        return true;
    }

    private void processCoreModule(TypeElement coreModuleElement) throws IOException {
        final CoreModule coreModule = coreModuleElement.getAnnotation(CoreModule.class);

        final PackageElement packageElement = (PackageElement) coreModuleElement.getEnclosingElement();
        final String packageName = packageElement.getQualifiedName().toString();

        final String qualifiedName = coreModuleElement.getQualifiedName().toString();
        if (!processed.add(qualifiedName)) {
            // Already processed, do nothing. This seems an Eclipse bug.
            return;
        }

        final JavaFileObject output = processingEnv
                .getFiler()
                .createSourceFile(qualifiedName + SUFFIX, coreModuleElement);
        final FileObject rubyFile = processingEnv.getFiler().createResource(
                StandardLocation.SOURCE_OUTPUT,
                "core_module_stubs",
                coreModule.value().replace("::", "/") + ".rb",
                (Element[]) null);

        final CoreModuleChecks coreModuleChecks = new CoreModuleChecks(this);

        try (PrintStream stream = new PrintStream(output.openOutputStream(), true, "UTF-8")) {
            try (PrintStream rubyStream = new PrintStream(rubyFile.openOutputStream(), true, "UTF-8")) {

                final List<? extends Element> enclosedElements = coreModuleElement.getEnclosedElements();
                final boolean anyCoreMethod = anyCoreMethod(enclosedElements);

                stream.println("package " + packageName + ";");
                stream.println();
                stream.println("import org.truffleruby.builtins.CoreMethodNodeManager;");
                stream.println("import org.truffleruby.builtins.PrimitiveManager;");
                if (anyCoreMethod) {
                    stream.println("import org.truffleruby.annotations.Visibility;");
                    stream.println("import org.truffleruby.annotations.Split;");
                }
                stream.println();
                stream.println("// GENERATED BY " + getClass().getName());
                stream.println("public class " + coreModuleElement.getSimpleName() + SUFFIX + " {");
                stream.println();
                stream.println(
                        "    public static void setup(CoreMethodNodeManager coreMethodManager) {");

                rubyStream.println("raise 'this file is a stub file for development and should never be loaded'");
                rubyStream.println();
                rubyStream.println((coreModule.isClass() ? "class" : "module") + " " + coreModule.value());
                rubyStream.println();

                final StringBuilder rubyPrimitives = new StringBuilder();

                for (Element e : enclosedElements) {
                    if (e instanceof TypeElement) {
                        final TypeElement klass = (TypeElement) e;

                        final CoreMethod coreMethod = klass.getAnnotation(CoreMethod.class);
                        if (coreMethod != null) {
                            // Do not use needsSelf=true in module functions, it is either the module/class or the instance.
                            // Usage of needsSelf is quite rare for singleton methods (except constructors).
                            boolean needsSelf = coreMethod.constructor() ||
                                    (!coreMethod.isModuleFunction() && !coreMethod.onSingleton() &&
                                            coreMethod.needsSelf());

                            CoreMethod checkAmbiguous = !coreMethod.alwaysInlined() &&
                                    (coreMethod.optional() > 0 || coreMethod.needsBlock())
                                            ? coreMethod
                                            : null;
                            coreModuleChecks.checks(coreMethod.lowerFixnum(), checkAmbiguous, klass, needsSelf);
                            if (!inherits(e.asType(), coreMethodNodeType) &&
                                    !inherits(e.asType(), alwaysInlinedMethodNodeType) &&
                                    !inherits(e.asType(), rubySourceNodeType)) {
                                error(e +
                                        " should inherit from CoreMethodArrayArgumentsNode, CoreMethodNode, AlwaysInlinedMethodNode or RubySourceNode",
                                        e);
                            }
                            processCoreMethod(stream, rubyStream, coreModuleElement, coreModule, klass, coreMethod,
                                    needsSelf);
                        }
                    }
                }

                stream.println("    }");
                stream.println();

                stream.println(
                        "    public static void setupPrimitives(PrimitiveManager primitiveManager) {");

                for (Element e : enclosedElements) {
                    if (e instanceof TypeElement) {
                        final TypeElement klass = (TypeElement) e;
                        final Primitive primitive = e.getAnnotation(Primitive.cla