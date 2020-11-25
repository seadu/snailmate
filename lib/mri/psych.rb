# frozen_string_literal: true
require_relative 'psych/versions'
case RUBY_ENGINE
when 'jruby'
  require_relative 'psych_jars'
  if JRuby::Util.respond_to?(:load_ext)
    JRuby::Util.load_ext('org.jruby.ext.psych.PsychLibrary')
  else
    require 'java'; require 'jruby'
    org.jruby.ext.psych.PsychLibrary.new.load(JRuby.runtime, false)
  end
else
  require 'psych.so'
end
require_relative 'psych/nodes'
require_relative 'psych/streaming'
require_relative 'psych/visitors'
require_relative 'psych/handler'
require_relative 'psych/tree_builder'
require_relative 'psych/parser'
require_relative 'psych/omap'
require_relative 'psych/set'
require_relative 'psych/coder'
require_relative 'psych/core_ext'
require_relative 'psych/stream'
require_relative 'psych/json/tree_builder'
require_relative 'psych/json/stream'
require_relative 'psych/handlers/document_stream'
require_relative 'psych/class_loader'

###
# = Overview
#
# Psych is a YAML parser and emitter.
# Psych leverages libyaml [Home page: https://pyyaml.org/wiki/LibYAML]
# or [git repo: https://github.com/yaml/libyaml] for its YAML parsing
# and emitting capabilities. In addition to wrapping libyaml, Psych also
# knows how to serialize and de-serialize most Ruby objects to and from
# the YAML format.
#
# = I NEED TO PARSE OR EMIT YAML RIGHT NOW!
#
#   # Parse some YAML
#   Psych.load("--- foo") # => "foo"
#
#   # Emit some YAML
#   Psych.dump("foo")     # => "--- foo\n...\n"
#   { :a => 'b'}.to_yaml  # => "---\n:a: b\n"
#
# Got more time on your hands?  Keep on reading!
#
# == YAML Parsing
#
# Psych provides a range of interfaces for parsing a YAML document ranging from
# low level to high level, depending on your parsing needs.  At the lowest
# level, is an event based parser.  Mid level is access to the raw YAML AST,
# and at the highest level is the ability to unmarshal YAML to Ruby objects.
#
# == YAML Emitting
#
# Psych provides a range of interfaces ranging from low to high level for
# producing YAML documents.  Very similar to the YAML parsing interfaces, Psych
# provides at the lowest level, an event based system, mid-level is building
# a YAML AST, and the highest level is converting a Ruby object straight to
# a YAML document.
#
# == High-level API
#
# === Parsing
#
# The high level YAML parser provided by Psych simply takes YAML as input and
# returns a Ruby data structure.  For information on using the high level parser
# see Psych.load
#
# ==== Reading from a string
#
#   Psych.safe_load("--- a")             # => 'a'
#   Psych.safe_load("---\n - a\n - b")   # => ['a', 'b']
#   # From a trusted string:
#   Psych.load("--- !ruby/range\nbegin: 0\nend: 42\nexcl: false\n") # => 0..42
#
# ==== Reading from a file
#
#   Psych.safe_load_file("data.yml", permitted_classes: [Date])
#   Psych.load_file("trusted_database.yml")
#
# ==== Exception handling
#
#   begin
#     # The second argument changes only the exception contents
#     Psych.parse("--- `", "file.txt")
#   rescue Psych::SyntaxError => ex
#     ex.file    # => 'file.txt'
#     ex.message # => "(file.txt): found character that cannot start any token"
#   end
#
# === Emitting
#
# The high level emitter has the easiest interface.  Psych simply takes a Ruby
# data structure and converts it to a YAML document.  See Psych.dump for more
# information on dumping a Ruby data structure.
#
# ==== Writing to a string
#
#   # Dump an array, get back a YAML string
#   Psych.dump(['a', 'b'])  # => "---\n- a\n- b\n"
#
#   # Dump an array to an IO object
#   Psych.dump(['a', 'b'], StringIO.new)  # => #<StringIO:0x000001009d0890>
#
#   # Dump an array with indentation set
#   Psych.dump(['a', ['b']], :indentation => 3) # => "---\n- a\n-  - b\n"
#
#   # Dump an array to an IO with indentation set
#   Psych.dump(['a', ['b']], StringIO.new, :indentation => 3)
#
# ==== Writing to a file
#
# Currently there is no direct API for dumping Ruby structure to file:
#
#   File.open('database.yml', 'w') do |file|
#     file.write(Psych.dump(['a', 'b']))
#   end
#
# == Mid-level API
#
# === Parsing
#
# Psych provides access to an AST produced from parsing a YAML document.  This
# tree is built using the Psych::Parser and Psych::TreeBuilder.  The AST can
# be examined and manipulated freely.  Please see Psych::parse_stream,
# Psych::Nodes, and Psych::Nodes::Node for more information on dealing with
# YAML syntax trees.
#
# ==== Reading from a string
#
#   # Returns Psych::Nodes::Stream
#   Psych.parse_stream("---\n - a\n - b")
#
#   # Returns Psych::Nodes::Document
#   Psych.parse("---\n - a\n - b")
#
# ==== Reading from a file
#
#   # Returns Psych::Nodes::Stream
#   Psych.parse_stream(File.read('database.yml'))
#
#   # Returns Psych::Nodes::Document
#   Psych.parse_file('database.yml')
#
# ==== Exception handling
#
#   begin
#     # The second argument changes only the exception contents
#     Psych.parse("--- `", "file.txt")
#   rescue Psych::SyntaxError => ex
#     ex.file    # => 'file.txt'
#     ex.message # => "(file.txt): found character that cannot start any token"
#   end
#
# === Emitting
#
# At the mid level is building an AST.  This AST is exactly the same as the AST
# used when parsing a YAML document.  Users can build an AST by hand and the
# AST knows how to emit itself as a YAML document.  See Psych::Nodes,
# Psych::Nodes::Node, and Psych::TreeBuilder for more information on building
# a YAML AST.
#
# ==== Writing to a string
#
#   # We need Psych::Nodes::Stream (not Psych::Nodes::Document)
#   stream = Psych.parse_stream("---\n - a\n - b")
#
#   stream.to_yaml # => "---\n- a\n- b\n"
#
# ==== Writing to a file
#
#   # We need Psych::Nodes::Stream (not Psych::Nodes::Document)
#   stream = Psych.parse_stream(File.read('database.yml'))
#
#   File.open('database.yml', 'w') do |file|
#     file.write(stream.to_yaml)
#   end
#
# == Low-level API
#
# === Parsing
#
# The lowest level parser should be used when the YAML input is already known,
# and the developer does not want to pay the price of building an AST or
# automatic detection and conversion to Ruby objects.  See Psych::Parser for
# more information on using the event based parser.
#
# ==== Reading to Psych::Nodes::Stream structure
#
#   parser = Psych::Parser.new(TreeBuilder.new) # => #<Psych::Parser>
#   parser = Psych.parser                       # it's an alias for the above
#
#   parser.parse("---\n - a\n - b")             # => #<Psych::Parser>
#   parser.handler                              # => #<Psych::TreeBuilder>
#   parser.handler.root                         # => #<Psych::Nodes::Stream>
#
# ==== Receiving an events stream
#
#   recorder = Psych::Handlers::Recorder.new
#   parser = Psych::Parser.new(recorder)
#
#   parser.parse("---\n - a\n - b")
#   recorder.events # => [list of [event, args] lists]
#                   # event is one of: Psych::Handler::EVENTS
#                   # args are the arguments passed to the event
#
# === Emitting
#
# The lowest level emitter is an event based system.  Events are sent to a
# Psych::Emitter object.  That object knows how to convert the events to a YAML
# document.  This interface should be used when document format is known in
# advance or speed is a concern.  See Psych::Emitter for more information.
#
# ==== Writing to a Ruby structure
#
#   Psych.parser.parse("--- a")       # => #<Psych::Parser>
#
#   parser.handler.first              # => #<Psych::Nodes::Stream>
#   parser.handler.first.to_ruby      # => ["a"]
#
#   parser.handler.root.first         # => #<Psych::Nodes::Document>
#   parser.handler.root.first.to_ruby # => "a"
#
#   # You can instantiate an Emitter manually
#   Psych::Visitors::ToRuby.new.accept(parser.handler.root.first)
#   # => "a"

module Psych
  # The version of libyaml Psych is using
  LIBYAML_VERSION = Psych.libyaml_version.join('.').freeze

  ###
  # Load +yaml+ in to a Ruby data structure.  If multiple documents are
  # provided, the object contained in the first document will be returned.
  # +filename+ will be used in the exception message if any exception
  # is raised while parsing.  If +yaml+ is empty, it returns
  # the specified +fallback+ return value, which defaults to +false+.
  #
  # Raises a Psych::SyntaxError when a YAML syntax error is detected.
  #
  # Example:
  #
  #   Psych.unsafe_load("--- a")             # => 'a'
  #   Psych.unsafe_load("---\n - a\n - b")   # => ['a', 'b']
  #
  #   begin
  #     Psych.unsafe_load("--- `", filename: "file.txt")
  #   rescue Psych::SyntaxError => ex
  #     ex.file    # => 'file.txt'
  #     ex.message # => "(file.txt): found character that cannot start any token"
  #   end
  #
  # When the optional +symbolize_names+ keyword argument is set to a
  # true value, returns symbols for keys in Hash objects (default: strings).
  #
  #   Psych.unsafe_load("---\n foo: bar")                         # => {"foo"=>"bar"}
  #   Psych.unsafe_load("---\n foo: bar", symbolize_names: true)  # => {:foo=>"bar"}
  #
  # Raises a TypeError when `yaml` parameter is NilClass
  #
  # NOTE: This method *should not* be used to parse untrusted documents, such as
  # YAML documents that are supplied via user input.  Instead, please use the
  # load method or the safe_load method.
  #
  def self.unsafe_load yaml, filename: nil, fallback: false, symbolize_names: false, freeze: false, strict_integer: false
    result = parse(yaml, filename: filename)
    return fallback unless result
    result.to_ruby(symbolize_names: symbolize_names, freeze: freeze, strict_integer: strict_integer)
  end

  ###
  # Safely load the yaml string in +yaml+.  By default, only the following
  # classes are allowed to be deserialized:
  #
  # * TrueClass
  # * FalseClass
  # * NilClass
  # * Integer
  # * Float
  # * String
  # * Array
  # * Hash
  #
  # Recursive data structures are not allowed by default.  Arbitrary classes
  # can be allowed by adding those classes to the +permitted_classes+ keyword argument.  They are
  # additive.  For example, to allow Date deserialization:
  #
  #   Psych.safe_load(yaml, permitted_classes: [Date])
  #
  # Now the Date class can be loaded in addition to the classes listed above.
  #
  # Aliases can be explicitly allowed by changing the +aliases+ keyword argument.
  # For example:
  #
  #   x = []
  #   x << x
  #   yaml = Psych.dump x
  #   Psych.safe_load yaml               # => raises an exception
  #   Psych.safe_load yaml, aliases: true # => loads the aliases
  #
  # A Psych::DisallowedClass exception will be raised if the yaml contains a
  # class that isn't in the +permitted_classes+ list.
  #
  # A Psych::BadAlias exception will be raised if the yaml contains aliases
  # but the +aliases+ keyword argument is set to false.
  #
  # +filename+ will be used in the exception message if any exception is raised
  # while parsing.
  #
  # When the optional +symbolize_names+ keyword argument is set to a
  # true value, returns symbols for keys in Hash objects (default: strings).
  #
  #   Psych.safe_load("---\n foo: bar")                         # => {"foo"=>"bar"}
  #   Psych.safe_load("---\n foo: bar", symbolize_names: true)  # => {:foo=>"bar"}
  #
  def self.safe_load yaml, permitted_classes: [], permitted_symbols: [], aliases: false, filename: nil, fallback: nil, symbolize_names: false, freeze: false, strict_integer: false
    result = parse(yaml, filename: filename)
    return fallback unless result

    class_loader = ClassLoader::Restricted.new(permitted_classes.map(&:to_s),
                                               permitted_symbols.map(&:to_s))
    scanner      = ScalarScanner.new class_loader, strict_integer: strict_integer
    visitor = if aliases
                Visitors::ToRuby.new scanner, class_loader, symbolize_names: symbolize_names, freeze: freeze
              else
                Visitors::NoAliasRuby.new scanner, class_loader, symbolize_names: symbolize_names, freeze: freeze
              end
    result = visitor.accept result
    result
  end

  ###
  # Load +yaml+ in to a Ruby data structure.  If multiple documents are
  # provided, the object contained in the first document will be returned.
  # +filename+ will be used in the exception message if any exception
  # is raised while parsing.  If +yaml+ is empty, it returns
  # the specified +fallback+ return value, which defaults to +false+.
  #
  # Raises a Psych::SyntaxError when a YAML syntax error is detected.
  #
  # Example:
  #
  #   Psych.load("--- a")             # => 'a'
  #   Psych.load("---\n - a\n - b")   # => ['a', 'b']
  #
  #   begin
  #     Psych.load("--- `", filename: "file.txt")
  #   rescue Psych::SyntaxError => ex
  #     ex.file    # => 'file.txt'
  #     ex.message # => "(file.txt): found character that cannot start any token"
  #   end
  #
  # When the optional +symbolize_names+ keyword argument is set to a
  # true value, returns symbols for keys in Hash objects (default: strings).
  #
  #   Psych.load("---\n foo: bar")                         # => {"foo"=>"bar"}
  #   Psych.load("---\n foo: bar", symbolize_names: true)  # => {:foo=>"bar"}
  #
  # Raises a TypeError when `yaml` parameter is NilClass.  This method is
  # similar to `safe_load` except that `Symbol` objects are allowed by default.
  #
  def self.load yaml, permitted_classes: [Symbol], permitted_symbols: [], aliases: false, filename: nil, fallback: nil, symbolize_names: false, freeze: false, strict_integer: false
    safe_load yaml, permitted_classes: permitted_classes,
                    permitted_symbols: permitted_symbols,
                    aliases: aliases,
                    filename: filename,
                    fallback: fallback,
                    symbolize_names: symbolize_names,
                    freeze: freeze,
                    strict_integer: strict_integer
  end

  ###
  # Parse a YAML string in +yaml+.  Returns the Psych::Nodes::Document.
  # +filename+ is used in the exception message if a Psych::SyntaxError is
  # raised.
  #
  # Raises a Psych::SyntaxError when a YAML syntax error is detected.
  #
  # Example:
  #
  #   Psych.parse("---\n - a\n - b") # => #<Psych::Nodes::Document:0x00>
  #
  #   begin
  #     Psych.parse("--- `", filename: "file.txt")
  #   rescue Psych::SyntaxError => ex
  #     ex.file    # => 'file.txt'
  #     ex.message # => "(file.txt): found character that cannot start any token"
  #   end
  #
  # See Psych::Nodes for more information about YAML AST.
  def self.parse yaml, filename: nil
    parse_stream(yaml, filename: filename) do |node|
      return n