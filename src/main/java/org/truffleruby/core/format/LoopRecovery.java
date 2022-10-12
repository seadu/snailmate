/*
 * Copyright (c) 2015, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.core.format;

public abstract class LoopRecovery {

    private static final String DIRECTIVES = "CSLQcslqInjJNvVUwDdFfEeFfAaZBbHhuMmpPXx@";

    /** Format strings can sometimes be dynamically generated with code such as:
     * <p>
     * <code>'x' + ('NX' * size)</code>
     * <p>
     * This is problematic for us as it expands to:
     * <p>
     * <code>xNXNXNXNXNXNXNXNXNXNX...</code>
     * <p>
     * We will struggle to compile that with a large value for size because it will generate a huge number of nodes.
     * Even if we could compile it, it's not great for memory consumption or the instruction cache. Instead, we'd like
     * to recover the loop in there and convert it to:
     * <p>
     * <code>x(NX)1000</code>
     * <p>
     * We could try and do something really sophisticated here, with nested loops and finding the optimal pattern, but
     * for the moment we just look for one simple loop.
     * <p>
     * To do that, for each character we look 1..n characters behind and see if that pattern is repeated. If it is we
     * have the loop. We then keep going and see how many more times we can loop. Nothing more complicated than that. */
    public static String recoverLoop(String format) {
        // The index is the point in the format string where we look backwards for loops from

        int index = 0;

        // Keep going until we reach the end of the format string

        while (index < format.length()) {
            // If we aren't at the start of a new directive, step forward one

            if (DIRECTIVES.indexOf(format.charAt(index)) == -1) {
                index++;
                continue;
            }

            // The length of the string that will be tried to be looped - initially trying just one character

            int tryLengthOfLoopedString = 1;

            // The length of the string that will be looped, where there was actually a loop found

            int successfulLengthOfLoopedString = -1;

            // Increase the size of the string that will be tried to be looped - but only as far as the