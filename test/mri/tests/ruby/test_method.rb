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
    assert_equal(:m, Class.new {define_method(:m) {tap{return __method__}}}.new.m)
    assert_nil(eval("class TestCallee; __method__; end"))

    assert_equal(:test_callee, __callee__)
    [
      ["method",              Class.new {def m; __callee__; end},],
      ["block",               Class.new {def m; tap{return __callee__}; end},],
      ["define_method",       Class.new {define_method(:m) {__callee__}}],
      ["define_method block", Class.new {define_method(:m) {tap{return __callee__}}}],
    ].each do |mesg, c|
      c.class_eval {alias m2 m}
      o = c.new
      assert_equal(:m, o.m, mesg)
      assert_equal(:m2, o.m2, mesg)
    end
    assert_nil(eval("class TestCallee; __callee__; end"))
  end

  def test_orphan_callee
    c = Class.new{def foo; proc{__callee__}; end; alias alias_foo foo}
    assert_equal(:alias_foo, c.new.alias_foo.call, '[Bug #11046]')
  end

  def test_method_in_define_method_block
    bug4606 = '[ruby-core:35386]'
    c = Class.new do
      [:m1, :m2].each do |m|
        define_method(m) do
          __method__
        end
      end
    end
    assert_equal(:m1, c.new.m1, bug4606)
    assert_equal(:m2, c.new.m2, bug4606)
  end

  def test_method_in_block_in_define_method_block
    bug4606 = '[ruby-core:35386]'
    c = Class.new do
      [:m1, :m2].each do |m|
        define_method(m) do
          tap { return __method__ }
        end
      end
    end
    assert_equal(:m1, c.new.m1, bug4606)
    assert_equal(:m2, c.new.m2, bug4606)
  end

  def test_body
    o = Object.new
    def o.foo; end
    assert_nothing_raised { RubyVM::InstructionSequence.disasm(o.method(:foo)) }
    assert_nothing_raised { RubyVM::InstructionSequence.disasm("x".method(:upcase)) }
    assert_nothing_raised { RubyVM::InstructionSequence.disasm(method(:to_s).to_proc) }
  end

  def test_new
    c1 = Class.new
    c1.class_eval { def foo; :foo; end }
    c2 = Class.new(c1)
    c2.class_eval { private :foo }
    o = c2.new
    o.extend(Module.new)
    assert_raise(NameError) { o.method(:bar) }
    assert_raise(NameError) { o.public_method(:foo) }
    assert_equal(:foo, o.method(:foo).call)
  end

  def test_eq
    o = Object.new
    class << o
      def foo; end
      alias bar foo
      def baz; end
    end
    assert_not_equal(o.method(:foo), nil)
    m = o.method(:foo)
    def m.foo; end
    assert_not_equal(o.method(:foo), m)
    assert_equal(o.method(:foo), o.method(:foo))
    assert_equal(o.method(:foo), o.method(:bar))
    assert_not_equal(o.method(:foo), o.method(:baz))
  end

  def test_hash
    o = Object.new
    def o.foo; end
    assert_kind_of(Integer, o.method(:foo).hash)
    assert_equal(Array.instance_method(:map).hash, Array.instance_method(:collect).hash)
    assert_kind_of(String, o.method(:foo).hash.to_s)
  end

  def test_owner
    c = Class.new do
      def foo; end
    end
    assert_equal(c, c.instance_method(:foo).owner)
    c2 = Class.new(c)
    assert_equal(c, c2.instance_method(:foo).owner)
  end

  def test_owner_missing
    c = Class.new do
      def respond_to_missing?(name, bool)
        name == :foo
      end
    end
    c2 = Class.new(c)
    assert_equal(c, c.new.method(:foo).owner)
    assert_equal(c2, c2.new.method(:foo).owner)
  end

  def test_receiver_name_owner
    o = Object.new
    def o.foo; end
    m = o.method(:foo)
    assert_equal(o, m.receiver)
    assert_equal(:foo, m.name)
    assert_equal(class << o; self; end, m.owner)
    assert_equal(:foo, m.unbind.name)
    assert_equal(class << o; self; end, m.unbind.owner)
    class << o
      alias bar foo
    end
    m = o.method(:bar)
    assert_equal(:bar, m.name)
    assert_equal(:foo, m.original_name)
  end

  def test_instance_method
    c = Class.new
    c.class_eval do
      def foo; :foo; end
      private :foo
    end
    o = c.new
    o.method(:foo).unbind
    assert_raise(NoMethodError) { o.foo }
    c.instance_method(:foo).bind(o)
    assert_equal(:foo, o.instance_eval { foo })
    assert_raise(NameError) { c.public_instance_method(:foo) }
    def o.bar; end
    m = o.method(:bar).unbind
    assert_raise(TypeError) { m.bind(Object.new) }

    cx = EnvUtil.labeled_class("X\u{1f431}")
    assert_raise_with_message(TypeError, /X\u{1f431}/) do
      o.method(cx)
    end
  end

  def test_bind_module_instance_method
    feature4254 = '[ruby-core:34267]'
    m = M.instance_method(:meth)
    assert_equal(:meth, m.bind(Object.new).call, feature4254)
  end

  def test_define_method
    c = Class.new
    c.class_eval { def foo; :foo; end }
    o = c.new
    def o.bar; :bar; end
    assert_raise(TypeError) do
      c.class_eval { define_method(:foo, :foo) }
    end
    assert_raise(ArgumentError) do
      c.class_eval { define_method }
    end
    c2 = Class.new(c)
    c2.class_eval { define_method(:baz, o.method(:foo)) }
    assert_equal(:foo, c2.new.baz)
    assert_raise(TypeError) do
      Class.new.class_eval { define_method(:foo, o.method(:foo)) }
    end
    assert_raise(TypeError) do
      Class.new.class_eval { define_method(:bar, o.method(:bar)) }
    end
    cx = EnvUtil.labeled_class("X\u{1f431}")
    assert_raise_with_message(TypeError, /X\u{1F431}/) do
      Class.new {define_method(cx) {}}
    end
  end

  def test_define_method_no_proc
    o = Object.new
    def o.foo(c)
      c.class_eval { define_method(:foo) }
    end
    c = Class.new
    assert_raise(ArgumentError) {o.foo(c)}

    bug11283 = '[ruby-core:69655] [Bug #11283]'
    assert_raise(ArgumentError, bug11283) {o.foo(c) {:foo}}
  end

  def test_define_singleton_method
    o = Object.new
    o.instance_eval { define_singleton_method(:foo) { :foo } }
    assert_equal(:foo, o.foo)
  end

  def test_define_singleton_method_no_proc
    o = Object.new
    assert_raise(ArgumentError) {
      o.instance_eval { define_singleton_method(:bar) }
    }

    bug11283 = '[ruby-core:69655] [Bug #11283]'
    def o.define(n)
      define_singleton_method(n)
    end
    assert_raise(ArgumentError, bug11283) {o.define(:bar) {:bar}}
  end

  def test_define_method_invalid_arg
    assert_raise(TypeError) do
      Class.new.class_eval { define_method(:foo, Object.new) }
    end

    assert_raise(TypeError) do
      Module.new.module_eval {define_method(:foo, Base.instance_method(:foo))}
    end
  end

  def test_define_singleton_method_with_extended_method
    bug8686 = "[ruby-core:56174]"

    m = Module.new do
      extend self

      def a
        "a"
      end
    end

    assert_nothing_raised(bug8686) do
      m.define_singleton_method(:a, m.method(:a))
    end
  end

  def test_define_method_transplating
    feature4254 = '[ruby-core:34267]'
    m = Module.new {define_m