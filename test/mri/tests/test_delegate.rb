# frozen_string_literal: true
require 'test/unit'
require 'delegate'

class TestDelegateClass < Test::Unit::TestCase
  module PP
    def mu_pp(obj)
      str = super
      str = "#<#{obj.class}: #{str}>" if Delegator === obj
      str
    end
  end

  module M
    attr_reader :m
  end

  def test_extend
    obj = DelegateClass(Array).new([])
    obj.instance_eval { @m = :m }
    obj.extend M
    assert_equal(:m, obj.m, "[ruby-dev:33116]")
  end

  def test_delegate_class_block
    klass = DelegateClass(Array) do
      alias foo first
    end
    assert_equal(1, klass.new([1]).foo)
  end

  def test_systemcallerror_eq
    e = SystemCallError.new(0)
    assert((SimpleDelegator.new(e) == e) == (e == SimpleDelegator.new(e)), "[ruby-dev:34808]")
  end

  class Myclass < DelegateClass(Array);end

  def test_delegateclass_class
    myclass=Myclass.new([])
    assert_equal(Myclass,myclass.class)
    assert_equal(Myclass,myclass.dup.class,'[ruby-dev:40313]')
    assert_equal(Myclass,myclass.clone.class,'[ruby-dev:40313]')
  end

  def test_simpledelegator_class
    simple=SimpleDelegator.new([])
    assert_equal(SimpleDelegator,simple.class)
    assert_equal(SimpleDelegator,simple.dup.class)
    assert_equal(SimpleDelegator,simple.clone.class)
  end

  def test_simpledelegator_clone
    simple=SimpleDelegator.new([])
    simple.freeze

    clone = simple.clone
    assert_predicate clone, :frozen?
    assert_predicate clone.__getobj__, :frozen?
    assert_equal true, Kernel.instance_method(:frozen?).bind(clone).call

    clone = simple.clone(freeze: false)
    assert_not_predicate clone, :frozen?
    assert_not_predicate clone.__getobj__, :frozen?
    assert_equal false, Kernel.instance_method(:frozen?).bind(clone).call
  end

  class Object
    def m
      :o
    end
    private
    def delegate_test_m
      :o
    end
  end

  class Foo
    def m
      :m
    end
    def delegate_test_m
      :m
    end
  end

  class Bar < DelegateClass(Foo)
  end

  def test_override
    foo = Foo.new
    foo2 = SimpleDelegator.new(foo)
    bar = Bar.new(foo)
    assert_equal(:o, Object.new.m)
    assert_equal(:m, foo.m)
    assert_equal(:m, foo2.m)
    assert_equal(:m, bar.m)
    bug = '[ruby-dev:39154]'
    assert_equal(:m, foo2.send(:delegate_test_m), bug)
    assert_equal(:m, bar.send(:delegate_test_m), bug)
  end

  class Parent
    def parent_public; end

    protected

    def parent_protected; end

    private

    def parent_private; end
  end

  class Child < DelegateClass(Parent)
  end

  class Parent
    def parent_public_added; end

    protected

    def parent_protected_added; end

    private

    def parent_private_added; end
  end

  def test_public_instance_methods
    ignores = Object.public_instance_methods | Delegator.public_instance_methods
    assert_equal([:parent_public, :parent_public_added], (Child.public_instance_methods - ignores).sort)
    assert_equal([:parent_public, :parent_public_added], (Child.new(Parent.new).public_methods - ignores).sort)
  end

  def test_protected_instance_methods
    ignores = Object.protected_instance_methods | Delegator.protected_instance_methods
    assert_equal([:parent_protected, :parent_protected_added], (Child.protected_instance_methods - ignores).sort)
    assert_equal([:parent_protected, :parent_protected_added], (Child.new(Parent.new).protected_methods - ignores).sort)
  end

  def test_instance_methods
    ignores = Object.instance_methods | Delegator.instance_methods
    assert_equal([:parent_protected, :parent_protected_added, :parent_public, :parent_public_added], (Child.instance_methods - ignores).sort)
    assert_equal([:parent_protected, :parent_protected_added, :parent_public, :parent_public_added], (Child.new(Parent.new).methods - ignores).sort)
  end

  def test_DelegateClass_instance_method
    assert_instance_of UnboundMethod, Child.instance_method(:parent_public)
    assert_instance_of UnboundMethod, Child.instance_method(:parent_public_added)
    assert_instance_of UnboundMethod, Child.instance_method(:parent_protected)
    assert_instance_of UnboundMethod, Child.instance_method(:parent_protected_added)
    assert_raise(NameError) { Child.instance_method(:parent_private) }
    assert_raise(NameError) { Child.instance_method(:parent_private_added) }
    assert_instance_of UnboundMethod, Child.instance_method(:to_s)
  end

  def test_DelegateClass_public_instance_method
    assert_instance_of UnboundMethod, Child.public_instance_method(:parent_public)
    assert_instance_of UnboundMethod, Child.public_instance_method(:parent_public_added)
    assert_raise(NameError) { Child.public_instance_method(:parent_protected) }
    assert_raise(NameError) { Child.public_instance_method(:parent_protected_added) }
    assert_raise(NameError) { Child.instance_method(:parent_private) }
    assert_raise(NameError) { Child.instance_method(:parent_private_added) }
    assert_instance_of UnboundMethod, Child.public_instance_method(:to_s)
  end

  class IV < DelegateClass(Integer)
    attr_accessor :var

    def initialize
      @var = 1
      super(0)
    end
  end

  def test_marshal
    bug1744 = '[ruby-core:24211]'
    c = IV.new
    assert_equal(1, c.var)
    d = Marshal.load(Marshal.dump(c))
    assert_equal(1, d.var, bug1744)
  end

  def test_copy_frozen
    bug2679 = '[ruby-dev:40242]'
    a = [42, :hello].freeze
    d = SimpleDelegator.new(a)
    assert_nothing_raised(bug2679) {d.dup[0] += 1}
    assert_raise(FrozenError) {d.clone[0] += 1}
    d.freeze
    assert(d.clone.frozen?)
    assert(!d.dup.frozen?)
  end

  def test_frozen
    d = SimpleDelegator.new([1, :foo])
    d.freeze
    assert_raise(FrozenError, '[ruby-dev:40314]#1') {d.__setobj__("foo")}
    assert_equal([1, :foo], d)
  end

  def test_instance_method
    s = SimpleDelegator.new("foo")
    m = s.method("upcase")
    s.__setobj__([1,2,3])
    assert_raise(NoMethodError, '[ruby-dev:40314]#3') {m.call}
  end

  def test_methods
    s = SimpleDelegator.new("foo")
    assert_equal([], s.methods(false))
    def s.bar; end
    assert_equal([:bar], s.methods(false))
  end

  def test_eql?
    extend PP
    s0 = SimpleDelegator.new("foo")
    s1 = SimpleDelegator.new("bar")
    s2 = SimpleDelegator.new("foo")
    assert_operator(s0, :eql?, s0)
    assert_operator(s0, :eql?, "foo")
    assert_operator(s0, :eql?, s2)
    assert_not_operator(s0, :eql?, s1)
    asser