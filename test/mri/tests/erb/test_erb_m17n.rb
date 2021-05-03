# -*- coding: utf-8 -*-
# frozen_string_literal: false
require 'test/unit'
require 'erb'

class TestERBEncoding < Test::Unit::TestCase
  def test_result_encoding
    erb = ERB.new("hello")
    assert_equal __ENCODING__, erb.result.encoding

    erb = ERB.new("こんにちは".encode("EUC-JP"))
    assert_equal Encoding::EUC_JP, erb.result.encoding

    erb = ERB.new("\xC4\xE3\xBA\xC3".force_encoding("EUC-CN"))
    assert_equal Encoding::EUC_CN, erb.result.encoding

    erb = ERB.new("γεια σας".encode("ISO-8859-7"))
    assert_equal Encoding::ISO_8859_7, erb.result.encoding

    assert_raise(ArgumentError, /ASCII compatible/) {
      ERB.new("こんにちは".force_encoding("ISO-2022-JP")) # dummy encoding
    }
  end

  def test_generate_magic_comment
    erb = ERB.new("hello")
    assert_match(/#coding:UTF-8/, erb.src)

    erb = ERB.new("hello".force_encoding("EUC-JP"))
    assert_match(/#coding:EUC-JP/, erb.src)

    erb = ERB.new("hello".force_encoding("ISO-8859-9"))
    assert_match(/#coding:ISO-8859-9/, erb.src)
  end

  def test_literal_encoding
    erb = ERB.new("literal encoding is <%= 'hello'.encoding  %>")
    assert_match(/literal encoding is UTF-8/, erb.result)

    erb = ERB.new("literal encoding is <%= 'こんにちは'.encoding  %>".encode("EUC-JP"))
    assert_match(/literal encoding is EUC-JP/, erb.result)

    erb = ERB.new("literal encoding is <%= '\xC4\xE3\xBA\xC3'.encoding %>".force_encoding("EUC-CN"))
    assert_match(/literal encoding is GB2312/, erb.result)
  end

  def test___ENCODING__
    erb = ERB.new("__ENCODING__ is <%= __ENCODING__ %>")
    assert_match(/__ENCODING__ is UTF-8/, erb.result)

    erb = ERB.new("__ENCODING__ is <%= __ENCODING__ %>".force_encoding("EUC-JP"))
    assert_match(/__ENCODING__ is EUC-JP/, erb.result)

    erb = ERB.new("__ENCODING__ is <%= __ENCODING__ %>".force_encoding("Big5"))
    assert_match(/__ENCODING__ is Big5/, erb.result)
  end

  def test_recognize_magic_comme