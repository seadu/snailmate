# frozen_string_literal: false
require 'test/unit'
require 'objspace'

class TestRubyOptimization < Test::Unit::TestCase
  def assert_redefine_method(klass, method, code, msg = nil)
    assert_separately([], "#{<<-"begin;"}\n#{<<~"end;"}")
    begin;
      class #{klass}
        undef #{method}
        def #{method}(*args)
          args[0]
        end
      end
      #{code}
    end;
  end

  def disasm(name)
    RubyVM::InstructionSequence.of(method(name)).disasm
  end

  def test_fixnum_plus
    assert_equal 21, 10 + 11
    assert_redefine_method('Integer', '+', 'assert_equal 11, 10 + 11')
  end

  def test_fixnum_minus
    assert_equal 5, 8 - 3
    assert_redefine_method('Integer', '-', 'assert_equal 3, 8 - 3')
  end

  def test_fixnum_mul
    assert_equal 15, 3 * 5
    assert_redefine_method('Integer', '*', 'assert_equal 5, 3 * 5')
  end

  def test_fixnum_div
    assert_equal 3, 15 / 5
    assert_redefine_method('Integer', '/', 'assert_equal 5, 15 / 5')
  end

  def test_fixnum_mod
    assert_equal 1, 8 % 7
    assert_redefine_method('Integer', '%', 'assert_equal 7, 8 % 7')
  end

  def test_fixnum_lt
    assert_equal true, 1 < 2
    assert_redefine_method('Integer', '<', 'assert_equal 2, 1 < 2')
  end

  def test_fixnum_le
    assert_equal true, 1 <= 2
    assert_redefine_method('Integer', '<=', 'assert_equal 2, 1 <= 2')
  end

  def test_fixnum_gt
    assert_equal false, 1 > 2
    assert_redefine_method('Integer', '>', 'assert_equal 2, 1 > 2')
  end

  def test_fixnum_ge
    assert_equal false, 1 >= 2
    assert_redefine_method('Integer', '>=', 'assert_equal 2, 1 >= 2')
  end

  def test_float_plus
    assert_equal 4.0, 2.0 + 2.0
    assert_redefine_method('Float', '+', 'assert_equal 2.0, 2.0 + 2.0')
  end

  def test_float_minus
    assert_equal 4.0, 2.0 + 2.0
    assert_redefine_method('Float', '+', 'assert_equal 2.0, 2.0 + 2.0')
  end

  def test_float_mul
    assert_equal 29.25, 4.5 * 6.5
    assert_redefine_method('Float', '*', 'assert_equal 6.5, 4.5 * 6.5')
  end

  def test_float_div
    assert_in_delta 0.63063063063063063, 4.2 / 6.66
    assert_redefine_method('Float', '/', 'assert_equal 6.66, 4.2 / 6.66', "[Bug #9238]")
  end

  def test_float_lt
    assert_equal true, 1.1 < 2.2
    assert_redefine_method('Float', '<', 'assert_equal 2.2, 1.1 < 2.2')
  end

  def test_float_le
    assert_equal true, 1.1 <= 2.2
    assert_redefine_method('Float', '<=', 'assert_equal 2.2, 1.1 <= 2.2')
  end

  def test_float_gt
    assert_equal false, 1.1 > 2.2
    assert_redefine_method('Float', '>', 'assert_equal 2.2, 1.1 > 2.2')
  end

  def test_float_ge
    assert_equal false, 1.1 >= 2.2
    assert_redefine_method('Float', '>=', 'assert_equal 2.2, 1.1 >= 2.2')
  end

  def test_string_length
    assert_equal 6, "string".length
    assert_redefine_method('String', 'length', 'assert_nil "string".length')
  end

  def test_string_size
    assert_equal 6, "string".size
    assert_redefine_method('String', 'size', 'assert_nil "string".size')
  end

  def test_string_empty?
    assert_equal true, "".empty?
    assert_equal false, "string".empty?
    assert_redefine_method('String', 'empty?', 'assert_nil "string".empty?')
  end

  def test_string_plus
    assert_equal "", "" + ""
    assert_equal "x", "x" + ""
    assert_equal "x", "" + "x"
    assert_equal "ab", "a" + "b"
    assert_redefine_method('String', '+', 'assert_equal "b", "a" + "b"')
  end

  def test_string_succ
    assert_equal 'b', 'a'.succ
    assert_equal 'B', 'A'.succ
  end

  def test_string_format
    assert_equal '2', '%d' % 2
    assert_redefine_method('String', '%', 'assert_equal 2, "%d" % 2')
  end

  def test_string_freeze
    assert_equal "foo", "foo".freeze
    assert_equal "foo".freeze.object_id, "foo".freeze.object_id
    assert_redefine_method('String', 'freeze', 'assert_nil "foo".freeze')
  end

  def test_string_uminus
    assert_same "foo".freeze, -"foo"
    assert_redefine_method('String', '-@', 'assert_nil(-"foo")')
  end

  def test_array_min
    assert_equal 1, [1, 2, 4].min
    assert_redefine_method('Array', 'min', 'assert_nil([1, 2, 4].min)')
    assert_redefine_method('Array', 'min', 'assert_nil([1 + 0, 2, 4].min)')
  end

  def test_array_max
    assert_equal 4, [1, 2, 4].max
    assert_redefine_method('Array', 'max', 'assert_nil([1, 2, 4].max)')
    assert_redefine_method('Array', 'max', 'assert_nil([1 + 0, 2, 4].max)')
  end

  def test_trace_optimized_methods
    bug14870 = "[ruby-core:87638]"
    expected = [:-@, :max, :min, :+, :-, :*, :/, :%, :==, :<, :<=, :>, :>=, :<<,
                :&, :|, :[], :[]=, :length, :empty?, :nil?, :succ, :!, :=~]
    [:c_call, :c_return].each do |type|
      methods = []
      tp = TracePoint.new(type) { |tp| methods << tp.method_id }
      tp.enable do
        x = "a"; x = -x
        [1].max
        [1].min
        x = 42 + 2
        x = 42 - 2
        x = 42 * 2
        x = 42 / 2
        x = 42 % 2
        y = x == 42
        y = x < 42
        y = x <= 42
        y = x > 42
        y = x >= 42
        x = x << 1
        x = x & 1
        x = x | 1
        x = []; x[1]
        x[1] = 2
        x.length
        x.empty?
        x.nil?
        x = 1; x.succ
        !x
        x = 'a'; x =~ /a/
        x = y
      end
      assert_equal(expected, methods, bug14870)
    end

    methods = []
    tp = TracePoint.new(:c_call, :c_return) { |tp| methods << tp.method_id }
    tp.enable do
      x = 1
      x != 42
    end
    assert_equal([:!=, :==, :==, :!=], methods, bug14870)
  end

  def test_string_freeze_saves_memory
    n = 16384
    dat