require_relative '../../spec_helper'
require_relative 'fixtures/classes'

describe "Method#source_location" do
  before :each do
    @method = MethodSpecs::SourceLocation.method(:location)
  end

  it "returns an Array" do
    @method.source_location.should be_an_instance_of(Array)
  end

  it "sets the first value to the path of the file in which the method was defined" do
    file = @method.source_location.first
    file.should be_an_instance_of(String)
    file.should == File.realpath('../fixtures/classes.rb', __FILE__)
  end

  it "sets the last value to an Integer representing the line on which the method was defined" do
    line = @method.source_location.last
    line.should be_an_instance_of(Integer)
    line.should == 5
  end

  it "returns the last place the method was defined" do
    MethodSpecs::SourceLocation.method(:redefined).source_location.last.should == 13
  end

  it "returns the location of the original method even if it was aliased" do
    MethodSpecs::SourceLocation.new.method(:aka).source_location.last.should == 17
  end

  it "works for methods defined with a block" do
    line = nil
    klass = Class.new do
      line = __LINE__ + 1
      define_method(:f) { }
    end

    method = klass.new.method(:f)
    method.source_location[0].should =~ /#{__FILE__}/
    method.source_location[1].should == line
  end

  it "works for methods defined with a Method" do
    line = nil
    klass = Class.new do
      line = __LINE__ + 1
      def f
      end
      define_method :g,