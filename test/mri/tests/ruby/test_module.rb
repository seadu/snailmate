
# frozen_string_literal: false
require 'test/unit'
require 'pp'

$m0 = Module.nesting

class TestModule < Test::Unit::TestCase
  def _wrap_assertion
    yield
  end

  def assert_method_defined?(klass, mid, message="")
    message = build_message(message, "#{klass}\##{mid} expected to be defined.")
    _wrap_assertion do
      klass.method_defined?(mid) or
        raise Test::Unit::AssertionFailedError, message, caller(3)
    end
  end

  def assert_method_not_defined?(klass, mid, message="")
    message = build_message(message, "#{klass}\##{mid} expected to not be defined.")
    _wrap_assertion do
      klass.method_defined?(mid) and
        raise Test::Unit::AssertionFailedError, message, caller(3)
    end
  end

  def setup
    @verbose = $VERBOSE
    @deprecated = Warning[:deprecated]
    Warning[:deprecated] = true
  end

  def teardown
    $VERBOSE = @verbose
    Warning[:deprecated] = @deprecated
  end

  def test_LT_0
    assert_equal true, String < Object
    assert_equal false, Object < String
    assert_nil String < Array
    assert_equal true, Array < Enumerable
    assert_equal false, Enumerable < Array
    assert_nil Proc < Comparable
    assert_nil Comparable < Proc
  end

  def test_GT_0
    assert_equal false, String > Object
    assert_equal true, Object > String
    assert_nil String > Array
    assert_equal false, Array > Enumerable
    assert_equal true, Enumerable > Array
    assert_nil Comparable > Proc
    assert_nil Proc > Comparable
  end

  def test_CMP_0
    assert_equal(-1, (String <=> Object))
    assert_equal 1, (Object <=> String)
    assert_nil(Array <=> String)
  end

  ExpectedException = NoMethodError

  # Support stuff

  module Mixin
    MIXIN = 1
    def mixin
    end
  end

  module User
    USER = 2
    include Mixin
    def user
    end

    def user2
    end
    protected :user2

    def user3
    end
    private :user3
  end

  OtherSetup = -> do
    remove_const :Other if defined? ::TestModule::Other
    Other = Module.new do
      def other
      end
    end
  end

  class AClass
    def AClass.cm1
      "cm1"
    end
    def AClass.cm2
      cm1 + "cm2" + cm3
    end
    def AClass.cm3
      "cm3"
    end

    private_class_method :cm1, "cm3"

    def aClass
      :aClass
    end

    def aClass1
      :aClass1
    end

    def aClass2
      :aClass2
    end

    private :aClass1
    protected :aClass2
  end

  class BClass < AClass
    def bClass1
      :bClass1
    end

    private

    def bClass2
      :bClass2
    end

    protected
    def bClass3
      :bClass3
    end
  end

  class CClass < BClass
    def self.cClass
    end
  end

  MyClass = AClass.clone
  class MyClass
    public_class_method :cm1
  end

  # -----------------------------------------------------------

  def test_CMP # '<=>'
    assert_equal( 0, Mixin <=> Mixin)
    assert_equal(-1, User <=> Mixin)
    assert_equal( 1, Mixin <=> User)

    assert_equal( 0, Object <=> Object)
    assert_equal(-1, String <=> Object)
    assert_equal( 1, Object <=> String)
  end

  def test_GE # '>='
    assert_operator(Mixin,    :>=, User)
    assert_operator(Mixin,    :>=, Mixin)
    assert_not_operator(User, :>=, Mixin)

    assert_operator(Object,     :>=, String)
    assert_operator(String,     :>=, String)
    assert_not_operator(String, :>=, Object)
  end

  def test_GT # '>'
    assert_operator(Mixin,     :>, User)
    assert_not_operator(Mixin, :>, Mixin)
    assert_not_operator(User,  :>, Mixin)

    assert_operator(Object,     :>, String)
    assert_not_operator(String, :>, String)
    assert_not_operator(String, :>, Object)
  end

  def test_LE # '<='
    assert_operator(User,      :<=, Mixin)
    assert_operator(Mixin,     :<=, Mixin)
    assert_not_operator(Mixin, :<=, User)

    assert_operator(String,     :<=, Object)
    assert_operator(String,     :<=, String)
    assert_not_operator(Object, :<=, String)
  end

  def test_LT # '<'
    assert_operator(User,      :<, Mixin)
    assert_not_operator(Mixin, :<, Mixin)
    assert_not_operator(Mixin, :<, User)

    assert_operator(String,     :<, Object)
    assert_not_operator(String, :<, String)
    assert_not_operator(Object, :<, String)
  end

  def test_VERY_EQUAL # '==='
    assert_operator(Object, :===, self)
    assert_operator(Test::Unit::TestCase, :===, self)
    assert_operator(TestModule, :===, self)
    assert_not_operator(String, :===, self)
  end

  def test_ancestors
    assert_equal([User, Mixin],      User.ancestors)
    assert_equal([Mixin],            Mixin.ancestors)

    ancestors = Object.ancestors
    mixins = ancestors - [Object, Kernel, BasicObject]
    mixins << JSON::Ext::Generator::GeneratorMethods::String if defined?(JSON::Ext::Generator::GeneratorMethods::String)
    assert_equal([Object, Kernel, BasicObject], ancestors - mixins)
    assert_equal([String, Comparable, Object, Kernel, BasicObject], String.ancestors - mixins)
  end

  CLASS_EVAL = 2
  @@class_eval = 'b'

  def test_class_eval
    OtherSetup.call

    Other.class_eval("CLASS_EVAL = 1")
    assert_equal(1, Other::CLASS_EVAL)
    assert_include(Other.constants, :CLASS_EVAL)
    assert_equal(2, Other.class_eval { CLASS_EVAL })

    Other.class_eval("@@class_eval = 'a'")
    assert_equal('a', Other.class_variable_get(:@@class_eval))
    assert_equal('b', Other.class_eval { @@class_eval })

    Other.class_eval do
      module_function

      def class_eval_test
        "foo"
      end
    end
    assert_equal("foo", Other.class_eval_test)

    assert_equal([Other], Other.class_eval { |*args| args })
  end

  def test_const_defined?
    assert_operator(Math, :const_defined?, :PI)
    assert_operator(Math, :const_defined?, "PI")
    assert_not_operator(Math, :const_defined?, :IP)
    assert_not_operator(Math, :const_defined?, "IP")
  end

  def each_bad_constants(m, &b)
    [
      "#<Class:0x7b8b718b>",
      ":Object",
      "",
      ":",
      ["String::", "[Bug #7573]"],
      "\u3042",
      "Name?",
    ].each do |name, msg|
      expected = "wrong constant name %s" % name
      msg = "#{msg}#{': ' if msg}wrong constant name #{name.dump}"
      assert_raise_with_message(NameError, Regexp.compile(Regexp.quote(expected)), "#{msg} to #{m}") do
        yield name
      end
    end
  end

  def test_bad_constants_get
    each_bad_constants("get") {|name|
      Object.const_get name
    }
  end

  def test_bad_constants_defined
    each_bad_constants("defined?") {|name|
      Object.const_defined? name
    }
  end

  def test_leading_colons
    assert_equal Object, AClass.const_get('::Object')
  end

  def test_const_get
    assert_equal(Math::PI, Math.const_get("PI"))
    assert_equal(Math::PI, Math.const_get(:PI))

    n = Object.new
    def n.to_str; @count = defined?(@count) ? @count + 1 : 1; "PI"; end
    def n.count; @count; end
    assert_equal(Math::PI, Math.const_get(n))
    assert_equal(1, n.count)
  end

  def test_nested_get
    OtherSetup.call

    assert_equal Other, Object.const_get([self.class, 'Other'].join('::'))
    assert_equal User::USER, self.class.const_get([User, 'USER'].join('::'))
    assert_raise(NameError) {
      Object.const_get([self.class.name, 'String'].join('::'))
    }
  end

  def test_nested_get_symbol
    OtherSetup.call

    const = [self.class, Other].join('::').to_sym
    assert_raise(NameError) {Object.const_get(const)}

    const = [User, 'USER'].join('::').to_sym
    assert_raise(NameError) {self.class.const_get(const)}
  end

  def test_nested_get_const_missing
    classes = []
    klass = Class.new {
      define_singleton_method(:const_missing) { |name|
        classes << name
        klass
      }
    }
    klass.const_get("Foo::Bar::Baz")
    assert_equal [:Foo, :Bar, :Baz], classes
  end

  def test_nested_get_bad_class
    assert_raise(TypeError) do
      self.class.const_get([User, 'USER', 'Foo'].join('::'))
    end
  end

  def test_nested_defined
    OtherSetup.call

    assert_send([Object, :const_defined?, [self.class.name, 'Other'].join('::')])
    assert_send([self.class, :const_defined?, 'User::USER'])
    assert_not_send([self.class, :const_defined?, 'User::Foo'])
    assert_not_send([Object, :const_defined?, [self.class.name, 'String'].join('::')])
  end

  def test_nested_defined_symbol
    OtherSetup.call

    const = [self.class, Other].join('::').to_sym
    assert_raise(NameError) {Object.const_defined?(const)}

    const = [User, 'USER'].join('::').to_sym
    assert_raise(NameError) {self.class.const_defined?(const)}
  end

  def test_nested_defined_inheritance
    assert_send([Object, :const_defined?, [self.class.name, 'User', 'MIXIN'].join('::')])
    assert_send([self.class, :const_defined?, 'User::MIXIN'])
    assert_send([Object, :const_defined?, 'File::SEEK_SET'])

    # const_defined? with `false`
    assert_not_send([Object, :const_defined?, [self.class.name, 'User', 'MIXIN'].join('::'), false])
    assert_not_send([self.class, :const_defined?, 'User::MIXIN', false])
    assert_not_send([Object, :const_defined?, 'File::SEEK_SET', false])
  end

  def test_nested_defined_bad_class
    assert_raise(TypeError) do
      self.class.const_defined?('User::USER::Foo')
    end
  end

  def test_const_set
    OtherSetup.call

    assert_not_operator(Other, :const_defined?, :KOALA)
    Other.const_set(:KOALA, 99)
    assert_operator(Other, :const_defined?, :KOALA)
    assert_equal(99, Other::KOALA)
    Other.const_set("WOMBAT", "Hi")
    assert_equal("Hi", Other::WOMBAT)

    n = Object.new
    def n.to_str; @count = defined?(@count) ? @count + 1 : 1; "HOGE"; end
    def n.count; @count; end
    def n.count=(v); @count=v; end
    assert_not_operator(Other, :const_defined?, :HOGE)
    Other.const_set(n, 999)
    assert_equal(1, n.count)
    n.count = 0
    assert_equal(999, Other.const_get(n))
    assert_equal(1, n.count)
    n.count = 0
    assert_equal(true, Other.const_defined?(n))
    assert_equal(1, n.count)
  end

  def test_constants
    assert_equal([:MIXIN], Mixin.constants)
    assert_equal([:MIXIN, :USER], User.constants.sort)
  end

  def test_initialize_copy
    mod = Module.new { define_method(:foo) {:first} }
    klass = Class.new { include mod }
    instance = klass.new
    assert_equal(:first, instance.foo)
    new_mod = Module.new { define_method(:foo) { :second } }
    assert_raise(TypeError) do
      mod.send(:initialize_copy, new_mod)
    end
    4.times { GC.start }
    assert_equal(:first, instance.foo) # [BUG] unreachable
  end

  def test_initialize_copy_empty
    m = Module.new do
      def x
      end
      const_set(:X, 1)
      @x = 2
    end
    assert_equal([:x], m.instance_methods)
    assert_equal([:@x], m.instance_variables)
    assert_equal([:X], m.constants)
    assert_raise(TypeError) do
      m.module_eval do
        initialize_copy(Module.new)
      end
    end

    m = Class.new(Module) do
      def initialize_copy(other)
        # leave uninitialized
      end
    end.new.dup
    c = Class.new
    assert_operator(c.include(m), :<, m)
    cp = Module.instance_method(:initialize_copy)
    assert_raise(TypeError) do
      cp.bind_call(m, Module.new)
    end
  end

  class Bug18185 < Module
    module InstanceMethods
    end
    attr_reader :ancestor_list
    def initialize
      @ancestor_list = ancestors
      include InstanceMethods
    end
    class Foo
      attr_reader :key
      def initialize(key:)
        @key = key
      end
    end
  end

  def test_module_subclass_initialize
    mod = Bug18185.new
    c = Class.new(Bug18185::Foo) do
      include mod
    end
    anc = c.ancestors
    assert_include(anc, mod)
    assert_equal(1, anc.count(BasicObject), ->{anc.inspect})
    b = c.new(key: 1)
    assert_equal(1, b.key)
    assert_not_include(mod.ancestor_list, BasicObject)
  end

  def test_module_collected_extended_object
    m1 = labeled_module("m1")
    m2 = labeled_module("m2")
    Object.new.extend(m1)
    GC.start
    m1.include(m2)
    assert_equal([m1, m2], m1.ancestors)
  end

  def test_dup
    OtherSetup.call

    bug6454 = '[ruby-core:45132]'

    a = Module.new
    Other.const_set :BUG6454, a
    b = a.dup
    Other.const_set :BUG6454_dup, b

    assert_equal "TestModule::Other::BUG6454_dup", b.inspect, bug6454
  end

  def test_dup_anonymous
    bug6454 = '[ruby-core:45132]'

    a = Module.new
    original = a.inspect

    b = a.dup

    assert_not_equal original, b.inspect, bug6454
  end

  def test_public_include
    assert_nothing_raised('#8846') do
      Module.new.include(Module.new { def foo; end }).instance_methods == [:foo]
    end
  end

  def test_include_toplevel
    assert_separately([], <<-EOS)
      Mod = Module.new {def foo; :include_foo end}
      TOPLEVEL_BINDING.eval('include Mod')

      assert_equal(:include_foo, TOPLEVEL_BINDING.eval('foo'))
      assert_equal([Object, Mod], Object.ancestors.slice(0, 2))
    EOS
  end

  def test_include_with_no_args
    assert_raise(ArgumentError) { Module.new { include } }
  end

  def test_include_before_initialize
    m = Class.new(Module) do
      def initialize(...)
        include Enumerable
        super
      end
    end.new
    assert_equal(true, m < Enumerable)
  end

  def test_prepend_self
    m = Module.new
    assert_equal([m], m.ancestors)
    m.prepend(m) rescue nil
    assert_equal([m], m.ancestors)
  end

  def test_bug17590
    m = Module.new
    c = Class.new
    c.prepend(m)
    c.include(m)
    m.prepend(m) rescue nil
    m2 = Module.new
    m2.prepend(m)
    c.include(m2)

    assert_equal([m, c, m2] + Object.ancestors, c.ancestors)
  end

  def test_prepend_works_with_duped_classes
    m = Module.new
    a = Class.new do
      def b; 2 end
      prepend m
    end
    a2 = a.dup.new
    a.class_eval do
      alias _b b
      def b; 1 end
    end
    assert_equal(2, a2.b)
  end

  def test_gc_prepend_chain
    assert_separately([], <<-EOS)
      10000.times { |i|
        m1 = Module.new do
          def foo; end
        end
        m2 = Module.new do
          prepend m1
          def bar; end
        end
        m3 = Module.new do
          def baz; end
          prepend m2
        end
        Class.new do
          prepend m3
        end
      }
      GC.start
    EOS
  end

  def test_refine_module_then_include
    assert_separately([], "#{<<~"end;"}\n")
      module M
      end
      class C
        include M
      end
      module RefinementBug
        refine M do
          def refined_method
            :rm
          end
        end
      end
      using RefinementBug

      class A
        include M
      end

      assert_equal(:rm, C.new.refined_method)
    end;
  end

  def test_include_into_module_already_included
    c = Class.new{def foo; [:c] end}
    modules = lambda do ||
      sub = Class.new(c){def foo; [:sc] + super end}
      [
        Module.new{def foo; [:m1] + super end},
        Module.new{def foo; [:m2] + super end},
        Module.new{def foo; [:m3] + super end},
        sub,
        sub.new
      ]
    end

    m1, m2, m3, sc, o = modules.call
    assert_equal([:sc, :c], o.foo)
    sc.include m1
    assert_equal([:sc, :m1, :c], o.foo)
    m1.include m2
    assert_equal([:sc, :m1, :m2, :c], o.foo)
    m2.include m3
    assert_equal([:sc, :m1, :m2, :m3, :c], o.foo)

    m1, m2, m3, sc, o = modules.call
    sc.prepend m1
    assert_equal([:m1, :sc, :c], o.foo)
    m1.include m2
    assert_equal([:m1, :m2, :sc, :c], o.foo)
    m2.include m3
    assert_equal([:m1, :m2, :m3, :sc, :c], o.foo)

    m1, m2, m3, sc, o = modules.call
    sc.include m2
    assert_equal([:sc, :m2, :c], o.foo)
    sc.prepend m1
    assert_equal([:m1, :sc, :m2, :c], o.foo)
    m1.include m2
    assert_equal([:m1, :sc, :m2, :c], o.foo)
    m1.include m3
    assert_equal([:m1, :m3, :sc, :m2, :c], o.foo)

    m1, m2, m3, sc, o = modules.call
    sc.include m3
    sc.include m2
    assert_equal([:sc, :m2, :m3, :c], o.foo)
    sc.prepend m1
    assert_equal([:m1, :sc, :m2, :m3, :c], o.foo)
    m1.include m2
    m1.include m3
    assert_equal([:m1, :sc, :m2, :m3, :c], o.foo)

    m1, m2, m3, sc, o = modules.call
    assert_equal([:sc, :c], o.foo)
    sc.prepend m1
    assert_equal([:m1, :sc, :c], o.foo)
    m1.prepend m2
    assert_equal([:m2, :m1, :sc, :c], o.foo)
    m2.prepend m3
    assert_equal([:m3, :m2, :m1, :sc, :c], o.foo)
    m1, m2, m3, sc, o = modules.call
    sc.include m1
    assert_equal([:sc, :m1, :c], o.foo)
    sc.prepend m2
    assert_equal([:m2, :sc, :m1, :c], o.foo)
    sc.prepend m3
    assert_equal([:m3, :m2, :sc, :m1, :c], o.foo)
    m1, m2, m3, sc, o = modules.call
    sc.include m1
    assert_equal([:sc, :m1, :c], o.foo)
    m2.prepend m3
    m1.include m2
    assert_equal([:sc, :m1, :m3, :m2, :c], o.foo)
  end

  def test_included_modules
    assert_equal([], Mixin.included_modules)
    assert_equal([Mixin], User.included_modules)

    mixins = Object.included_modules - [Kernel]
    mixins << JSON::Ext::Generator::GeneratorMethods::String if defined?(JSON::Ext::Generator::GeneratorMethods::String)
    assert_equal([Kernel], Object.included_modules - mixins)
    assert_equal([Comparable, Kernel], String.included_modules - mixins)
  end

  def test_included_modules_with_prepend
    m1 = Module.new
    m2 = Module.new
    m3 = Module.new

    m2.prepend m1
    m3.include m2
    assert_equal([m1, m2], m3.included_modules)
  end

  def test_include_with_prepend
    c = Class.new{def m; [:c] end}
    p = Module.new{def m; [:p] + super end}
    q = Module.new{def m; [:q] + super end; include p}
    r = Module.new{def m; [:r] + super end; prepend q}
    s = Module.new{def m; [:s] + super end; include r}
    a = Class.new(c){def m; [:a] + super end; prepend p; include s}
    assert_equal([:p, :a, :s, :q, :r, :c], a.new.m)
  end

  def test_prepend_after_include
    c = Class.new{def m; [:c] end}
    sc = Class.new(c){def m; [:sc] + super end}
    m = Module.new{def m; [:m] + super end}
    sc.include m
    sc.prepend m
    sc.prepend m
    assert_equal([:m, :sc, :m, :c], sc.new.m)

    c = Class.new{def m; [:c] end}
    sc = Class.new(c){def m; [:sc] + super end}
    m0 = Module.new{def m; [:m0] + super end}
    m1 = Module.new{def m; [:m1] + super end}
    m1.prepend m0
    sc.include m1
    sc.prepend m1
    assert_equal([:m0, :m1, :sc, :m0, :m1, :c], sc.new.m)
    sc.prepend m
    assert_equal([:m, :m0, :m1, :sc, :m0, :m1, :c], sc.new.m)
    sc.prepend m1
    assert_equal([:m, :m0, :m1, :sc, :m0, :m1, :c], sc.new.m)


    c = Class.new{def m; [:c] end}
    sc = Class.new(c){def m; [:sc] + super end}
    m0 = Module.new{def m; [:m0] + super end}
    m1 = Module.new{def m; [:m1] + super end}
    m1.include m0
    sc.include m1
    sc.prepend m
    sc.prepend m1
    sc.prepend m1
    assert_equal([:m1, :m0, :m, :sc, :m1, :m0, :c], sc.new.m)
  end

  def test_instance_methods
    assert_equal([:user, :user2], User.instance_methods(false).sort)
    assert_equal([:user, :user2, :mixin].sort, User.instance_methods(true).sort)
    assert_equal([:mixin], Mixin.instance_methods)
    assert_equal([:mixin], Mixin.instance_methods(true))
    assert_equal([:cClass], (class << CClass; self; end).instance_methods(false))
    assert_equal([], (class << BClass; self; end).instance_methods(false))
    assert_equal([:cm2], (class << AClass; self; end).instance_methods(false))
    assert_equal([:aClass, :aClass2], AClass.instance_methods(false).sort)
    assert_equal([:aClass, :aClass2],
        (AClass.instance_methods(true) - Object.instance_methods(true)).sort)
  end

  def test_method_defined?
    [User, Class.new{include User}, Class.new{prepend User}].each do |klass|
      [[], [true]].each do |args|
        assert !klass.method_defined?(:wombat, *args)
        assert klass.method_defined?(:mixin, *args)
        assert klass.method_defined?(:user, *args)
        assert klass.method_defined?(:user2, *args)
        assert !klass.method_defined?(:user3, *args)

        assert !klass.method_defined?("wombat", *args)
        assert klass.method_defined?("mixin", *args)
        assert klass.method_defined?("user", *args)
        assert klass.method_defined?("user2", *args)
        assert !klass.method_defined?("user3", *args)
      end
    end
  end

  def test_method_defined_without_include_super
    assert User.method_defined?(:user, false)
    assert !User.method_defined?(:mixin, false)
    assert Mixin.method_defined?(:mixin, false)

    User.const_set(:FOO, c = Class.new)

    c.prepend(User)
    assert !c.method_defined?(:user, false)
    c.define_method(:user){}
    assert c.method_defined?(:user, false)

    assert !c.method_defined?(:mixin, false)
    c.define_method(:mixin){}
    assert c.method_defined?(:mixin, false)

    assert !c.method_defined?(:userx, false)
    c.define_method(:userx){}
    assert c.method_defined?(:userx, false)

    # cleanup
    User.class_eval do
      remove_const :FOO
    end
  end

  def module_exec_aux
    Proc.new do
      def dynamically_added_method_3; end
    end
  end
  def module_exec_aux_2(&block)
    User.module_exec(&block)
  end

  def test_module_exec
    User.module_exec do
      def dynamically_added_method_1; end
    end
    assert_method_defined?(User, :dynamically_added_method_1)

    block = Proc.new do
      def dynamically_added_method_2; end
    end
    User.module_exec(&block)
    assert_method_defined?(User, :dynamically_added_method_2)

    User.module_exec(&module_exec_aux)
    assert_method_defined?(User, :dynamically_added_method_3)

    module_exec_aux_2 do
      def dynamically_added_method_4; end
    end
    assert_method_defined?(User, :dynamically_added_method_4)

    # cleanup
    User.class_eval do
      remove_method :dynamically_added_method_1
      remove_method :dynamically_added_method_2
      remove_method :dynamically_added_method_3
      remove_method :dynamically_added_method_4
    end
  end

  def test_module_eval
    User.module_eval("MODULE_EVAL = 1")
    assert_equal(1, User::MODULE_EVAL)
    assert_include(User.constants, :MODULE_EVAL)
    User.instance_eval("remove_const(:MODULE_EVAL)")
    assert_not_include(User.constants, :MODULE_EVAL)
  end

  def test_name
    assert_equal("Integer", Integer.name)
    assert_equal("TestModule::Mixin",  Mixin.name)
    assert_equal("TestModule::User",   User.name)

    assert_predicate Integer.name, :frozen?
    assert_predicate Mixin.name, :frozen?
    assert_predicate User.name, :frozen?
  end

  def test_accidental_singleton_naming_with_module
    o = Object.new
    assert_nil(o.singleton_class.name)
    class << o
      module Hi; end
    end
    assert_nil(o.singleton_class.name)
  end

  def test_accidental_singleton_naming_with_class
    o = Object.new
    assert_nil(o.singleton_class.name)
    class << o
      class Hi; end
    end
    assert_nil(o.singleton_class.name)
  end

  def test_classpath
    m = Module.new
    n = Module.new
    m.const_set(:N, n)
    assert_nil(m.name)
    assert_match(/::N$/, n.name)
    assert_equal([:N], m.constants)
    m.module_eval("module O end")
    assert_equal([:N, :O], m.constants.sort)
    m.module_eval("class C; end")
    assert_equal([:C, :N, :O], m.constants.sort)
    assert_match(/::N$/, m::N.name)
    assert_match(/\A#<Module:.*>::O\z/, m::O.name)
    assert_match(/\A#<Module:.*>::C\z/, m::C.name)
    self.class.const_set(:M, m)
    prefix = self.class.name + "::M::"
    assert_equal(prefix+"N", m.const_get(:N).name)
    assert_equal(prefix+"O", m.const_get(:O).name)
    assert_equal(prefix+"C", m.const_get(:C).name)
    c = m.class_eval("Bug15891 = Class.new.freeze")
    assert_equal(prefix+"Bug15891", c.name)
  ensure
    self.class.class_eval {remove_const(:M)}
  end

  def test_private_class_method
    assert_raise(ExpectedException) { AClass.cm1 }
    assert_raise(ExpectedException) { AClass.cm3 }
    assert_equal("cm1cm2cm3", AClass.cm2)

    c = Class.new(AClass)
    c.class_eval {private_class_method [:cm1, :cm2]}
    assert_raise(NoMethodError, /private method/) {c.cm1}
    assert_raise(NoMethodError, /private method/) {c.cm2}
  end

  def test_private_instance_methods
    assert_equal([:aClass1], AClass.private_instance_methods(false))
    assert_equal([:bClass2], BClass.private_instance_methods(false))
    assert_equal([:aClass1, :bClass2],
        (BClass.private_instance_methods(true) -
         Object.private_instance_methods(true)).sort)
  end

  def test_protected_instance_methods
    assert_equal([:aClass2], AClass.protected_instance_methods)
    assert_equal([:bClass3], BClass.protected_instance_methods(false))
    assert_equal([:bClass3, :aClass2].sort,
                 (BClass.protected_instance_methods(true) -
                  Object.protected_instance_methods(true)).sort)
  end

  def test_public_class_method
    assert_equal("cm1",       MyClass.cm1)
    assert_equal("cm1cm2cm3", MyClass.cm2)
    assert_raise(ExpectedException) { eval "MyClass.cm3" }

    c = Class.new(AClass)
    c.class_eval {public_class_method [:cm1, :cm2]}
    assert_equal("cm1",       c.cm1)
    assert_equal("cm1cm2cm3", c.cm2)
  end

  def test_public_instance_methods
    assert_equal([:aClass],  AClass.public_instance_methods(false))
    assert_equal([:bClass1], BClass.public_instance_methods(false))
  end

  def test_s_public
    o = (c = Class.new(AClass)).new
    assert_raise(NoMethodError, /private method/) {o.aClass1}
    assert_raise(NoMethodError, /protected method/) {o.aClass2}
    c.class_eval {public :aClass1}
    assert_equal(:aClass1, o.aClass1)

    o = (c = Class.new(AClass)).new
    c.class_eval {public :aClass1, :aClass2}
    assert_equal(:aClass1, o.aClass1)
    assert_equal(:aClass2, o.aClass2)

    o = (c = Class.new(AClass)).new
    c.class_eval {public [:aClass1, :aClass2]}
    assert_equal(:aClass1, o.aClass1)
    assert_equal(:aClass2, o.aClass2)

    o = AClass.new
    assert_equal(:aClass, o.aClass)
    assert_raise(NoMethodError, /private method/) {o.aClass1}
    assert_raise(NoMethodError, /protected method/) {o.aClass2}
  end

  def test_s_private
    o = (c = Class.new(AClass)).new
    assert_equal(:aClass, o.aClass)
    c.class_eval {private :aClass}
    assert_raise(NoMethodError, /private method/) {o.aClass}

    o = (c = Class.new(AClass)).new
    c.class_eval {private :aClass, :aClass2}
    assert_raise(NoMethodError, /private method/) {o.aClass}
    assert_raise(NoMethodError, /private method/) {o.aClass2}

    o = (c = Class.new(AClass)).new
    c.class_eval {private [:aClass, :aClass2]}
    assert_raise(NoMethodError, /private method/) {o.aClass}
    assert_raise(NoMethodError, /private method/) {o.aClass2}

    o = AClass.new
    assert_equal(:aClass, o.aClass)
    assert_raise(NoMethodError, /private method/) {o.aClass1}
    assert_raise(NoMethodError, /protected method/) {o.aClass2}
  end

  def test_s_protected
    aclass = Class.new(AClass) do
      def _aClass(o) o.aClass; end
      def _aClass1(o) o.aClass1; end
      def _aClass2(o) o.aClass2; end
    end

    o = (c = Class.new(aclass)).new
    assert_equal(:aClass, o.aClass)
    c.class_eval {protected :aClass}
    assert_raise(NoMethodError, /protected method/) {o.aClass}
    assert_equal(:aClass, c.new._aClass(o))

    o = (c = Class.new(aclass)).new
    c.class_eval {protected :aClass, :aClass1}
    assert_raise(NoMethodError, /protected method/) {o.aClass}
    assert_raise(NoMethodError, /protected method/) {o.aClass1}
    assert_equal(:aClass, c.new._aClass(o))
    assert_equal(:aClass1, c.new._aClass1(o))

    o = (c = Class.new(aclass)).new
    c.class_eval {protected [:aClass, :aClass1]}
    assert_raise(NoMethodError, /protected method/) {o.aClass}
    assert_raise(NoMethodError, /protected method/) {o.aClass1}
    assert_equal(:aClass, c.new._aClass(o))
    assert_equal(:aClass1, c.new._aClass1(o))

    o = AClass.new
    assert_equal(:aClass, o.aClass)
    assert_raise(NoMethodError, /private method/) {o.aClass1}
    assert_raise(NoMethodError, /protected method/) {o.aClass2}
  end

  def test_visibility_method_return_value
    no_arg_results = nil
    c = Module.new do
      singleton_class.send(:public, :public, :private, :protected, :module_function)
      def foo; end
      def bar; end
      no_arg_results = [public, private, protected, module_function]
    end

    assert_equal([nil]*4, no_arg_results)

    assert_equal(:foo, c.private(:foo))
    assert_equal(:foo, c.public(:foo))
    assert_equal(:foo, c.protected(:foo))
    assert_equal(:foo, c.module_function(:foo))

    assert_equal([:foo, :bar], c.private(:foo, :bar))
    assert_equal([:foo, :bar], c.public(:foo, :bar))
    assert_equal([:foo, :bar], c.protected(:foo, :bar))
    assert_equal([:foo, :bar], c.module_function(:foo, :bar))
  end

  def test_s_constants
    c1 = Module.constants
    Object.module_eval "WALTER = 99"
    c2 = Module.constants
    assert_equal([:WALTER], c2 - c1)

    Object.class_eval do
      remove_const :WALTER
    end

    assert_equal([], Module.constants(true))
    assert_equal([], Module.constants(false))

    src = <<-INPUT
      ary = Module.constants
      module M
        WALTER = 99
      end
      class Module
        include M
      end
      p Module.constants - ary, Module.constants(true), Module.constants(false)
    INPUT
    assert_in_out_err([], src, %w([:M] [:WALTER] []), [])

    klass = Class.new do
      const_set(:X, 123)
    end
    assert_equal(false, klass.class_eval { Module.constants }.include?(:X))

    assert_equal(false, Complex.constants(false).include?(:compatible))
  end

  module M1
    $m1 = Module.nesting
    module M2
      $m2 = Module.nesting
    end
  end

  def test_s_nesting
    assert_equal([],                               $m0)
    assert_equal([TestModule::M1, TestModule],     $m1)
    assert_equal([TestModule::M1::M2,
                  TestModule::M1, TestModule],     $m2)
  end

  def test_s_new
    m = Module.new
    assert_instance_of(Module, m)
  end

  def test_freeze
    m = Module.new do
      def self.baz; end
      def bar; end
    end
    m.freeze
    assert_raise(FrozenError) do
      m.module_eval do
        def foo; end
      end
    end
    assert_raise(FrozenError) do
      m.__send__ :private, :bar
    end
    assert_raise(FrozenError) do
      m.private_class_method :baz
    end
  end

  def test_attr_obsoleted_flag
    c = Class.new do
      extend Test::Unit::Assertions
      extend Test::Unit::CoreAssertions
      def initialize
        @foo = :foo
        @bar = :bar
      end
      assert_deprecated_warning(/optional boolean argument/) do
        attr :foo, true
      end
      assert_deprecated_warning(/optional boolean argument/) do
        attr :bar, false
      end
    end
    o = c.new
    assert_equal(true, o.respond_to?(:foo))
    assert_equal(true, o.respond_to?(:foo=))
    assert_equal(true, o.respond_to?(:bar))
    assert_equal(false, o.respond_to?(:bar=))
  end

  def test_attr_public_at_toplevel
    s = Object.new
    TOPLEVEL_BINDING.eval(<<-END).call(s.singleton_class)
      proc do |c|
        c.send(:attr_accessor, :x)
        c.send(:attr, :y)
        c.send(:attr_reader, :z)
        c.send(:attr_writer, :w)
      end
    END
    assert_nil s.x
    s.x = 1
    assert_equal 1, s.x

    assert_nil s.y
    s.instance_variable_set(:@y, 2)
    assert_equal 2, s.y

    assert_nil s.z
    s.instance_variable_set(:@z, 3)
    assert_equal 3, s.z

    s.w = 4
    assert_equal 4, s.instance_variable_get(:@w)
  end

  def test_const_get_evaled
    c1 = Class.new
    c2 = Class.new(c1)

    eval("c1::Foo = :foo")
    assert_equal(:foo, c1::Foo)
    assert_equal(:foo, c2::Foo)
    assert_equal(:foo, c2.const_get(:Foo))
    assert_raise(NameError) { c2.const_get(:Foo, false) }

    c1.__send__(:remove_const, :Foo)
    eval("c1::Foo = :foo")
    assert_raise(NameError) { c1::Bar }
    assert_raise(NameError) { c2::Bar }
    assert_raise(NameError) { c2.const_get(:Bar) }
    assert_raise(NameError) { c2.const_get(:Bar, false) }
    assert_raise(NameError) { c2.const_get("Bar", false) }
    assert_raise(NameError) { c2.const_get("BaR11", false) }
    assert_raise(NameError) { Object.const_get("BaR11", false) }

    c1.instance_eval do
      def const_missing(x)
        x
      end
    end

    assert_equal(:Bar, c1::Bar)
    assert_equal(:Bar, c2::Bar)
    assert_equal(:Bar, c2.const_get(:Bar))
    assert_equal(:Bar, c2.const_get(:Bar, false))
    assert_equal(:Bar, c2.const_get("Bar"))
    assert_equal(:Bar, c2.const_get("Bar", false))

    v = c2.const_get("Bar11", false)
    assert_equal("Bar11".to_sym, v)

    assert_raise(NameError) { c1.const_get(:foo) }
  end

  def test_const_set_invalid_name
    c1 = Class.new
    assert_raise_with_message(NameError, /foo/) { c1.const_set(:foo, :foo) }
    assert_raise_with_message(NameError, /bar/) { c1.const_set("bar", :foo) }
    assert_raise_with_message(TypeError, /1/) { c1.const_set(1, :foo) }
    assert_nothing_raised(NameError) { c1.const_set("X\u{3042}", :foo) }
    assert_raise(NameError) { c1.const_set("X\u{3042}".encode("utf-16be"), :foo) }
    assert_raise(NameError) { c1.const_set("X\u{3042}".encode("utf-16le"), :foo) }
    assert_raise(NameError) { c1.const_set("X\u{3042}".encode("utf-32be"), :foo) }
    assert_raise(NameError) { c1.const_set("X\u{3042}".encode("utf-32le"), :foo) }
    cx = EnvUtil.labeled_class("X\u{3042}")
    assert_raise_with_message(TypeError, /X\u{3042}/) { c1.const_set(cx, :foo) }
  end

  def test_const_get_invalid_name
    c1 = Class.new
    assert_raise(NameError) { c1.const_get(:foo) }
    bug5084 = '[ruby-dev:44200]'
    assert_raise(TypeError, bug5084) { c1.const_get(1) }
  end

  def test_const_defined_invalid_name
    c1 = Class.new
    assert_raise(NameError) { c1.const_defined?(:foo) }
    bug5084 = '[ruby-dev:44200]'
    assert_raise(TypeError, bug5084) { c1.const_defined?(1) }
  end

  def test_const_get_no_inherited
    bug3422 = '[ruby-core:30719]'
    assert_in_out_err([], <<-INPUT, %w[1 NameError A], [], bug3422)
    BasicObject::A = 1
    puts [true, false].map {|inh|
      begin
        Object.const_get(:A, inh)
      rescue NameError => e
        [e.class, e.name]
      end
    }
    INPUT
  end

  def test_const_get_inherited
    bug3423 = '[ruby-core:30720]'
    assert_in_out_err([], <<-INPUT, %w[NameError A NameError A], [], bug3423)
    module Foo; A = 1; end
    class Object; include Foo; end
    class Bar; include Foo; end

    puts [Object, Bar].map {|klass|
      begin
        klass.const_get(:A, false)
      rescue NameError => e
        [e.class, e.name]
      end
    }
    INPUT
  end

  def test_const_in_module
    bug3423 = '[ruby-core:37698]'
    assert_in_out_err([], <<-INPUT, %w[ok], [], bug3423)
    module LangModuleSpecInObject
      module LangModuleTop
      end
    end
    include LangModuleSpecInObject
    module LangModuleTop
    end
    puts "ok" if LangModuleSpecInObject::LangModuleTop == LangModuleTop
    INPUT

    bug5264 = '[ruby-core:39227]'
    assert_in_out_err([], <<-'INPUT', [], [], bug5264)
    class A
      class X; end
    end
    class B < A
      module X; end
    end
    INPUT
  end

  def test_class_variable_get
    c = Class.new
    c.class_eval('@@foo = :foo')
    assert_equal(:foo, c.class_variable_get(:@@foo))
    assert_raise(NameError) { c.class_variable_get(:@@bar) } # c.f. instance_variable_get
    assert_raise(NameError) { c.class_variable_get(:'@@') }
    assert_raise(NameError) { c.class_variable_get('@@') }
    assert_raise(NameError) { c.class_variable_get(:foo) }
    assert_raise(NameError) { c.class_variable_get("bar") }
    assert_raise(TypeError) { c.class_variable_get(1) }

    n = Object.new
    def n.to_str; @count = defined?(@count) ? @count + 1 : 1; "@@foo"; end
    def n.count; @count; end
    assert_equal(:foo, c.class_variable_get(n))
    assert_equal(1, n.count)
  end

  def test_class_variable_set
    c = Class.new
    c.class_variable_set(:@@foo, :foo)
    assert_equal(:foo, c.class_eval('@@foo'))
    assert_raise(NameError) { c.class_variable_set(:'@@', 1) }
    assert_raise(NameError) { c.class_variable_set('@@', 1) }
    assert_raise(NameError) { c.class_variable_set(:foo, 1) }
    assert_raise(NameError) { c.class_variable_set("bar", 1) }
    assert_raise(TypeError) { c.class_variable_set(1, 1) }

    n = Object.new
    def n.to_str; @count = defined?(@count) ? @count + 1 : 1; "@@foo"; end
    def n.count; @count; end
    c.class_variable_set(n, :bar)
    assert_equal(:bar, c.class_eval('@@foo'))
    assert_equal(1, n.count)
  end

  def test_class_variable_defined
    c = Class.new
    c.class_eval('@@foo = :foo')
    assert_equal(true, c.class_variable_defined?(:@@foo))
    assert_equal(false, c.class_variable_defined?(:@@bar))
    assert_raise(NameError) { c.class_variable_defined?(:'@@') }
    assert_raise(NameError) { c.class_variable_defined?('@@') }
    assert_raise(NameError) { c.class_variable_defined?(:foo) }
    assert_raise(NameError) { c.class_variable_defined?("bar") }
    assert_raise(TypeError) { c.class_variable_defined?(1) }
    n = Object.new
    def n.to_str; @count = defined?(@count) ? @count + 1 : 1; "@@foo"; end
    def n.count; @count; end
    assert_equal(true, c.class_variable_defined?(n))
    assert_equal(1, n.count)
  end

  def test_remove_class_variable
    c = Class.new
    c.class_eval('@@foo = :foo')
    c.class_eval { remove_class_variable(:@@foo) }
    assert_equal(false, c.class_variable_defined?(:@@foo))
    assert_raise(NameError) do
      c.class_eval { remove_class_variable(:@var) }
    end
  end

  def test_export_method
    m = Module.new
    assert_raise(NameError) do
      m.instance_eval { public(:foo) }
    end
  end

  def test_attr
    assert_in_out_err([], <<-INPUT, %w(nil))
      $VERBOSE = true
      c = Class.new
      c.instance_eval do
        private
        attr_reader :foo
      end
      o = c.new
      p(o.instance_eval { foo })
    INPUT

    c = Class.new
    assert_raise(NameError) do
      c.instance_eval { attr_reader :"." }
    end

    assert_equal([:a], c.class_eval { attr :a })
    assert_equal([:b, :c], c.class_eval { attr :b, :c })
    assert_equal([:d], c.class_eval { attr_reader :d })
    assert_equal([:e, :f], c.class_eval { attr_reader :e, :f })
    assert_equal([:g=], c.class_eval { attr_writer :g })
    assert_equal([:h=, :i=], c.class_eval { attr_writer :h, :i })
    assert_equal([:j, :j=], c.class_eval { attr_accessor :j })
    assert_equal([:k, :k=, :l, :l=], c.class_eval { attr_accessor :k, :l })
  end

  def test_alias_method
    c = Class.new do
      def foo; :foo end
    end
    o = c.new
    assert_respond_to(o, :foo)
    assert_not_respond_to(o, :bar)
    r = c.class_eval {alias_method :bar, :foo}
    assert_respond_to(o, :bar)
    assert_equal(:foo, o.bar)
    assert_equal(:bar, r)
  end

  def test_undef
    c = Class.new
    assert_raise(NameError) do
      c.instance_eval { undef_method(:foo) }
    end

    m = Module.new
    assert_raise(NameError) do
      m.instance_eval { undef_method(:foo) }
    end

    o = Object.new
    assert_raise(NameError) do
      class << o; self; end.instance_eval { undef_method(:foo) }
    end

    %w(object_id __send__ initialize).each do |n|
      assert_in_out_err([], <<-INPUT, [], %r"warning: undefining `#{n}' may cause serious problems$")
        $VERBOSE = false
        Class.new.instance_eval { undef_method(:#{n}) }
      INPUT
    end
  end

  def test_alias
    m = Module.new
    assert_raise(NameError) do
      m.class_eval { alias foo bar }
    end

    assert_in_out_err([], <<-INPUT, %w(2), /discarding old foo$/)
      $VERBOSE = true
      c = Class.new
      c.class_eval do
        def foo; 1; end
        def bar; 2; end
      end
      c.class_eval { alias foo bar }
      p c.new.foo
    INPUT
  end

  def test_mod_constants
    m = Module.new
    m.const_set(:Foo, :foo)
    assert_equal([:Foo], m.constants(true))
    assert_equal([:Foo], m.constants(false))
    m.instance_eval { remove_const(:Foo) }
  end

  class Bug9413
    class << self
      Foo = :foo
    end
  end

  def test_singleton_constants
    bug9413 = '[ruby-core:59763] [Bug #9413]'
    c = Bug9413.singleton_class
    assert_include(c.constants(true), :Foo, bug9413)
    assert_include(c.constants(false), :Foo, bug9413)
  end

  def test_frozen_module
    m = Module.new
    m.freeze
    assert_raise(FrozenError) do
      m.instance_eval { undef_method(:foo) }
    end
  end

  def test_frozen_class
    c = Class.new
    c.freeze
    assert_raise(FrozenError) do
      c.instance_eval { undef_method(:foo) }
    end
  end

  def test_frozen_singleton_class
    klass = Class.new
    o = klass.new
    c = class << o; self; end
    c.freeze
    assert_raise_with_message(FrozenError, /frozen/) do
      c.instance_eval { undef_method(:foo) }
    end
    klass.class_eval do
      def self.foo
      end
    end
  end

  def test_method_defined
    cl = Class.new
    def_methods = proc do
      def foo; end
      def bar; end
      def baz; end
      public :foo
      protected :bar
      private :baz
    end
    cl.class_eval(&def_methods)
    sc = Class.new(cl)
    mod = Module.new(&def_methods)
    only_prepend = Class.new{prepend(mod)}
    empty_prepend = cl.clone
    empty_prepend.prepend(Module.new)
    overlap_prepend = cl.clone
    overlap_prepend.prepend(mod)

    [[], [true], [false]].each do |args|
      [cl, sc, only_prepend, empty_prepend, overlap_prepend].each do |c|
        always_false = [sc, only_prepend].include?(c) && args == [false]

        assert_equal(always_false ? false : true, c.public_method_defined?(:foo, *args))
        assert_equal(always_false ? false : false, c.public_method_defined?(:bar, *args))
        assert_equal(always_false ? false : false, c.public_method_defined?(:baz, *args))

        # Test if string arguments are converted to symbols
        assert_equal(always_false ? false : true, c.public_method_defined?("foo", *args))
        assert_equal(always_false ? false : false, c.public_method_defined?("bar", *args))
        assert_equal(always_false ? false : false, c.public_method_defined?("baz", *args))

        assert_equal(always_false ? false : false, c.protected_method_defined?(:foo, *args))
        assert_equal(always_false ? false : true, c.protected_method_defined?(:bar, *args))
        assert_equal(always_false ? false : false, c.protected_method_defined?(:baz, *args))

        # Test if string arguments are converted to symbols
        assert_equal(always_false ? false : false, c.protected_method_defined?("foo", *args))
        assert_equal(always_false ? false : true, c.protected_method_defined?("bar", *args))
        assert_equal(always_false ? false : false, c.protected_method_defined?("baz", *args))

        assert_equal(always_false ? false : false, c.private_method_defined?(:foo, *args))
        assert_equal(always_false ? false : false, c.private_method_defined?(:bar, *args))
        assert_equal(always_false ? false : true, c.private_method_defined?(:baz, *args))

        # Test if string arguments are converted to symbols
        assert_equal(always_false ? false : false, c.private_method_defined?("foo", *args))
        assert_equal(always_false ? false : false, c.private_method_defined?("bar", *args))
        assert_equal(always_false ? false : true, c.private_method_defined?("baz", *args))
      end
    end
  end

  def test_top_public_private
    assert_in_out_err([], <<-INPUT, %w([:foo] [:bar] [:bar,\ :foo] [] [:bar,\ :foo] []), [])
      private
      def foo; :foo; end
      public
      def bar; :bar; end
      p self.private_methods.grep(/^foo$|^bar$/)
      p self.methods.grep(/^foo$|^bar$/)

      private :foo, :bar
      p self.private_methods.grep(/^foo$|^bar$/).sort

      public :foo, :bar
      p self.private_methods.grep(/^foo$|^bar$/).sort

      private [:foo, :bar]
      p self.private_methods.grep(/^foo$|^bar$/).sort

      public [:foo, :bar]