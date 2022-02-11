# -*- encoding: binary -*-
# frozen_string_literal: true

require 'uri'
require 'fileutils'
require 'set'
require 'tempfile'
require 'time'

require_relative 'query_parser'

module Rack
  # Rack::Utils contains a grab-bag of useful methods for writing web
  # applications adopted from all kinds of Ruby libraries.

  module Utils
    (require_relative 'core_ext/regexp'; using ::Rack::RegexpExtensions) if RUBY_VERSION < '2.4'

    ParameterTypeError = QueryParser::ParameterTypeError
    InvalidParameterError = QueryParser::InvalidParameterError
    DEFAULT_SEP = QueryParser::DEFAULT_SEP
    COMMON_SEP = QueryParser::COMMON_SEP
    KeySpaceConstrainedParams = QueryParser::Params

    class << self
      attr_accessor :default_query_parser
    end
    # The default number of bytes to allow parameter keys to take up.
    # This helps prevent a rogue client from flooding a Request.
    self.default_query_parser = QueryParser.make_default(65536, 100)

    module_function

    # URI escapes. (CGI style space to +)
    def escape(s)
      URI.encode_www_form_component(s)
    end

    # Like URI escaping, but with %20 instead of +. Strictly speaking this is
    # true URI escaping.
    def escape_path(s)
      ::URI::DEFAULT_PARSER.escape s
    end

    # Unescapes the **path** component of a URI.  See Rack::Utils.unescape for
    # unescaping query parameters or form components.
    def unescape_path(s)
      ::URI::DEFAULT_PARSER.unescape s
    end

    # Unescapes a URI escaped string with +encoding+. +encoding+ will be the
    # target encoding of the string returned, and it defaults to UTF-8
    def unescape(s, encoding = Encoding::UTF_8)
      URI.decode_www_form_component(s, encoding)
    end

    class << self
      attr_accessor :multipart_part_limit
    end

    # The maximum number of parts a request can contain. Accepting too many part
    # can lead to the server running out of file handles.
    # Set to `0` for no limit.
    self.multipart_part_limit = (ENV['RACK_MULTIPART_PART_LIMIT'] || 128).to_i

    def self.param_depth_limit
      default_query_parser.param_depth_limit
    end

    def self.param_depth_limit=(v)
      self.default_query_parser = self.default_query_parser.new_depth_limit(v)
    end

    def self.key_space_limit
      default_query_parser.key_space_limit
    end

    def self.key_space_limit=(v)
      self.default_query_parser = self.default_query_parser.new_space_limit(v)
    end

    if defined?(Process::CLOCK_MONOTONIC)
      def clock_time
        Process.clock_gettime(Process::CLOCK_MONOTONIC)
      end
    else
      # :nocov:
      def clock_time
        Time.now.to_f
      end
      # :nocov:
    end

    def parse_query(qs, d = nil, &unescaper)
      Rack::Utils.default_query_parser.parse_query(qs, d, &unescaper)
    end

    def parse_nested_query(qs, d = nil)
      Rack::Utils.default_query_parser.parse_nested_query(qs, d)
    end

    def build_query(params)
      params.map { |k, v|
        if v.class == Array
          build_query(v.map { |x| [k, x] })
        else
          v.nil? ? escape(k) : "#{escape(k)}=#{escape(v)}"
        end
      }.join("&")
    end

    def build_nested_query(value, prefix = nil)
      case value
      when Array
        value.map { |v|
          build_nested_query(v, "#{prefix}[]")
        }.join("&")
      when Hash
        value.map { |k, v|
          build_nested_query(v, prefix ? "#{prefix}[#{escape(k)}]" : escape(k))
        }.delete_if(&:empty?).join('&')
      when nil
        prefix
      else
        raise ArgumentError, "value must be a Hash" if prefix.nil?
        "#{prefix}=#{escape(value)}"
      end
    end

    def q_values(q_value_header)
      q_value_header.to_s.split(/\s*,\s*/).map do |part|
        value, parameters = part.split(/\s*;\s*/, 2)
        quality = 1.0
        if parameters && (md = /\Aq=([\d.]+)/.match(parameters))
          quality = md[1].to_f
        end
        [value, quality]
      end
    end

    # Return best accept value to use, based on the algorithm
    # in RFC 2616 Section 14.  If there are multiple best
    # matches (same specificity and quality), the value returned
    # is arbitrary.
    def best_q_match(q_value_header, available_mimes)
      values = q_values(q_value_header)

      matches = values.map do |req_mime, quality|
        match = available_mimes.find { |am| Rack::Mime.match?(am, req_mime) }
        next unless match
        [match, quality]
      end.compact.sort_by do |match, quality|
        (match.split('/', 2).count('*') * -10) + quality
      end.last
      matches && matches.first
    end

    ESCAPE_HTML = {
      "&" => "&amp;",
      "<" => "&lt;",
      ">" => "&gt;",
      "'" => "&#x27;",
      '"' => "&quot;",
      "/" => "&#x2F;"
    }

    ESCAPE_HTML_PATTERN = Regexp.union(*ESCAPE_HTML.keys)

    # Escape ampersands, brackets and quotes to their HTML/XML entities.
    def escape_html(string)
      string.to_s.gsub(ESCAPE_HTML_PATTERN){|c| ESCAPE_HTML[c] }
    end

    def select_best_encoding(available_encodings, accept_encoding)
      # http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html

      expanded_accept_encoding = []

      accept_encoding.each do |m, q|
        preference = available_encodings.index(m) || available_encodings.size

        if m == "*"
          (available_encodings - accept_encoding.map(&:first)).each do |m2|
            expanded_accept_encoding << [m2, q, preference]
          end
        else
          expanded_accept_encoding << [m, q, preference]
        end
      end

      encoding_candidates = expanded_accept_encoding
        .sort_by { |_, q, p| [-q, p] }
        .map!(&:first)

      unless encoding_candidates.include?("identity")
        encoding_candidates.push("identity")
      end

      expanded_accept_encoding.each do |m, q|
        encoding_candidates.delete(m) if q == 0.0
      end

      (encoding_candidates & available_encodings)[0]
    end

    def parse_cookies(env)
      parse_cookies_header env[HTTP_COOKIE]
    end

    def parse_cookies_header(header)
      # According to RFC 6265:
      # The syntax for cookie headers only supports semicolons
      # User Agent -> Server ==
      # Cookie: SID=31d4d96e407aad42; lang=en-US
      return {} unless header
      header.split(/[;] */n).each_with_object({}) do |cookie, cookies|
        next if cookie.empty?
        key, value =