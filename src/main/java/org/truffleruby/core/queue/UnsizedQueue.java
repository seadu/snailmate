/*
 * Copyright (c) 2017, 2023 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
package org.truffleruby.core.queue;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Objects;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

import com.oracle.truffle.api.CompilerDirectives.TruffleBoundary;

public class UnsizedQueue {

    private final ReentrantLock lock;
    private final Condition canTake;

    private Item addEnd;
    private Item takeEnd;
    private int size;
    private boolean closed;

    public static final Object CLOSED = new Object();

    @TruffleBoundary
    public UnsizedQueue() {
        this.lock = new ReentrantLock();
        this.canTake = lock.newCondition();
    }

    @TruffleBoundary
    public boolean add(Object item) {
        lock.lock();

        try {
            if (closed) {
                return false;
            }

            doAdd(item);

            return true;
        } finally {
            lock.unlock();
        }
    }

    private void doAdd(Object item) {
        final Item newItem = new Item(item);
        if (addEnd != null) {
            addEnd.setNextToTake(newItem);
        }
        addEnd = newItem;
        if (takeEnd == null) {
            takeEnd = addEnd;
        }
        size++;
        canTake.signal();
    }

    @TruffleBoundary
    public boolean addAll(Object[] items) {
        lock.lock();

        try {
            if (closed) {
                return false;
            }

            for (Object i : items) {
                doAdd(i);
            }

            return true;
        } finally {
            lock.unlock();
        }
    }

    @TruffleBoundary
    public Object take() throws InterruptedException {
        lock.lock();

        try {
            while (takeEnd == null) {
                if (closed) {
                    return CLOSED;
                }

                canTake.await();

                /* We need to check the interrupted flag here, while holding the lock and after the await(). Otherwise,
                 * if another thread does {@code th.raise(exc); q.push(1)} like in core/thread/handle_interrupt_spec.rb
                 * then we might miss the ThreadLocalAction and instead return, which would incorrectly delay the
                 * ThreadLocalAction if under {@code Thread.handle_interrupt(Object => :on_blocking)}. The other thread
                 * cannot wait on the Future, as that might block arbitrary long when side-effects are disabled on this
                 * thread. */
                if (Thread.interrupted()) {
                    throw new InterruptedException();
                }
            }

            return doTake();
        } finally {
            lock.unlock();
        }
    }

    @TruffleBoundary
    public Object poll(long timeoutMilliseconds) throws InterruptedException {
        lock.lock();

        try {
            if (takeEnd 