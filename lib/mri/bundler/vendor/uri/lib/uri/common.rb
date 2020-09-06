# frozen_string_literal: true
#--
# = uri/common.rb
#
# Author:: Akira Yamada <akira@ruby-lang.org>
# License::
#   You can redistribute it and/or modify it under the same term as Ruby.
#
# See Bundler::URI for general documentation
#

require_relative "rfc2396_parser"
require_relative "rfc3986_parser"

module Bundler::URI
  REGEXP = RFC2396_REGEXP
  Parser = RFC2396_Parser
  RFC3986_PARSER = RFC3986_Parser.new

  # Bundler::URI::Parser.new
  DEFAULT_PARSER = Parser.new
  DEFAULT_PARSER.pattern.each_pair do |sym, str|
    unless REGEXP::PATTERN.const_defined?(sym)
      REGEXP::PATTERN.const_set(sym, str)
    end
  end
  DEFAULT_PARSER.regexp.each_pair do |sym, str|
    const_set(sym, str)
  end

  module Util # :nodoc:
    def make_components_hash(klass, array_hash)
      tmp = {}
      if array_hash.kind_of?(Array) &&
          array_hash.size == klass.component.size - 1
        klass.component[1..-1].each_index do |i|
          begin
            tmp[klass.component[i + 1]] = array_hash[i].clone
          rescue TypeError
            tmp[klass.component[i + 1]] = array_hash[i]
          end
        end

      elsif array_hash.kind_of?(Hash)
        array_hash.each do |key, value|
          begin
            tmp[key] = value.clone
          rescue TypeError
            tmp[key] = value
          end
        end
      else
        raise ArgumentError,
          "expected Array of or Hash of components of #{klass} (#{klass.component[1..-1].join(', ')})"
      end
      tmp[:scheme] = klass.to_s.sub(/\A.*::/, '').downcase

      return tmp
    end
    module_function :make_components_hash
  end

  include REGEXP

  @@schemes = {}
  # Returns a Hash of the defined schemes.
  def self.scheme_list
    @@schemes
  end

  #
  # Construct a Bundler::URI instance, using the scheme to detect the appropriate class
  # from +Bundler::URI.scheme_list+.
  #
  def self.for(scheme, *arguments, default: Generic)
    if scheme
      uri_class = @@schemes[scheme.upcase] || default
    else
      uri_class = default
    end

    return uri_class.new(scheme, *arguments)
  end

  #
  # Base class for all Bundler::URI exceptions.
  #
  class Error < StandardError; end
  #
  # Not a Bundler::URI.
  #
  class InvalidURIError < Error; end
  #
  # Not a Bundler::URI component.
  #
  class InvalidComponentError < Error; end
  #
  # Bundler::URI is valid, bad usage is not.
  #
  class BadURIError < Error; end

  #
  # == Synopsis
  #
  #   Bundler::URI::split(uri)
  #
  # == Args
  #
  # +uri+::
  #   String with Bundler::URI.
  #
  # == Description
  #
  # Splits the string on following parts and returns array with result:
  #
  # * Scheme
  # * Userinfo
  # * Host
  # * Port
  # * Registry
  # * Path
  # * Opaque
  # * Query
  # * Fragment
  #
  # == Usage
  #
  #   require 'bundler/vendor/uri/lib/uri'
  #
  #   Bundler::URI.split("http://www.ruby-lang.org/")
  #   # => ["http", nil, "www.ruby-lang.org", nil, nil, "/", nil, nil, nil]
  #
  def self.split(uri)
    RFC3986_PARSER.split(uri)
  end

  #
  # == Synopsis
  #
  #   Bundler::URI::parse(uri_str)
  #
  # == Args
  #
  # +uri_str+::
  #   String with Bundler::URI.
  #
  # == Description
  #
  # Creates one of the Bundler::URI's subclasses instance from the string.
  #
  # == Raises
  #
  # Bundler::URI::InvalidURIError::
  #   Raised if Bundler::URI given is not a correct one.
  #
  # == Usage
  #
  #   require 'bundler/vendor/uri/lib/uri'
  #
  #   uri = Bundler::URI.parse("http://www.ruby-lang.org/")
  #   # => #<Bundler::URI::HTTP http://www.ruby-lang.org/>
  #   uri.scheme
  #   # => "http"
  #   uri.host
  #   # => "www.ruby-lang.org"
  #
  # It's recommended to first ::escape the provided +uri_str+ if there are any
  # invalid Bundler::URI characters.
  #
  def self.parse(