require_relative '../../spec_helper'
require_relative '../../fixtures/rational'

describe :rational_cmp_rat, shared: true do
  it "returns 1 when self is greater than the passed argument" do
    (Rational(4, 4) <=> Rational(3, 4)).should equal(1)
    (Rational(-3, 4) <=> Rational(-4, 4)).should equal(1)
  end

  it "returns 0 when self is equal to the passed argument" do
    (Rational(4, 4) <=> Rational(4, 4)).should equal(0)
    (Rational(-3, 4) <=> Rational(-3, 4)).should equal(0)
  end

  it "returns -1 when self is less than the passed argument" do
    (Rational(3, 4) <=> Rational(4, 4)).should equal(-1)
    (Rational(-4, 4) <=> Rational(-3, 4)).should equal(-1)
  end
end

describe :rational_cmp_int, shared: true do
  it "returns 1 when self is greater than the passed argument" do
    (Rational(4, 4) <=> 0).should equal(1)
    (Rational(4, 4) <=> -10).should equal(1)
    (Rational(-3, 4) <=> -1).should equal(1)
  end

  it "returns 0 when self is equal to the passed argument" do
    (Rational(4, 4) <=> 1).should equal(0)
    (Rational(-8, 4) <=> -2).should equal(0)
  end

  it "returns -1 when self is less than the passed argument" do
    (Rational(3, 4) <=> 1).should equal(-1)
    (Rational(-4, 4) <=> 0).should equal(-1)
  end
end

describe :rational_cmp_float, shared: true do
  it "returns 1 when self is greater than the passed argument" do
    (Rational(4, 4) <=> 0.5).should equal(1)
    (Rational(4, 4) <=> -1.5).should equal(1)
    (Rational(-3, 4) <=> -0.8).should equal(1)
  end

  it "returns 0 when self is equal to the passed argument" do
    (Rational(4, 4) <=> 1.0).should equal(0)
    (Rational(-6, 4) <=> -1.5).should equal(0)
  end

  it "returns -1 when self is less than the passed argument" do
    (Rational(3, 4) <=> 1.2).should equal(-1)
    (Rationa