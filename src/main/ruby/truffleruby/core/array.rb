# frozen_string_literal: true

# Copyright (c) 2015, 2023 Oracle and/or its affiliates. All rights reserved. This
# code is released under a tri EPL/GPL/LGPL license. You can use it,
# redistribute it and/or modify it under the terms of the:
#
# Eclipse Public License version 2.0, or
# GNU General Public License version 2, or
# GNU Lesser General Public License version 2.1.

# Copyright (c) 2007-2015, Evan Phoenix and contributors
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# * Neither the name of Rubinius nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

class Array
  include Enumerable

  # The flow control for many of these methods is
  # pretty evil due to how MRI works. There is
  # also a lot of duplication of code due to very
  # subtle processing differences and, in some
  # cases, to avoid mutual dependency. Apologies.


  def self.[](*args)
    ary = allocate
    ary.replace args
    ary
  end

  # Try to convert obj into an array, using to_ary method.
  # Returns converted array or nil if obj cannot be converted
  # for any reason. This method is to check if an argument is an array.
  def self.try_convert(obj)
    Truffle::Type.try_convert obj, Array, :to_ary
  end

  def &(other)
    other = Truffle::Type.coerce_to other, Array, :to_ary

    h = {}
    other.each { |e| h[e] = true }
    select { |x| h.delete x }
  end

  def |(other)
    other = Truffle::Type.coerce_to other, Array, :to_ary

    h = {}
    each { |e| h[e] = true }
    other.each { |e| h[e] = true }
    h.keys
  end

  def -(other)
    other = Truffle::Type.coerce_to other, Array, :to_ary

    h = {}
    other.each { |e| h[e] = true }
    reject { |x| h.include? x }
  end

  def <=>(other)
    other = Truffle::Type.rb_check_convert_type other, Array, :to_ary
    return 0 if equal? other
    return nil if Primitive.nil? other

    total = other.size

    Truffle::ThreadOperations.detect_pair_recursion self, other do
      i = 0
      count = Primitive.min(total, size)

      while i < count
        order = self[i] <=> other[i]
        return order unless order == 0

        i += 1
      end
    end

    # subtle: if we are recursing on that pair, then let's
    # no go any further down into that pair;
    # any difference will be found elsewhere if need be
    size <=> total
  end

  def *(count)
    result = Primitive.array_mul(self, count)

    if !Primitive.undefined?(result)
      result
    elsif str = Truffle::Type.rb_check_convert_type(count, String, :to_str)
      join(str)
    else
      self * Truffle::Type.coerce_to(count, Integer, :to_int)
    end
  end

  def ==(other)
    result = Primitive.array_equal self, other
    unless Primitive.undefined?(result)
      return result
    end

    return true if equal?(other)
    unless Primitive.object_kind_of?(other, Array)
      return false unless other.respond_to? :to_ary
      return other == self
    end

    return false unless size == other.size

    Truffle::ThreadOperations.detect_pair_recursion self, other do
      i = 0
      total = size

      while i < total
        return false unless Primitive.object_same_or_equal(self[i], other[i])
        i += 1
      end
    end

    true
  end

  private def slice_arithmetic_sequence(seq)
    len = size

    if seq.step < 0 # inverse range with negative step
      start = seq.end
      stop  = seq.begin
      step  = seq.step
    else
      start = seq.begin
      stop  = seq.end
      step  = seq.step
    end

    start ||= 0 # begin-less range
    stop ||= -1 # endless range

    # negative indexes refer to the end of array
    start += len if start < 0
    stop  += len if stop < 0

    stop += 1 unless seq.exclude_end?
    diff = stop - start

    is_out_of_bound = start < 0 || start > len

    if step < -1 || step > 1
      raise RangeError, "#{seq.inspect} out of range" if is_out_of_bound || diff > len
    elsif is_out_of_bound
      return nil
    end

    return [] if diff <= 0

    diff = len - start if (len < diff || len < start + diff)

    return self[start, diff] if step == 1 # step == 1 is a simple slice

    # optimize when no step will be done and only start element is returned
    return self[start, 1] if (step > 0 && step > diff) || (step < 0 && step < -diff)

    ustep = step.abs
    nlen = (diff + ustep - 1) / ustep
    i = 0
    j = start + (step > 0 ? 0 : diff - 1) # because we inverted negative step ranges
    res = Array.new(nlen)

    while i < nlen
      res[i] = self[j]
      i += 1
      j += step
    end

    res
  end

  def assoc(obj)
    each do |x|
      if Primitive.object_kind_of?(x, Array) and x.first == obj
        return x
      end
    end

    nil
  end

  def bsearch(&block)
    return to_enum :bsearch unless block_given?

    if idx = bsearch_index(&block)
      self[idx]
    else
      nil
    end
  end

  def bsearch_index
    return to_enum :bsearch_index unless block_given?

    min = 0
    max = total = size

    last_true = nil
    i = size / 2

    while max >= min and i >= 0 and i < total
      x = yield at(i)

      return i if x == 0

      case x
      when Numeric
        if x > 0
          min = i + 1
        else
          max = i - 1
        end
      when true
        last_true = i
        max = i - 1
      when false, nil
        min = i + 1
      else
        raise TypeError, 'wrong argument type (must be numeric, true, false or nil)'
      end

      i = min + (max - min) / 2
    end

    return i if max > min
    return last_true if last_true

    nil
  end

  def combination(num)
    num = Primitive.rb_num2int num

    unless block_given?
      return to_enum(:combination, num) do
        combination_size(num)
      end
    end

    if num == 0
      yield []
    elsif num == 1
      each do |i|
        yield [i]
      end
    elsif num == size
      yield self.dup
    elsif num >= 0 && num < size
      stack = Array.new(num + 1, 0)
      chosen = Array.new(num)
      lev = 0
      done = false
      stack[0] = -1
      until done
        chosen[lev] = self.at(stack[lev+1])
        while lev < num - 1
          lev += 1
          chosen[lev] = self.at(stack[lev+1] = stack[lev] + 1)
        end
        yield chosen.dup
        lev += 1
        begin
          done = lev == 0
          stack[lev] += 1
          lev -= 1
        end while stack[lev+1] + num == size + lev + 1
      end
    end
    self
  end

  def count(item = undefined)
    seq = 0
    if !Primitive.undefined?(item)
      Primitive.warn_given_block_not_used if block_given?
      each { |o| seq += 1 if item == o }
    elsif block_given?
      each { |o| seq += 1 if yield(o) }
    else
      return size
    end
    seq
  end

  def cycle(n = nil)
    unless block_given?
      return to_enum(:cycle, n) do
        Truffle::EnumerableOperations.cycle_size(size, n)
      end
    end

    return nil if empty?

    if nil.equal? n
      until empty?
        each { |x| yield x }
      end
    else
      n = Primitive.rb_num2int n
      n.times do
        each { |x| yield x }
      end
    end
    nil
  end

  def delete_if(&block)
    return to_enum(:delete_if) { size } unless block_given?
    reject!(&block)
    self
  end

  def difference(*others)
    other = []
    others.each { |a| other = other | a }
    self - other
  end

  def dig(idx, *more)
    result = self.at(idx)
    if Primitive.nil?(result) || more.empty?
      result
    else
      Truffle::Diggable.dig(result, more)
    end
  end

  def deconstruct
    self
  end

  def each_index
    return to_enum(:each_index) { size } unless block_given?

    i = 0

    while i < size
      yield i
      i += 1
    end

    self
  end

  def eql?(other)
    result = Primitive.array_eql self, other
    unless Primitive.undefined?(result)
      return result
    end

    return true if equal? other
    return false unless Primitive.object_kind_of?(other, Array)
    return false if size != other.size

    Truffle::ThreadOperations.detect_pair_recursion self, other do
      i = 0
      each do |x|
        return false unless x.eql? other[i]
        i += 1
      end
    end

    true
  end

  def empty?
    size == 0
  end

  def fetch(idx, default=undefined)
    orig = idx
    idx = Primitive.rb_num2int idx

    idx += size if idx < 0

    if idx < 0 or idx >= size
      if block_given?
        Primitive.warn_block_supersedes_default_value_argument unless Primitive.undefined?(default)

        return yield(orig)
      end

      return default unless Primitive.undefined?(default)

      raise IndexError, "index #{idx} out of bounds"
    end

    at(idx)
  end

  private def fill_internal(a=undefined, b=undefined, c=undefined)
    Primitive.check_frozen self

    if block_given?
      unless Primitive.undefined?(c)
        raise ArgumentError, 'wrong number of arguments'
      end
      index = a
      length = b
    else
      if Primitive.undefined?(a)
        raise ArgumentError, 'wrong number of arguments'
      end
      obj = a
      index = b
      length = c
    end

    if Primitive.undefined?(index) || !index
      left = 0
      right = size
    elsif Primitive.object_kind_of?(index, Range)
      raise TypeError, 'length invalid with range' unless Primitive.undefined?(length)

      left, length = Primitive.range_normalized_start_length(index, size)
      raise RangeError, "#{index.inspect} out of range" if left < 0
      right = left + length
      return self if right <= left # Nothing to modify

    elsif index
      left = Primitive.rb_num2int index
      left += size if left < 0
      left = 0 if left < 0

      if !Primitive.undefined?(length) and length
        