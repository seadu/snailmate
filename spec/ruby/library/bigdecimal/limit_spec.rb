require_relative '../../spec_helper'
require_relative 'fixtures/classes'
require 'bigdecimal'

describe "BigDecimal.limit" do
  it "returns the value before set if the passed argument is nil or is not specified" do
    old = BigDecimal.limit
    BigDecimal.limit.should == 0
    BigDecimal.limit(10).should == 0
    BigDecimal.limit.should == 10
    BigDecimal.limit(old)
  end

  it "uses the global limit if no precision is specified" do
    BigDecimalSpecs.with_limit(0) do
      (BigDecimal('0.888') + BigDecimal('0')).should == BigDecimal('0.888')
      (BigDecimal('0.888') - Bi