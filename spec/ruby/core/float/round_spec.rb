require_relative '../../spec_helper'

describe "Float#round" do
  it "returns the nearest Integer" do
    5.5.round.should == 6
    0.4.round.should == 0
    0.6.round.should == 1
    -1.4.round.should == -1
    -2.8.round.should == -3
    0.0.round.should == 0
  end

  it "returns the nearest Integer for Float near the limit" do
    0.49999999999999994.round.should == 0
    -0.49999999999999994.round.should == 0
  end

  it "raises FloatDomainError for exceptional values" do
    -> { (+infinity_value).round }.should raise_error(FloatDomainError)
    -> { (-infinity_value).round }.should raise_error(FloatDomainError)
    -> { nan_value.round }.should raise_error(FloatDomainError)
  end

  it "rounds self to an optionally given precision" do
    5.5.round(0).should eql(6)
    5.7.round(1).should eql(5.7)
   