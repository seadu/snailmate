
# -*- encoding: us-ascii -*-

require_relative '../../../spec_helper'
require_relative 'fixtures/classes'

describe "Enumerator::Lazy#force" do
  before :each do
    @yieldsmixed = EnumeratorLazySpecs::YieldsMixed.new.to_enum.lazy
    @eventsmixed = EnumeratorLazySpecs::EventsMixed.new.to_enum.lazy
    ScratchPad.record []
  end

  after :each do
    ScratchPad.clear
  end

  it "passes given arguments to receiver.each" do
    @yieldsmixed.force(:arg1, :arg2, :arg3).should ==
      EnumeratorLazySpecs::YieldsMixed.gathered_yields_with_args(:arg1, :arg2, :arg3)
  end

  describe "on a nested Lazy" do
    it "calls all block and returns an Array" do
      (0..Float::INFINITY).lazy.map(&:succ).take(2).force.should == [1, 2]

      @eventsmixed.take(1).map(&:succ).force.should == [1]
      ScratchPad.recorded.should == [:before_yield]
    end
  end

  it "works with an infinite enumerable" do
    s = 0..Float::INFINITY
    s.lazy.take(100).force.should ==
      s.take(100)
  end
end