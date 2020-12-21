require 'bigdecimal'

describe :bigdecimal_quo, shared: true do
  before :each do
    @one = BigDecimal("1")
    @zero = BigDecimal("0")
    @zero_plus = BigDecimal("+0")
    @zero_minus = BigDecimal("-0")
    @two = BigDecimal("2")
    @three = BigDecimal("3")
    @eleven = BigDecimal("11")
    @nan = BigDecimal("NaN")
    @infinity = BigDecimal("Infinity")
    