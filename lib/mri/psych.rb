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
  # +filename+ will 