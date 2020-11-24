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
# Got more time on yo