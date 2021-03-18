# frozen_string_literal: false
require 'test/unit'

module TestEOF
  def test_eof_0
    open_file("") {|f|
      assert_equal("", f.read(0))
      assert_equal("", f.read(0))
      assert_equal("", f.read)
      assert_equal("", f.read(0))
      assert_equal("", f.read(0))
    }
    open_file("") {|f|
      assert_nil(f.read(1))
      assert_equal("", f.read)
      assert_nil(f.read(1))
    }
    open_file("") {|f|
      s = "x"
      assert_equal("", f.read(nil, s))
      assert_equal("", s)
    }
    open_file("") {|f|
      s = "x"
      assert_nil(f.read(10, s))
      assert_equal("", s)
    }
  end

  def test_eof_0_rw
    return unless respond_to? :open_file_rw
    open_file_rw("") {|f|
      assert_equal("", f.read)
      assert_equal("", f.read)
      assert_equal(0, f.syswrite(""))
      assert_equal("", f.read)
    }
  end

  def test_eof_1
    open_file("a") {|f|
      assert_equal("", f.read(0))
      assert_equal("a", f.read(1))
      assert_equal("" , f.read(0))
      assert_equal("" , f.read(0))
      assert_equal("", f.read)
      assert_equal("", f.read(0))
      assert_equal("", f.read(0))
    }
    open_file("a") {|f|
      a