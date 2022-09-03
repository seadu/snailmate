require_relative '../../spec_helper'
require 'set'

ruby_version_is "3.0" do
  describe "Set#join" do
    it "returns an empty string if the Set is empty" do
      Set[].join.should == ''
    end

    it "returns a new string formed by joining elements after conversion" do
      set = Set[:a, :b, :c]
      set.join.should == "abc"
    end

    it "does not separate elements when the passed separator is nil" do
      set = Set[:a, :b, :c]
      set.join(nil).should == "abc"
    end

    it "returns a string formed by concatenating each element s