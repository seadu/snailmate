require_relative '../../spec_helper'
require_relative 'fixtures/classes'

describe "Array.new" do
  it "returns an instance of Array" do
    Array.new.should be_an_instance_of(Array)
  end

  it "returns an instance of a subclass" do
    ArraySpecs::MyArray.new(1, 2).should be_an_instance_of(ArraySpecs::MyArray)
  end

  it "raises an ArgumentError if passed 3 or more arguments" do
    -> do
      [1, 2].send :initialize, 1, 'x', true
    end.should raise_error(ArgumentError)
    -> do
      [1, 2].send(:initialize, 1, 'x', true) {}
    end.should raise_error(ArgumentError)
  end
end

describe "Array.new with no arguments" do
  it "returns an empty array" do
    Array.new.should be_empty
  end

  it "does not use the given block" do
    -> {
      -> { Array.new { raise } }.should_not raise_error
    }.should complain(/warning: given block not used/, verbose: true)
  end
end

describe "Array.new with (array)" do
  it "returns an array initialized to the other array" do
    b = [4, 5, 6]
    Array.new(b).should == b
  end

  it "does not use the given block" do
    ->{ Array.new([1, 2]) { raise } }.should_not raise_error
  end

  it "calls #to_ary to convert the value to an array" do
    a = mock("array")
    a.should_receive(:to_ary).and_return([1, 2])
    a.should_not_receive(:to_int)
    Array.new(a).should == [1, 2]
  end

  it "does not call #to_ary on instances of Array or subclasses of Array" do
    a = [1, 2]