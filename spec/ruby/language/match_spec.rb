require_relative '../spec_helper'
require_relative 'fixtures/match_operators'

describe "The !~ operator" do
  before :each do
    @obj = OperatorImplementor.new
  end

  it "evaluates as a call to !~" do
    expected = "hello world"

    opval = (@obj !~ expected)
    methodval = @obj.send(:"!~", expected)

    opval.should == expected
    methodval.should == expected
  end
end

describe "The =~ operator" do
  before :each do
    @impl = OperatorImplementor.new
  en