
require_relative '../../spec_helper'
require_relative 'fixtures/classes'
require_relative 'shared/push'

describe "Array#<<" do
  it "pushes the object onto the end of the array" do
    ([ 1, 2 ] << "c" << "d" << [ 3, 4 ]).should == [1, 2, "c", "d", [3, 4]]
  end

  it "returns self to allow chaining" do
    a = []
    b = a
    (a << 1).should equal(b)
    (a << 2 << 3).should equal(b)
  end

  it "correctly resizes the Array" do
    a = []
    a.size.should == 0
    a << :foo
    a.size.should == 1
    a << :bar << :baz
    a.size.should == 3

    a = [1, 2, 3]
    a.shift
    a.shift
    a.shift
    a << :foo
    a.should == [:foo]
  end

  it "raises a FrozenError on a frozen array" do
    -> { ArraySpecs.frozen_array << 5 }.should raise_error(FrozenError)
  end
end

describe "Array#append" do
  it_behaves_like :array_push, :append
end