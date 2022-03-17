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
    m = Module.new {define_method(:meth, M.instance_method(:meth))}
    assert_equal(:meth, Object.new.extend(m).meth, feature4254)
    c = Class.new {define_method(:meth, M.instance_method(:meth))}
    assert_equal(:meth, c.new.meth, feature4254)
  end

  def test_define_method_visibility
    c = Class.new do
      public
      define_method(:foo) {:foo}
      protected
      define_method(:bar) {:bar}
      private
      define_method(:baz) {:baz}
    end

    assert_equal(true, c.public_method_defined?(:foo))
    assert_equal(false, c.public_method_defined?(:bar))
    assert_equal(false, c.public_method_defined?(:baz))

    assert_equal(false, c.protected_method_defined?(:foo))
    assert_equal(true, c.protected_method_defined?(:bar))
    assert_equal(false, c.protected_method_defined?(:baz))

    assert_equal(false, c.private_method_defined?(:foo))
    assert_equal(false, c.private_method_defined?(:bar))
    assert_equal(true, c.private_method_defined?(:baz))

    m = Module.new do
      module_function
      define_method(:foo) {:foo}
    end
    assert_equal(true, m.respond_to?(:foo))
    assert_equal(false, m.public_method_defined?(:foo))
    assert_equal(false, m.protected_method_defined?(:foo))
    assert_equal(true, m.private_method_defined?(:foo))
  end

  def test_define_method_in_private_scope
    bug9005 = '[ruby-core:57747] [Bug #9005]'
    c = Class.new
    class << c
      public :define_method
    end
    TOPLEVEL_BINDING.eval("proc{|c|c.define_method(:x) {|x|throw x}}").call(c)
    o = c.new
    assert_throw(bug9005) {o.x(bug9005)}
  end

  def test_singleton_define_method_in_private_scope
    bug9141 = '[ruby-core:58497] [Bug #9141]'
    o = Object.new
    class << o
      public :define_singleton_method
    end
    TOPLEVEL_BINDING.eval("proc{|o|o.define_singleton_method(:x) {|x|throw x}}").call(o)
    assert_throw(bug9141) do
      o.x(bug9141)
    end
  end

  def test_super_in_proc_from_define_method
    c1 = Class.new {
      def m
        :m1
      end
    }
    c2 = Class.new(c1) { define_method(:m) { Proc.new { super() } } }
    assert_equal(:m1, c2.new.m.call, 'see [Bug #4881] and [Bug #3136]')
  end

  def test_clone
    o = Object.new
    def o.foo; :foo; end
    m = o.method(:foo)
    def m.bar; :bar; end
    assert_equal(:foo, m.clone.call)
    assert_equal(:bar, m.clone.bar)
  end

  def test_inspect
    o = Object.new
    def o.foo; end; line_no = __LINE__
    m = o.method(:foo)
    assert_equal("#<Method: #{ o.inspect }.foo() #{__FILE__}:#{line_no}>", m.inspect)
    m = o.method(:foo)
    assert_match("#<UnboundMethod: #{ class << o; self; end.inspect }#foo() #{__FILE__}:#{line_no}", m.unbind.inspect)

    c = Class.new
    c.class_eval { def foo; end; }; line_no = __LINE__
    m = c.new.method(:foo)
    assert_equal("#<Method: #{ c.inspect }#foo() #{__FILE__}:#{line_no}>", m.inspect)
    m = c.instance_method(:foo)
    assert_equal("#<UnboundMethod: #{ c.inspect }#foo() #{__FILE__}:#{line_no}>", m.inspect)

    c2 = Class.new(c)
    c2.class_eval { private :foo }
    m2 = c2.new.method(:foo)
    assert_equal("#<Method: #{ c2.inspect }(#{ c.inspect })#foo() #{__FILE__}:#{line_no}>", m2.inspect)

    bug7806 = '[ruby-core:52048] [Bug #7806]'
    c3 = Class.new(c)
    c3.class_eval { alias bar foo }
    m3 = c3.new.method(:bar)
    assert_equal("#<Method: #{c3.inspect}(#{c.inspect})#bar(foo)() #{__FILE__}:#{line_no}>", m3.inspect, bug7806)

    bug15608 = '[ruby-core:91570] [Bug #15608]'
    c4 = Class.new(c)
    c4.class_eval { alias bar foo }
    o = c4.new
    o.singleton_class
    m4 = o.method(:bar)
    assert_equal("#<Method: #{c4.inspect}(#{c.inspect})#bar(foo)() #{__FILE__}:#{line_no}>", m4.inspect, bug15608)

    bug17428 = '[ruby-core:101635] [Bug #17428]'
    assert_equal("#<Method: #<Class:String>(Module)#prepend(*)>", String.method(:prepend).inspect, bug17428)

    c5 = Class.new(String)
    m = Module.new{def prepend; end; alias prep prepend}; line_no = __LINE__
    c5.extend(m)
    c6 = Class.new(c5)
    assert_equal("#<Method: #<Class:#{c6.inspect}>(#{m.inspect})#prep(prepend)() #{__FILE__}:#{line_no}>", c6.method(:prep).inspect, bug17428)
  end

  def test_callee_top_level
    assert_in_out_err([], "p __callee__", %w(nil), [])
  end

  def test_caller_top_level
    assert_in_out_err([], "p caller", %w([]), [])
  end

  def test_caller_negative_level
    assert_raise(ArgumentError) { caller(-1) }
  end

  def test_attrset_ivar
    c = Class.new
    c.class_eval { attr_accessor :foo }
    o = c.new
    o.method(:foo=).call(42)
    assert_equal(42, o.foo)
    assert_raise(ArgumentError) { o.method(:foo=).call(1, 2, 3) }
    assert_raise(ArgumentError) { o.method(:foo).call(1) }
  end

  def test_default_accessibility
    tmethods = T.public_instance_methods
    assert_include tmethods, :normal_method, 'normal methods are public by default'
    assert_not_include tmethods, :initialize, '#initialize is private'
    assert_not_include tmethods, :initialize_copy, '#initialize_copy is private'
    assert_not_include tmethods, :initialize_clone, '#initialize_clone is private'
    assert_not_include tmethods, :initialize_dup, '#initialize_dup is private'
    assert_not_include tmethods, :respond_to_missing?, '#respond_to_missing? is private'
    mmethods = M.public_instance_methods
    assert_not_include mmethods, :func, 'module methods are private by default'
    assert_include mmethods, :meth, 'normal methods are public by default'
  end

  def test_respond_to_missing_argument
    obj = Struct.new(:mid).new
    def obj.respond_to_missing?(id, *)
      self.mid = id
      true
    end
    assert_kind_of(Method, obj.method("bug15640"))
    assert_kind_of(Symbol, obj.mid)
    assert_equal("bug15640", obj.mid.to_s)

    arg = Struct.new(:to_str).new("bug15640_2")
    assert_kind_of(Method, obj.method(arg))
    assert_kind_of(Symbol, obj.mid)
    assert_equal("bug15640_2", obj.mid.to_s)
  end

  define_method(:pm0) {||}
  define_method(:pm1) {|a|}
  define_method(:pm2) {|a, b|}
  define_method(:pmo1) {|a = nil, &b|}
  define_method(:pmo2) {|a, b = nil|}
  define_method(:pmo3) {|*a|}
  define_method(:pmo4) {|a, *b, &c|}
  define_method(:pmo5) {|a, *b, c|}
  define_method(:pmo6) {|a, *b, c, &d|}
  define_method(:pmo7) {|a, b = nil, *c, d, &e|}
  define_method(:pma1) {|(a), &b| nil && a}
  define_method(:pmk1) {|**|}
  define_method(:pmk2) {|**o|}
  define_method(:pmk3) {|a, **o|}
  define_method(:pmk4) {|a = nil, **o|}
  define_method(:pmk5) {|a, b = nil, **o|}
  define_method(:pmk6) {|a, b = nil, c, **o|}
  define_method(:pmk7) {|a, b = nil, *c, d, **o|}
  define_method(:pmk8) {|a, b = nil, *c, d, e:, f: nil, **o|}
  define_method(:pmnk) {|**nil|}

  d