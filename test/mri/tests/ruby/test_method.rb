# -*- coding: us-ascii -*-
# frozen_string_literal: false
require 'test/unit'

class TestMethod < Test::Unit::TestCase
  def setup
    @verbose = $VERBOSE
  end

  def teardown
    $VERBOSE = @verbose
  end

  def m0() end
  def m1(a) end
  def m2(a, b) end
  def mo1(a = nil, &b) end
  def mo2(a, b = nil) end
  def mo3(*a) end
  def mo4(a, *b, &c) end
  def mo5(a, *b, c) end
  def mo6(a, *b, c, &d) end
  def mo7(a, b = nil, *c, d, &e) end
  def mo8(a, b = nil, *, d, &e) end
  def ma1((a), &b) nil && a end
  def mk1(**) end
  def mk2(**o) nil && o end
  def mk3(a, **o) nil && o end
  def mk4(a = nil, **o) nil && o end
  def mk5(a, b = nil, **o) nil && o end
  def mk6(a, b = nil, c, **o) nil && o end
  def mk7(a, b = nil, *c, d, **o) nil && o end
  def mk8(a, b = nil, *c, d, e:, f: nil, **o) nil && o end
  def mnk(**nil) end
  def mf(...) end

  class Base
    def foo() :base end
  end
  class Derived < Base
    def foo() :derived end
  end
  class T
    def initialize; end
    def initialize_copy(*) super end
    def initialize_clone(*) super end
    def initialize_dup(*) super end
    def respond_to_missing?(*) super end
    def normal_method; end
  end
  module M
    def func; end
    module_function :func
    def meth; :meth end
  end

  def mv1() end
  def mv2() end
  private :mv2
  def mv3() end
  protected :mv3

  class Visibility
    def mv1() end
    def mv2() end
    private :mv2
    def mv3() end
    protected :mv3
  end

  def test_arity
    assert_equal(0, method(:m0).arity)
    assert_equal(1, method(:m1).arity)
    assert_equal(2, method(:m2).arity)
    assert_equal(-1, method(:mo1).arity)
    assert_equal(-2, method(:mo2).arity)
    assert_equal(-1, method(:mo3).arity)
    assert_equal(-2, method(:mo4).arity)
    assert_equal(-3, method(:mo5).arity)
    assert_equal(-3, method(:mo6).arity)
    assert_equal(-1, method(:mk1).arity)
    assert_equal(-1, method(:mk2).arity)
    assert_equal(-2, method(:mk3).arity)
    assert_equal(-1, method(:mk4).arity)
    assert_equal(-2, method(:mk5).arity)
    assert_equal(-3, method(:mk6).arity)
    assert_equal(-3, method(:mk7).arity)
  end

  def test_arity_special
    assert_equal(-1, method(:__send__).arity)
  end

  def test_unbind
    assert_equal(:derived, Derived.new.foo)
    um = Derived.new.method(:foo).unbind
    assert_instance_of(UnboundMethod, um)
    Derived.class_eval do
      def foo() :changed end
    end
    assert_equal(:changed, Derived.new.foo)
    assert_equal(:derived, um.bind(Derived.new).call)
    assert_raise(TypeError) do
      um.bind(Base.new)
    end

    # cleanup
    Derived.class_eval do
      remove_method :foo
      def foo() :derived; end
    end
  end

  def test_callee
    assert_equal(:test_callee, __method__)
    assert_equal(:m, Class.new {def m; __method__; end}.new.m)
    assert_equal(:m, Class.new {def m; tap{return __method__}; end}.new.m)
    assert_equal(:m, Class.new {define_method(:m) {__method__}}.new.m)
    assert_equal(:m, Class.new {define_method(