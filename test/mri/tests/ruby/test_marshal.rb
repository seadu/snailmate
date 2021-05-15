# frozen_string_literal: false
require 'test/unit'
require 'tempfile'
require_relative 'marshaltestlib'

class TestMarshal < Test::Unit::TestCase
  include MarshalTestLib

  def setup
    @verbose = $VERBOSE
  end

  def teardown
    $VERBOSE = @verbose
  end

  def encode(o)
    Marshal.dump(o)
  end

  def decode(s)
    Marshal.load(s)
  end

  def fact(n)
    return 1 if n == 0
    f = 1
    while n>0
      f *= n
      n -= 1
    end
    return f
  end

  def test_marshal
    a = [1, 2, 3, [4,5,"foo"], {1=>"bar"}, 2.5, fact(30)]
    assert_equal a, Marshal.load(Marshal.dump(a))

    [[1,2,3,4], [81, 2, 118, 3146]].each { |w,x,y,z|
      obj = (x.to_f + y.to_f / z.to_f) * Math.exp(w.to_f / (x.to_f + y.to_f / z.to_f))
      assert_equal obj, Marshal.load(Marshal.dump(obj))
    }

    bug3659 = '[ruby-dev:41936]'
    [1.0, 10.0, 100.0, 110.0].each {|x|
      assert_equal(x, Marshal.load(Marshal.dump(x)), bug3659)
    }
  end

  StrClone = String.clone
  def test_marshal_cloned_class
    assert_instance_of(StrClone, Marshal.load(Marshal.dump(StrClone.new("abc"))))
  end

  def test_inconsistent_struct
    TestMarshal.const_set :StructOrNot, Struct.new(:a)
    s = Marshal.dump(StructOrNot.new(1))
    TestMarshal.instance_eval { remove_const :StructOrNot }
    TestMarshal.const_set :StructOrNot, Class.new
    assert_raise(TypeError, "[ruby-dev:31709]") { Marshal.load(s) }
  ensure
    TestMarshal.instance_eval { remove_const :StructOrNot }
  end

  def test_struct_invalid_members
    TestMarshal.const_set :StructInvalidMembers, Struct.new(:a)
    assert_raise(TypeError, "[ruby-dev:31759]") {
      Marshal.load("\004\bIc&TestMarshal::StructInvalidMembers\006:\020__members__\"\bfoo")
      TestMarshal::StructInvalidMembers.members
    }
  ensure
    TestMarshal.instance_eval { remove_const :StructInvalidMembers }
  end