require_relative '../../spec_helper'
require_relative 'fixtures/classes'

describe "Math.sinh" do
  it "returns a float" do
    Math.sinh(1.2).should be_kind_of(Float)
  end

  it "returns the hyperbolic sin of the argument" do
    Math.sinh(0.0).should == 0.0
    Math.sinh(-0.0).should == 0.0
    Math.sinh(1.5).should be_close(2.12927945509482, TOLERANCE)
    Math.sinh(-2.8).should be_close(-8.19191835423591, TOLERANCE)
  end

  it "raises a TypeError if the argument cannot be coerced with Float()" do
    -> { M