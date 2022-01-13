# frozen_string_literal: false
require 'test/unit'
require "c/string"

class Test_StringCStr < Test::Unit::TestCase
  Bug4319 = '[ruby-dev:43094]'

  def test_embed
    s = Bug::String.new("abcdef")
    s.set_len(3)
    s.cstr_unterm('x')
    assert_equal(0, s.cstr_term, Bug4319)
  end

  def test_long
    s = Bug::String.new(Bug::String.new("abcdef")*100000)
    s.cstr_unterm('x')
    assert_equal(0, s.cstr_term, Bug4319)
  end

  def test_shared
    s = Bug::String.new(Bug::String.new("abcdef")*5)
    s = s.unterminated_substring(0, 29)
    assert_equal(0, s.cstr_term, Bug4319)
  end

  def test_frozen
    s0 = Bug::String.new("abcdefgh"*8)

    [4, 4*3-1, 8*3-1, 64].each do |n|
      s = Bug::String.new(s0[0, n])
      s.cstr_unterm('x')
      s.freeze
      assert_equal(0, s.cstr_term)
      WCHARS.each do |enc|
        s = s0.encode(enc)
        s.set_len(n -