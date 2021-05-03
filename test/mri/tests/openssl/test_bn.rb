# coding: us-ascii
# frozen_string_literal: true
require_relative 'utils'

if defined?(OpenSSL)

class OpenSSL::TestBN < OpenSSL::TestCase
  def setup
    super
    @e1 = OpenSSL::BN.new(999.to_s(16), 16) # OpenSSL::BN.new(str, 16) must be most stable
    @e2 = OpenSSL::BN.new("-" + 999.to_s(16), 16)
    @e3 = OpenSSL::BN.new((2**107-1).to_s(16), 16)
    @e4 = OpenSSL::BN.new("-" + (2**107-1).to_s(16), 16)
  end

  def test_new
    assert_raise(ArgumentError) { OpenSSL::BN.new }
    assert_raise(ArgumentError) { OpenSSL::BN.new(nil) }
    assert_raise(ArgumentError) { OpenSSL::BN.new(nil, 2) }

    assert_equal(@e1, OpenSSL::BN.new("999"))
    assert_equal(@e1, OpenSSL::BN.new("999", 10))
    assert_equal(@e1, OpenSSL::BN.new("\x03\xE7", 2))
    assert_equal(@e1, OpenSSL::BN.new("\x00\x00\x00\x02\x03\xE7", 0))
    assert_equal(@e2, OpenSSL::BN.new("-999"))
    assert_equal(@e2, OpenSSL::BN.new("-999", 10))
    assert_equal(@e2, OpenSSL::BN.new("\x00\x00\x00\x02\x83\xE7", 0))
    assert_equal(@e3, OpenSSL::BN.new((2**107-1).to_s))
    assert_equal(@e3, OpenSSL::BN.new((2**107-1).to_s, 10))
    assert_equal(@e3, OpenSSL::BN.new("\a\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 2))
    assert_equal(@e3, OpenSSL::BN.new("\x00\x00\x00\x0E\a\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 0))
    assert_equal(@e4, OpenSSL::BN.new("-" + (2**107-1).to_s))
    assert_equal(@e4, OpenSSL::BN.new("-" + (2**107-1).to_s, 10))
    assert_equal(@e4, OpenSSL::BN.new("\x00\x00\x00\x0E\x87\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 0))

    e1copy = OpenSSL::BN.new(@e1)
    assert_equal(@e1, e1copy)
    e1copy.clear_bit!(0) #=> 998
    assert_not_equal(@e1, e1copy)

    assert_equal(@e1, OpenSSL::BN.new(999))
    assert_equal(@e2, OpenSSL::BN.new(-999))
    assert_equal(@e3, OpenSSL::BN.new(2**107-1))
    assert_equal(@e4, OpenSSL::BN.new(-(2**107-1)))

    assert_equal(@e1, 999.to_bn)
    assert_equal(@e2, -999.to_bn)
    assert_equal(@e3, (2**107-1).to_bn)
    assert_equal(@e4, (-(2**107-1)).to_bn)
  end

  def test_to_str
    assert_equal("999", @e1.to_s(10))
    assert_equal("-999", @e2.to_s(10))
    assert_equal((2**107-1).to_s, @e3.to_s(10))
    assert_equal((-(2**107-1)).to_s, @e4.to_s(10))
    assert_equal("999", @e1.to_s)

    assert_equal("03E7", @e1.to_s(16))
    assert_equal("-03E7", @e2.to_s(16))
    assert_equal("07FFFFFFFFFFFFFFFFFFFFFFFFFF", @e3.to_s(16))
    assert_equal("-07FFFFFFFFFFFFFFFFFFFFFFFFFF", @e4.to_s(16))

    assert_equal("\x03\xe7", @e1.to_s(2))
    assert_equal("\x03\xe7", @e2.to_s(2))
    assert_equal("\x07\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", @e3.to_s(2))
    assert_equal("\x07\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", @e4.to_s(2))

    assert_equal("\x00\x00\x00\x02\x03\xe7", @e1.to_s(0))
    assert_equal("\x00\x00\x00\x02\x83\xe7", @e2.to_s(0))
    assert_equal("\x00\x00\x00\x0e\x07\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", @e3.to_s(0))
    assert_equal("\x00\x00\x00\x0e\x87\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", @e4.to_s(0))
  end

  def test_to_int
    assert_equal(999, @e1.to_i)
    assert_equal(-999, @e2.to_i)
    assert_equal(2**107-1, @e3.to_i)
    assert_equal(-(2**107-1), @e4.to_i)

    assert_equal(999, @e1.to_int)
  end

  def test_coerce
    assert_equal(["", "-999"], @e2.coerce(""))
    assert_equal([1000, -999], @e2.coerce(1000))
    assert_raise(TypeError) { @e2.coerce(Class.new.new) }
  end

  def test_zero_p
    assert_equal(true, 0.to_bn.zero?)
    assert_equal(false, 1.to_bn.zero?)
  end

  def test_one_p
    assert_equal(true, 1.to_bn.one?)
    assert_equal(false, 2.to_bn.one?)
  end

  def test_odd_p
    assert_equal(true, 1.to_bn.odd?)
    assert_equal(false, 2.to_bn.odd?)
  end

  def test_negative_p
    assert_equ