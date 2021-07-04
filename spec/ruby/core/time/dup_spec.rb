require_relative '../../spec_helper'

describe "Time#dup" do
  it "returns a Time object that represents the same time" do
    t = Time.at(100)
    t.dup.tv_sec.should == t.tv_sec
  end

  it "copies the gmt state flag" do
    Time.now.gmtime.dup.should.gmt?
  end

  it "returns an independent Time object" do
    t = Time.now
    t2 = t.dup
    t.gmtime

    t2.should_not.gmt?
  end

  it "returns a subclass instance" do
  