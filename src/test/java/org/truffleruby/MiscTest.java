/*
 * Copyright (c) 2017, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import org.graalvm.polyglot.Context;
import org.graalvm.polyglot.PolyglotException;
import org.graalvm.polyglot.Value;
import org.junit.Assert;
import org.junit.Test;

public class MiscTest {

    @Test
    public void testMembersAndStringUnboxing() {
        try (Context context = RubyTest.createContext()) {
            Value result = context.eval("ruby", "Struct.new(:id, :text, :arr).new(42, '42', [1, 42, 3])");
            assertTrue(result.hasMembers());

            int id = result.invokeMember("id").asInt();
            assertEquals(42, id);

            String text = result.invokeMember("text").asString();
            assertEquals("42", text);

            Value array = result.invokeMember("arr");
            assertTrue(array.hasArrayElements());
            assertEquals(3, array.getArraySize());
            assertEquals(42, array.getArrayElement(1).asInt());
        }
    }

    @Test
    public void timeoutExecution() throws Throwable {
        Context context = RubyTest.createContext();

        // schedule a timeout in 1s
        TestingThread thread = new TestingThread(() -> {
            try {
                Thread.sleep(1000);
                context.close(true);
            } catch (InterruptedException e) {
                throw new Error(e);
            } catch (PolyglotException e) {
                if (e.isCancelled()) {
                    assertTrue(e.isCancelled());
            