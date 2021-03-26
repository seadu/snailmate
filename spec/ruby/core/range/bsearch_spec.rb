require_relative '../../spec_helper'
require_relative '../enumerable/shared/enumeratorized'

describe "Range#bsearch" do
  it "returns an Enumerator when not passed a block" do
    (0..1).bsearch.should be_an_instance_of(Enumerator)
  end

  it_behaves_like :enumeratorized_with_unknown_size, :bsearch, (1..3)

  it "raises a TypeError if the block returns an Object" do
    -> { (0..1).bsearch { Object.new } }.should raise_error(TypeError)
  end

  it "raises a TypeError if the block returns a String" do
    -> { (0..1).bsearch { "1" } }.should raise_error(TypeError)
  end

  it "raises a TypeError if the Range has Object values" do
    value = mock("range bsearch")
    r = Range.new value, value

    -> { r.bsearch { true } }.should raise_error(TypeError)
  end

  it "raises a TypeError if the Range has String values" do
    -> { ("a".."e").bsearch { true } }.should raise_error(TypeError)
  end

  context "with Integer values" do
    context "with a block returning true or false" do
      it "returns nil if the block returns false for every element" do
        (0...3).bsearch { |x| x > 3 }.should be_nil
      end

      it "returns nil if the block returns nil for every element" do
        (0..3).bsearch { |x| nil }.should be_nil
      end

      it "returns minimum element if the block returns true for every element" do
        (-2..4).bsearch { |x| x < 4 }.should == -2
      end

      it "returns the smallest element for which block returns true" do
        (0..4).bsearch { |x| x >= 2 }.should == 2
        (-1..4).bsearch { |x| x >= 1 }.should == 1
      end

      it "returns the last element if the block returns true for the last element" do
        (0..4).bsearch { |x| x >= 4 }.should == 4
        (0...4).bsearch { |x| x >= 3 }.should == 3
      end
    end

    context "with a block returning negative, zero, positive numbers" do
      it "returns nil if the block returns less than zero for every element" do
        (0..3).bsearch { |x| x <=> 5 }.should be_nil
      end

      it "returns nil if the block returns greater than zero for every element" do
        (0..3).bsearch { |x| x <=> -1 }.should be_nil

      end

      it "returns nil if the block never returns zero" do
        (0..3).bsearch { |x| x < 2 ? 1 : -1 }.should be_nil
      end

      it "accepts (+/-)Float::INFINITY from the block" do
        (0..4).bsearch { |x| Float::INFINITY }.should be_nil
        (0..4).bsearch { |x| -Float::INFINITY }.should be_nil
      end

      it "returns an element at an index for which block returns 0.0" do
        result = (0..4).bsearch { |x| x < 2 ? 1.0 : x > 2 ? -1.0 : 0.0 }
        result.should == 2
      end

      it "returns an element at an index for which block returns 0" do
        result = (0..4).bsearch { |x| x < 1 ? 1 : x > 3 ? -1 : 0 }
        [1, 2].should include(result)
      end
    end

    it "returns nil for empty ranges" do
      (0...0).bsearch { true }.should == nil
      (0...0).bsearch { false }.should == nil
      (0...0).bsearch { 1 }.should == nil
      (0...0).bsearch { 0 }.should == nil
      (0...0).bsearch { -1 }.should == nil

      (4..2).bsearch { true }.should == nil
      (4..2).bsearch { 1 }.should == nil
      (4..2).bsearch { 0 }.should == nil
      (4..2).bsearch { -1 }.should == nil
    end
  end

  context "with Float values" do
    context "with a block returning true or false" do
      it "returns nil if the block returns false for every element" do
        (0.1...2.3).bsearch { |x| x > 3 }.should be_nil
      end

      it "returns nil if the block returns nil for every element" do
        (-0.0..2.3).bsearch { |x| nil }.should be_nil
      end

      it "returns minimum element if the block returns true for every element" do
        (-0.2..4.8).bsearch { |x| x < 5 }.should == -0.2
      end

      it "returns the smallest element for which block returns true" do
        (0..4.2).bsearch { |x| x >= 2 }.should == 2
        (-1.2..4.3).bsearch { |x| x >= 1 }.should == 1
      end

      it "returns a boundary element if appropriate" do
        (1.0..3.0).bsearch { |x| x >= 3.0 }.should == 3.0
        (1.0...3.0).bsearch { |x| x >= 3.0.prev_float }.should == 3.0.prev_float
        (1.0..3.0).bsearch { |x| x >= 1.0 }.should == 1.0
        (1.0...3.0).bsearch { |x| x >= 1.0 }.should == 1.0
      end

      it "works with infinity bounds" do
        inf = Float::INFINITY
        (0..inf).bsearch { |x| x == inf }.should == inf
        (0...inf).bsearch { |x| x == inf }.should == nil
        (-inf..0).bsearch { |x| x != -inf }.should == -Float::MAX
        (-inf...0).bsearch { |x| x != -inf }.should == -Float::MAX
        (inf..inf).bsearch { |x| true }.should == inf
        (inf...inf).bsearch { |x| true }.should == nil
        (-inf..-inf).bsearch { |x| true }.should == -inf
        (-inf...-inf).bsearch { |x| true }.should == nil
        (inf..0).bsearch { true }.should == nil
        (inf...0).bsearch { true }.should == nil
        (0..-inf).bsearch { true }.should == nil
        (0...-inf).bsear