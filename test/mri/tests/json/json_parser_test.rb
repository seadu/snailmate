# encoding: utf-8
# frozen_string_literal: false
require 'test_helper'
require 'stringio'
require 'tempfile'
require 'ostruct'
require 'bigdecimal'

class JSONParserTest < Test::Unit::TestCase
  include JSON

  def test_construction
    parser = JSON::Parser.new('test')
    assert_equal 'test', parser.source
  end

  def test_argument_encoding
    source = "{}".encode("UTF-16")
    JSON::Parser.new(source)
    assert_equal Encoding::UTF_16, source.encoding
  end if defined?(Encoding::UTF_16)

  def test_error_message_encoding
    bug10705 = '[ruby-core:67386] [Bug #10705]'
    json = ".\"\xE2\x88\x9A\"".force_encoding(Encoding::UTF_8)
    e = assert_raise(JSON::ParserError) {
      JSON::Ext::Parser.new(json).parse
    }
    assert_equal(Encoding::UTF_8, e.message.encoding, bug10705)
    assert_include(e.message, json, bug10705)
  end if defined?(Encoding::UTF_8) and defined?(JSON::Ext::Parser)

  def test_parsing
    parser = JSON::Parser.new('"test"')
    assert_equal 'test', parser.parse
  end

  def test_parser_reset
    parser = Parser.new('{"a":"b"}')
    assert_equal({ 'a' => 'b' }, parser.parse)
    assert_equal({ 'a' => 'b' }, parser.parse)
  end

  def test_parse_values
    assert_equal(nil,      parse('null'))
    assert_equal(false,    parse('false'))
    assert_equal(true,     parse('true'))
    assert_equal(-23,      parse('-23'))
    assert_equal(23,       parse('23'))
    assert_in_delta(0.23,  parse('0.23'), 1e-2)
    assert_in_delta(0.0,   parse('0e0'), 1e-2)
    assert_equal("",       parse('""'))
    assert_equal("foobar", parse('"foobar"'))
  end

  def test_parse_simple_arrays
    assert_equal([],             parse('[]'))
    assert_equal([],             parse('  [  ] '))
    assert_equal([ nil ],        parse('[null]'))
    assert_equal([ false ],      parse('[false]'))
    assert_equal([ true ],       parse('[true]'))
    assert_equal([ -23 ],        parse('[-23]'))
    assert_equal([ 23 ],         parse('[23]'))
    assert_equal_float([ 0.23 ], parse('[0.23]'))
    assert_equal_float([ 0.0 ],  parse('[0e0]'))
    assert_equal([""],           parse('[""]'))
    assert_equal(["foobar"],     parse('["foobar"]'))
    assert_equal([{}],           parse('[{}]'))
  end

  def test_parse_simple_objects
    assert_equal({}, parse('{}'))
    assert_equal({}, parse(' {   }   '))
    assert_equal({ "a" => nil }, parse('{   "a"   :  null}'))
    assert_equal({ "a" => nil }, parse('{"a":null}'))
    assert_equal({ "a" => false }, parse('{   "a"  :  false  }  '))
    assert_equal({ "a" => false }, parse('{"a":false}'))
    assert_raise(JSON::ParserError) { parse('{false}') }
    assert_equal({ "a" => true }, parse('{"a":true}'))
    assert_equal({ "a" => true }, parse('  { "a" :  true  }   '))
    assert_equal({ "a" => -23 }, parse('  {  "a"  :  -23  }  '))
    assert_equal({ "a" => -23 }, parse('  { "a" : -23 } '))
    assert_equal({ "a" => 23 }, parse('{"a":23  } '))
    assert_equal({ "a" => 23 }, parse('  { "a"  : 23  } '))
    assert_equal({ "a" => 0.23 }, parse(' { "a"  :  0.23 }  '))
    assert_equal({ "a" => 0.23 }, parse('  {  "a"  :  0.23  }  '))
  end

  def test_parse_numbers
    assert_raise(JSON::ParserError) { parse('+23.2') }
    assert_raise(JSON::ParserError) { parse('+23') }
    assert_raise(JSON::ParserError) { parse('.23') }
    assert_raise(JSON::ParserError) { parse('023') }
    assert_equal(23, parse('23'))
    assert_equal(-23, parse('-23'))
    assert_equal_float(3.141, parse('3.141'))
    assert_equal_float(-3.141, parse('-3.141'))
    assert_equal_float(3.141, parse('3141e-3'))
    assert_equal_float(3.141, parse('3141.1e-3'))
    assert_equal_float(3.141, parse('3141E-3'))
    assert_equal_float(3.141, parse('3141.0E-3'))
    assert_equal_float(-3.141, parse('-3141.0e-3'))
    assert_equal_float(-3.141, parse('-3141e-3'))
    assert_raise(ParserError) { parse('NaN') }
    assert parse('NaN', :allow_nan => true).nan?
    assert_raise(ParserError) { parse('Infinity') }
    assert_equal(1.0/0, parse('Infinity', :allow_nan => true))
    assert_raise(ParserError) { parse('-Infinity') }
    assert_equal(-1.0/0, parse('-Infinity', :allow_nan => true))
  end

  def test_parse_bigdecimals
    assert_equal(BigDecimal,                             JSON.parse('{"foo": 9.01234567890123456789}', decimal_class: BigDecimal)["foo"].class)
    assert_equal(BigDecimal("0.901234567890123456789E1"),JSON.parse('{"foo": 9.01234567890123456789}', decimal_class: BigDecimal)["foo"]      )
  end

  if Array.method_defined?(:permutation)
    def test_parse_more_complex_arrays
      a = [ nil, false, true, "foßbar", [ "n€st€d", true ], { "nested" => true, "n€ßt€ð2" => {} }]
      a.permutation.each do |perm|
        json = pretty_generate(perm)
        assert_equal perm, parse(json)
      end
    end

    def test_parse_complex_objects
      a = [ nil, false, true, "foßbar", [ "n€st€d", true ], { "nested" => true, "n€ßt€ð2" => {} }]
      a.permutation.each do |perm|
        s = "a"
        orig_obj = perm.inject({}) { |h, x| h[s.dup] = x; s = s.succ; h }
        json = pretty_generate(orig_obj)
        assert_equal orig_obj, parse(json)
      end
    end
  end

  def test_parse_arrays
    assert_equal([1,2,3], parse('[1,2,3]'))
    assert_equal([1.2,2,3], parse('[1.2,2,3]'))
    assert_equal([[],[[],[]]], parse('[[],[[],[]]]'))
    a