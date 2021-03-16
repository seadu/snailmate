require_relative '../../spec_helper'
require_relative 'fixtures/classes'

describe "Kernel#instance_variables" do
  describe "immediate values" do
    it "returns an empty array if no instance variables are defined" do
      [0, 0.5, true, false, nil].each do |value|
        value.instance_variables.should == []
      end
    end

    it "returns the correct array if an instance variable is added" do
      a = 0
      ->{ a.instance_variable_set("@test", 1) }.should r