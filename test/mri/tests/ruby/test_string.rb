# frozen_string_literal: false
require 'test/unit'

class TestString < Test::Unit::TestCase
  WIDE_ENCODINGS = [
     Encoding::UTF_16BE, Encoding::UTF_16LE,
     Encoding::UTF_32BE, Encoding::UTF_32LE,
  ]

  def initialize(*args)
    @cls = String
    @aref_re_nth = true
    @aref_re_silent = false
    @aref_slicebang_silent = true
    super
  end

  def S(*args, **kw)
    @cls.new(*args, **kw)
  end

  def test_s_new
    assert_equal("", S())
    assert_equal(Encoding::ASCII_8BIT, S().encoding)

    assert_equal("", S(""))
    assert_equal(__ENCODING__, S("").encoding)

    src = "RUBY"
    assert_equal(src, S(src))
    assert_equal(__ENCODING__, S(src).encoding)

    src.force_encoding("euc-jp")
    assert_equal(src, S(src))
    assert_equal(Encoding::EUC_JP, S(src).encoding)


    assert_equal("", S(encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S(encoding: "euc-jp").encoding)

    assert_equal("", S("", encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S("", encoding: "euc-jp").encoding)

    src = "RUBY"
    assert_equal(src, S(src, encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S(src, encoding: "euc-jp").encoding)

    src.force_encoding("euc-jp")
    assert_equal(src, S(src, encoding: "utf-8"))
    assert_equal(Encoding::UTF_8, S(src, encoding: "utf-8").encoding)

    assert_equal("", S(capacity: 1000))
    assert_equal(Encoding::ASCII_8BIT, S(capacity: 1000).encoding)

    assert_equal("", S(capacity: 1000, encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S(capacity: 1000, encoding: "euc-jp").encoding)

    assert_equal("", S("", capacity: 1000))
    assert_equal(__ENCODING__, S("", capacity: 1000).encoding)

    assert_equal("", S("", capacity: 1000, encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S("", capacity: 1000, encoding: "euc-jp").encoding)
  end

  def test_initialize
    str = S("").freeze
    assert_equal("", str.__send__(:initialize))
    assert_raise(FrozenError){ str.__send__(:initialize, 'abc') }
    assert_raise(FrozenError){ str.__send__(:initialize, capacity: 1000) }
    assert_raise(FrozenError){ str.__send__(:initialize, 'abc', capacity: 1000) }
    assert_raise(FrozenError){ str.__send__(:initialize, encoding: 'euc-jp') }
    assert_raise(FrozenError){ str.__send__(:initialize, 'abc', encoding: 'euc-jp') }
    assert_raise(FrozenError){ str.__send__(:initialize, 'abc', capacity: 1000, encoding: 'euc-jp') }

    str = S("")
    assert_equal("mystring", str.__send__(:initialize, "mystring"))
    str = S("mystring")
    assert_equal("mystring", str.__send__(:initialize, str))
    str = S("")
    assert_equal("mystring", str.__send__(:initialize, "mystring", capacity: 1000))
    str = S("mystring")
    assert_equal("mystring", str.__se