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
  def self.parse(uri)
    RFC3986_PARSER.parse(uri)
  end

  #
  # == Synopsis
  #
  #   Bundler::URI::join(str[, str, ...])
  #
  # == Args
  #
  # +str+::
  #   String(s) to work with, will be converted to RFC3986 URIs before merging.
  #
  # == Description
  #
  # Joins URIs.
  #
  # == Usage
  #
  #   require 'bundler/vendor/uri/lib/uri'
  #
  #   Bundler::URI.join("http://example.com/","main.rbx")
  #   # => #<Bundler::URI::HTTP http://example.com/main.rbx>
  #
  #   Bundler::URI.join('http://example.com', 'foo')
  #   # => #<Bundler::URI::HTTP http://example.com/foo>
  #
  #   Bundler::URI.join('http://example.com', '/foo', '/bar')
  #   # => #<Bundler::URI::HTTP http://example.com/bar>
  #
  #   Bundler::URI.join('http://example.com', '/foo', 'bar')
  #   # => #<Bundler::URI::HTTP http://example.com/bar>
  #
  #   Bundler::URI.join('http://example.com', '/foo/', 'bar')
  #   # => #<Bundler::URI::HTTP http://example.com/foo/bar>
  #
  def self.join(*str)
    RFC3986_PARSER.join(*str)
  end

  #
  # == Synopsis
  #
  #   Bundler::URI::extract(str[, schemes][,&blk])
  #
  # == Args
  #
  # +str+::
  #   String to extract URIs from.
  # +schemes+::
  #   Limit Bundler::URI matching to specific schemes.
  #
  # == Description
  #
  # Extracts URIs from a string. If block given, iterates through all matched URIs.
  # Returns nil if block given or array with matches.
  #
  # == Usage
  #
  #   require "bundler/vendor/uri/lib/uri"
  #
  #   Bundler::URI.extract("text here http://foo.example.org/bla and here mailto:test@example.com and here also.")
  #   # => ["http://foo.example.com/bla", "mailto:test@example.com"]
  #
  def self.extract(str, schemes = nil, &block)
    warn "Bundler::URI.extract is obsolete", uplevel: 1 if $VERBOSE
    DEFAULT_PARSER.extract(str, schemes, &block)
  end

  #
  # == Synopsis
  #
  #   Bundler::URI::regexp([match_schemes])
  #
  # == Args
  #
  # +match_schemes+::
  #   Array of schemes. If given, resulting regexp matches to URIs
  #   whose scheme is one of the match_schemes.
  #
  # == Description
  #
  # Returns a Regexp object which matches to Bundler::URI-like strings.
  # The Regexp object returned by this method includes arbitrary
  # number of capture group (parentheses).  Never rely on its number.
  #
  # == Usage
  #
  #   require 'bundler/vendor/uri/lib/uri'
  #
  #   # extract first Bundler::URI from html_string
  #   html_string.slice(Bundler::URI.regexp)
  #
  #   # remove ftp URIs
  #   html_string.sub(Bundler::URI.regexp(['ftp']), '')
  #
  #   # You should not rely on the number of parentheses
  #   html_string.scan(Bundler::URI.regexp) do |*matches|
  #     p $&
  #   end
  #
  def self.regexp(schemes = nil)
    warn "Bundler::URI.regexp is obsolete", uplevel: 1 if $VERBOSE
    DEFAULT_PARSER.make_regexp(schemes)
  end

  TBLENCWWWCOMP_ = {} # :nodoc:
  256.times do |i|
    TBLENCWWWCOMP_[-i.chr] = -('%%%02X' % i)
  end
  TBLENCWWWCOMP_[' '] = '+'
  TBLENCWWWCOMP_.freeze
  TBLDECWWWCOMP_ = {} # :nodoc:
  256.times do |i|
    h, l = i>>4, i&15
    TBLDECWWWCOMP_[-('%%%X%X' % [h, l])] = -i.chr
    TBLDECWWWCOMP_[-('%%%x%X' % [h, l])] = -i.chr
    TBLDECWWWCOMP_[-('%%%X%x' % [h, l])] = -i.chr
    TBLDECWWWCOMP_[-('%%%x%x' % [h, l])] = -i.chr
  end
  TBLDECWWWCOMP_['+'] = ' '
  TBLDECWWWCOMP_.freeze

  # Encodes given +str+ to URL-encoded form data.
  #
  # This method doesn't convert *, -, ., 0-9, A-Z, _, a-z, but does convert SP
  # (ASCII space) to + and converts others to %XX.
  #
  # If +enc+ is given, convert +str+ to the encoding before percent encoding.
  #
  # This is an implementation of
  # https://www.w3.org/TR/2013/CR-html5-20130806/forms.html#url-encoded-form-data.
  #
  # See Bundler::URI.decode_www_form_component, Bundler::URI.encode_www_form.
  def self.encode_www_form_component(str, enc=nil)
    str = str.to_s.dup
    if str.encoding != Encoding::ASCII_8BIT
      if enc && enc != Encoding::ASCII_8BIT
        str.encode!(Encoding::UTF_8, invalid: :replace, undef: :replace)
        str.encode!(enc, fallback: ->(x){"&##{x.ord};"})
      end
      str.force_encoding(Encoding::ASCII_8BIT)
    end
    str.gsub!(/[^*\-.0-9A-Z_a-z]/, TBLENCWWWCOMP_)
    str.force_encoding(Encoding::US_ASCII)
  end

  # Decodes given +str+ of URL-encoded form data.
  #
  # This decodes + to SP.
  #
  # See Bundler::URI.encode_www_form_component, Bundler::URI.decode_www_form.
  def self.decode_www_form_component(str, enc=Encoding::UTF_8)
    raise ArgumentError, "invalid %-encoding (#{str})" if /%(?!\h\h)/ =~ str
    str.b.gsub(/\+|%\h\h/, TBLDECWWWCOMP_).force_encoding(enc)
  end

  # Generates URL-encoded form data from given +enum+.
  #
  # This generates application/x-www-form-urlencoded data defined in HTML5
  # from given an Enumerable object.
  #
  # This internally uses Bundler::URI.encode_www_form_component(str).
  #
  # This method doesn't convert the encoding of given items, so convert them
  # before calling this method if you want to send data as other than original
  # encoding or mixed encoding data. (Strings which are encoded in an HTML5
  # ASCII incompatible encoding are converted to UTF-8.)
  #
  # This method doesn't handle files.  When you send a file, use
  # multipart/form-data.
  #
  # This refers https://url.spec.whatwg.org/#concept-urlencoded-serializer
  #
  #    Bundler::URI.encode_www_form([["q", "ruby"], ["lang", "en"]])
  #    #=> "q=ruby&lang=en"
  #    Bundler::URI.encode_www_form("q" => "ruby", "lang" => "en")
  #    #=> "q=ruby&lang=en"
  #    Bundler::URI.encode_www_form("q" => ["ruby", "perl"], "lang" => "en")
  #    #=> "q=ruby&q=perl&lang=en"
  #    Bundler::URI.encode_www_form([["q", "ruby"], ["q", "perl"], ["lang", "en"]])
  #    #=> "q=ruby&q=perl&lang=en"
  #
  # See Bundler::URI.encode_www_form_component, Bundler::URI.decode_www_form.
  def self.encode_www_form(enum, enc=nil)
    enum.map do |k,v|
      if v.nil?
        encode_www_form_component(k, enc)
      elsif v.respond_to?(:to_ary)
        v.to_ary.map do |w|
          str = encode_www_form_component(k, enc)
          unless w.nil?
            str << '='
            str << encode_www_form_component(w, enc)
          end
        end.join('&')
      else
        str = encode_www_form_component(k, enc)
        str << '='
        str << encode_www_form_component(v, enc)
      end
    end.join('&')
  end

  # Decodes URL-encoded form data from given +str+.
  #
  # This decodes application/x-www-form-urlencoded data
  # and returns an array of key-value arrays.
  #
  # This refers http://url.spec.whatwg.org/#concept-urlencoded-parser,
  # so this supports only &-separator, and doesn't support ;-separator.
  #
