
#frozen_string_literal: false
require 'json/common'

##
# = JavaScript \Object Notation (\JSON)
#
# \JSON is a lightweight data-interchange format.
#
# A \JSON value is one of the following:
# - Double-quoted text:  <tt>"foo"</tt>.
# - Number:  +1+, +1.0+, +2.0e2+.
# - Boolean:  +true+, +false+.
# - Null: +null+.
# - \Array: an ordered list of values, enclosed by square brackets:
#     ["foo", 1, 1.0, 2.0e2, true, false, null]
#
# - \Object: a collection of name/value pairs, enclosed by curly braces;
#   each name is double-quoted text;
#   the values may be any \JSON values:
#     {"a": "foo", "b": 1, "c": 1.0, "d": 2.0e2, "e": true, "f": false, "g": null}
#
# A \JSON array or object may contain nested arrays, objects, and scalars
# to any depth:
#   {"foo": {"bar": 1, "baz": 2}, "bat": [0, 1, 2]}
#   [{"foo": 0, "bar": 1}, ["baz", 2]]
#
# == Using \Module \JSON
#
# To make module \JSON available in your code, begin with:
#   require 'json'
#
# All examples here assume that this has been done.
#
# === Parsing \JSON
#
# You can parse a \String containing \JSON data using
# either of two methods:
# - <tt>JSON.parse(source, opts)</tt>
# - <tt>JSON.parse!(source, opts)</tt>
#
# where
# - +source+ is a Ruby object.
# - +opts+ is a \Hash object containing options
#   that control both input allowed and output formatting.
#
# The difference between the two methods
# is that JSON.parse! omits some checks
# and may not be safe for some +source+ data;
# use it only for data from trusted sources.
# Use the safer method JSON.parse for less trusted sources.
#
# ==== Parsing \JSON Arrays
#
# When +source+ is a \JSON array, JSON.parse by default returns a Ruby \Array:
#   json = '["foo", 1, 1.0, 2.0e2, true, false, null]'
#   ruby = JSON.parse(json)
#   ruby # => ["foo", 1, 1.0, 200.0, true, false, nil]
#   ruby.class # => Array
#
# The \JSON array may contain nested arrays, objects, and scalars
# to any depth:
#   json = '[{"foo": 0, "bar": 1}, ["baz", 2]]'
#   JSON.parse(json) # => [{"foo"=>0, "bar"=>1}, ["baz", 2]]
#
# ==== Parsing \JSON \Objects
#
# When the source is a \JSON object, JSON.parse by default returns a Ruby \Hash:
#   json = '{"a": "foo", "b": 1, "c": 1.0, "d": 2.0e2, "e": true, "f": false, "g": null}'
#   ruby = JSON.parse(json)
#   ruby # => {"a"=>"foo", "b"=>1, "c"=>1.0, "d"=>200.0, "e"=>true, "f"=>false, "g"=>nil}
#   ruby.class # => Hash
#
# The \JSON object may contain nested arrays, objects, and scalars
# to any depth:
#   json = '{"foo": {"bar": 1, "baz": 2}, "bat": [0, 1, 2]}'
#   JSON.parse(json) # => {"foo"=>{"bar"=>1, "baz"=>2}, "bat"=>[0, 1, 2]}
#
# ==== Parsing \JSON Scalars
#
# When the source is a \JSON scalar (not an array or object),
# JSON.parse returns a Ruby scalar.
#
# \String:
#   ruby = JSON.parse('"foo"')
#   ruby # => 'foo'
#   ruby.class # => String
# \Integer:
#   ruby = JSON.parse('1')
#   ruby # => 1
#   ruby.class # => Integer
# \Float:
#   ruby = JSON.parse('1.0')
#   ruby # => 1.0
#   ruby.class # => Float
#   ruby = JSON.parse('2.0e2')
#   ruby # => 200
#   ruby.class # => Float
# Boolean:
#   ruby = JSON.parse('true')
#   ruby # => true
#   ruby.class # => TrueClass
#   ruby = JSON.parse('false')
#   ruby # => false
#   ruby.class # => FalseClass
# Null:
#   ruby = JSON.parse('null')
#   ruby # => nil
#   ruby.class # => NilClass
#
# ==== Parsing Options
#
# ====== Input Options
#
# Option +max_nesting+ (\Integer) specifies the maximum nesting depth allowed;
# defaults to +100+; specify +false+ to disable depth checking.
#
# With the default, +false+:
#   source = '[0, [1, [2, [3]]]]'
#   ruby = JSON.parse(source)
#   ruby # => [0, [1, [2, [3]]]]
# Too deep:
#   # Raises JSON::NestingError (nesting of 2 is too deep):
#   JSON.parse(source, {max_nesting: 1})
# Bad value:
#   # Raises TypeError (wrong argument type Symbol (expected Fixnum)):
#   JSON.parse(source, {max_nesting: :foo})
#
# ---
#
# Option +allow_nan+ (boolean) specifies whether to allow
# NaN, Infinity, and MinusInfinity in +source+;
# defaults to +false+.
#
# With the default, +false+:
#   # Raises JSON::ParserError (225: unexpected token at '[NaN]'):
#   JSON.parse('[NaN]')
#   # Raises JSON::ParserError (232: unexpected token at '[Infinity]'):
#   JSON.parse('[Infinity]')
#   # Raises JSON::ParserError (248: unexpected token at '[-Infinity]'):
#   JSON.parse('[-Infinity]')
# Allow:
#   source = '[NaN, Infinity, -Infinity]'
#   ruby = JSON.parse(source, {allow_nan: true})
#   ruby # => [NaN, Infinity, -Infinity]
#
# ====== Output Options
#
# Option +symbolize_names+ (boolean) specifies whether returned \Hash keys
# should be Symbols;
# defaults to +false+ (use Strings).
#
# With the default, +false+:
#   source = '{"a": "foo", "b": 1.0, "c": true, "d": false, "e": null}'
#   ruby = JSON.parse(source)
#   ruby # => {"a"=>"foo", "b"=>1.0, "c"=>true, "d"=>false, "e"=>nil}
# Use Symbols:
#   ruby = JSON.parse(source, {symbolize_names: true})
#   ruby # => {:a=>"foo", :b=>1.0, :c=>true, :d=>false, :e=>nil}
#
# ---
#
# Option +object_class+ (\Class) specifies the Ruby class to be used
# for each \JSON object;
# defaults to \Hash.
#
# With the default, \Hash:
#   source = '{"a": "foo", "b": 1.0, "c": true, "d": false, "e": null}'
#   ruby = JSON.parse(source)
#   ruby.class # => Hash
# Use class \OpenStruct:
#   ruby = JSON.parse(source, {object_class: OpenStruct})
#   ruby # => #<OpenStruct a="foo", b=1.0, c=true, d=false, e=nil>
#
# ---
#
# Option +array_class+ (\Class) specifies the Ruby class to be used
# for each \JSON array;
# defaults to \Array.
#
# With the default, \Array: