# frozen_string_literal: false
require 'test/unit'

class TestSprintf < Test::Unit::TestCase
  def test_positional
    assert_equal("     00001", sprintf("%*1$.*2$3$d", 10, 5, 1))
  end

  def test_binary
    assert_equal("0", sprintf("%b", 0))
    assert_equal("1", sprintf("%b", 1))
    assert_equal("10", sprintf("%b", 2))
    assert_equal("..1", sprintf("%b", -1))

    assert_equal("   0", sprintf("%4b", 0))
    assert_equal("   1", sprintf("%4b", 1))
    assert_equal("  10", sprintf("%4b", 2))
    assert_equal(" ..1", sprintf("%4b", -1))

    assert_equal("0000", sprintf("%04b", 0))
    assert_equal("0001", sprintf("%04b", 1))
    assert_equal("0010", sprintf("%04b", 2))
    assert_equal("..11", sprintf("%04b", -1))

    assert_equal("0000", sprintf("%.4b", 0))
    assert_equal("0001", sprintf("%.4b", 1))
    assert_equal("0010", sprintf("%.4b", 2))
    assert_equal("..11", sprintf("%.4b", -1))

    assert_equal("  0000", sprintf("%6.4b", 0))
    assert_equal("  0001", sprintf("%6.4b", 1))
    assert_equal("  0010", sprintf("%6.4b", 2))
    assert_equal("  ..11", sprintf("%6.4b", -1))

    assert_equal("   0", sprintf("%#4b", 0))
    assert_equal(" 0b1", sprintf("%#4b", 1))
    assert_equal("0b10", sprintf("%#4b", 2))
    assert_equal("0b..1", sprintf("%#4b", -1))

    assert_equal("0000", sprintf("%#04b", 0))
    assert_equal("0b01", sprintf("%#04b", 1))
    assert_equal("0b10", sprintf("%#04b", 2))
    assert_equal("0b..1", sprintf("%#04b", -1))

    assert_equal("0000", sprintf("%#.4b", 0))
    assert_equal("0b0001", sprintf("%#.4b", 1))
    assert_equal("0b0010", sprintf("%#.4b", 2))
    assert_equal("0b..11", sprintf("%#.4b", -1))

    assert_equal("  0000", sprintf("%#6.4b", 0))
    assert_equal("0b0001", sprintf("%#6.4b", 1))
    assert_equal("0b0010", sprintf("%#6.4b", 2))
    assert_equal("0b..11", sprintf("%#6.4b", -1))

    assert_equal("+0", sprintf("%+b", 0))
    assert_equal("+1", sprintf("%+b", 1))
    assert_equal("+10", sprintf("%+b", 2))
    assert_equal("-1", sprintf("%+b", -1))

    assert_equal("  +0", sprintf("%+4b", 0))
    assert_equal("  +1", sprintf("%+4b", 1))
    assert_equal(" +10", sprintf("%+4b", 2))
    assert_equal("  -1", sprintf("%+4b", -1))

    assert_equal("+000", sprintf("%+04b", 0))
    assert_equal("+001", sprintf("%+04b", 1))
    assert_equal("+010", sprintf("%+04b", 2))
    assert_equal("-001", sprintf("%+04b", -1))

    assert_equal("+0000", sprintf("%+.4b", 0))
    assert_equal("+0001", sprintf("%+.4b", 1))
    assert_equal("+0010", sprintf("%+.4b", 2))
    assert_equal("-0001", sprintf("%+.4b", -1))

    assert_equal(" +0000", sprintf("%+6.4b", 0))
    assert_equal(" +0001", sprintf("%+6.4b", 1))
    assert_equal(" +0010", sprintf("%+6.4b", 2))
    assert_equal(" -0001", sprintf("%+6.4b", -1))
  end

  def test_nan
    nan = 0.0 / 0.0
    assert_equal("NaN", sprintf("%f", nan))
    assert_equal("NaN", sprintf("%-f", nan))
    assert_equal("+NaN", sprintf("%+f", nan))

    assert_equal("NaN", sprintf("%3f", nan))
    assert_equal("NaN", sprintf("%-3f", nan))
    assert_equal("+NaN", sprintf("%+3f", nan))

    assert_equal(" NaN", sprintf("% 3f", nan))
    assert_equal(" NaN", sprintf("%- 3f", nan))
    assert_equal("+NaN", sprintf("%+ 3f", nan))

    assert_equal(" NaN", sprintf("% 03f", nan))
    assert_equal(" NaN", sprintf("%- 03f", nan))
    assert_equal("+NaN", sprintf("%+ 03f", nan))

    assert_equal("     NaN", sprintf("%8f", nan))
    assert_equal("NaN     ", sprintf("%-8f", nan))
    assert_equal("    +NaN", sprintf("%+8f", nan))

    assert_equal("     NaN", sprintf("%08f", nan))
    assert_equal("NaN     ", sprintf("%-08f", nan))
    assert_equal("    +NaN", sprintf("%+08f", nan))

    assert_equal("     NaN", sprintf("% 8f", nan))
    assert_equal(" NaN    ", sprintf("%- 8f", nan))
    assert_equal("    +NaN", sprintf("%+ 8f", nan))

    assert_equal("     NaN", sprintf("% 08f", nan))
    assert_equal(" NaN    ", sprintf("%- 08f", nan))
    assert_equal("    +NaN", sprintf("%+ 08f", nan))
  end

  def test_inf
    inf = 1.0 / 0.0
    assert_equal("Inf", sprintf("%f", inf))
    assert_equal("Inf", sprintf("%-f", inf))
    assert_equal("+Inf", sprintf("%+f", inf))

    assert_equal(" Inf", sprintf("% f", inf))
    assert_equal(" Inf", sprintf("%- f", inf))
    assert_equal("+Inf", sprintf("%+ f", inf))

    assert_equal(" Inf", sprintf("% 0f", inf))
    assert_equal(" Inf", sprintf("%- 0f", inf))
    assert_equal("+Inf", sprintf("%+ 0f", inf))

    assert_equal("Inf", sprintf("%3f", inf))
    assert_equal("Inf", sprintf("%-3f", inf))
    assert_equal("+Inf", sprintf("%+3f", inf))

    assert_equal(" Inf", sprintf("% 3f", inf))
    assert_equal(" Inf", sprintf("%- 3f", inf))
    assert_equal("+Inf", sprintf("%+ 3f", inf))

    assert_equal(" Inf", sprintf("% 03f", inf))
    assert_equal(" Inf", sprintf("%- 03f", inf))
    assert_equal("+Inf", sprintf("%+ 03f", inf))

    assert_equal("     Inf", sprintf("%8f", inf))
    assert_equal("Inf     ", sprintf("%-8f", inf))
    assert_equal("    +Inf", sprintf("%+8f", inf))

    assert_equal("     Inf", sprintf("%08f", inf))
    assert_equal("Inf     ", sprintf("%-08f", inf))
    assert_equal("    +Inf", sprintf("%+08f", inf))

    assert_equal("     Inf", sprintf("% 8f", inf))
    assert_equal(" Inf    ", sprintf("%- 8f", inf))
    assert_equal("    +Inf", sprintf("%+ 8f", inf))

    assert_equal("     Inf", sprintf("% 08f", inf))
    assert_equal(" Inf    ", sprintf("%- 08f", inf))
    assert_equal("    +Inf", sprintf("%+ 08f", inf))

    assert_equal("-Inf", sprintf("%f", -inf))
    assert_equal("-Inf", sprintf("%-f", -inf))
    assert_equal("-Inf", sprintf("%+f", -inf))

    assert_equal("-Inf", sprintf("% f", -inf))
    assert_equal("-Inf", sprintf("%- f", -inf))
    assert_equal("-Inf", sprintf("%+ f", -inf))

    assert_equal("-Inf", sprintf("% 0f", -inf))
    assert_equal("-Inf", sprintf("%- 0f", -inf))
    assert_equal("-Inf", sprintf("%+ 0f", -inf))

    assert_equal("-Inf", sprintf("%4f", -inf))
    assert_equal("-Inf", sprintf("%-4f", -inf))
    assert_equal("-Inf", sprintf("%+4f", -inf))

    assert_equal("-Inf", sprintf("% 4f", -inf))
    assert_equal("-Inf", sprintf("%- 4f", -inf))
    assert_equal("-Inf", sprintf("%+ 4f", -inf))

    assert_equal("-Inf", sprintf("% 04f", -inf))
    assert_equal("-Inf", sprintf("%- 04f", -inf))
    assert_equal("-Inf", sprintf("%+ 04f", -inf))

    assert_equal("    -Inf", sprintf("%8f", -inf))
    assert_equal("-Inf    ", sprintf("%-8f", -inf))
    assert_equal("    -Inf", sprintf("%+8f", -inf))

    assert_equal("    -Inf", sprintf("%08f", -inf))
    assert_equal("-Inf    ", sprintf("%-08f", -inf))
    assert_equal("    -Inf", sprintf("%+08f", -inf))

    assert_equal("    -Inf", sprintf("% 8f", -inf))
    assert_equal("-Inf    ", sprintf("%- 8f", -inf))
    assert_equal("    -Inf", sprintf("%+ 8f", -inf))

    assert_equal("    -Inf", sprintf("% 08f", -inf))
    assert_equal("-Inf    ", sprintf("%- 08f", -inf))
    assert_equal("    -Inf", sprintf("%+ 08f", -inf))
    assert_equal('..f00000000',
      sprintf("%x", -2**32), '[ruby-dev:32351]')
    assert_equal("..101111111111111111111111111111111",
      sprintf("%b", -