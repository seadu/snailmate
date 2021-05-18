# frozen_string_literal: false
#--
# = uri/common.rb
#
# Author:: Akira Yamada <akira@ruby-lang.org>
# License::
#   You can redistribute it and/or modify it under the same term as Ruby.
#
# See URI for general documentation
#

module URI
  #
  # Includes URI::REGEXP::PATTERN
  #
  module RFC2396_REGEXP
    #
    # Patterns used to parse URI's
    #
    module PATTERN
      # :stopdoc:

      # RFC 2396 (URI Generic Syntax)
      # RFC 2732 (IPv6 Literal Addresses in URL's)
      # RFC 2373 (IPv6 Addressing Architecture)

      # alpha         = lowalpha | upalpha
      ALPHA = "a-zA-Z"
      # alphanum      = alpha | digit
      ALNUM = "#{ALPHA}\\d"

      # hex           = digit | "A" | "B" | "C" | "D" | "E" | "F" |
      #                         "a" | "b" | "c" | "d" | "e" | "f"
      HEX     = "a-fA-F\\d"
      # escaped       = "%" hex hex
      ESCAPED = "%[#{HEX}]{2}"
      # mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |
      #                 "(" | ")"
      # unreserved    = alphanum | mark
      UNRESERVED = "\\-_.!~*'()#{ALNUM}"
      # reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
      #                 "$" | ","
      # reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
      #                 "$" | "," | "[" | "]" (RFC 2732)
      RESERVED = ";/?:@&=+$,\\[\\]"

      # domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
      DOMLABEL = "(?:[#{ALNUM}](?:[-#{ALNUM}]*[#{ALNUM}])?)"
      # toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
      TOPLABEL = "(?:[#{ALPHA}](?:[-#{ALNUM}]*[#{ALNUM}])?)"
      # hostname      = *( domainlabel "." ) toplabel [ "." ]
      HOSTNAME = "(?:#{DOMLABEL}\\.)*#{TOPLABEL}\\.?"

      # :startdoc:
    end # PATTERN

    # :startdoc:
  end # REGEXP

  # Class that parses String's into URI's.
  #
  # It contains a Hash set of patterns and Regexp's that match and validate.
  #
  class RFC2396_Parser
    include RFC2396_REGEXP

    #
    # == Synopsis
    #
    #   URI::Parser.new([opts])
    #
    # == Args
    #
    # The constructor accepts a hash as options for parser.
    # Keys of options are pattern names of URI components
    # and values of options are pattern strings.
    # The constructor generates set of regexps for parsing URIs.
    #
    # You can use the following keys:
    #
    #   * :ESCAPED (URI::PATTERN::ESCAPED in default)
    #   * :UNRESERVED (URI::PATTERN::UNRESERVED in default)
    #   * :DOMLABEL (URI::PATTERN::DOMLABEL in default)
    #   * :TOPLABEL (URI::PATTERN::TOPLABEL in default)
    #   * :HOSTNAME (URI::PATTERN::HOSTNAME in default)
    #
    # == Examples
    #
    #   p = URI::Parser.new(:ESCAPED => "(?:%[a-fA-F0-9]{2}|%u[a-fA-F0-9]{4})")
    #   u = p.parse("http://example.jp/%uABCD") #=> #<URI::HTTP http://example.jp/%uABCD>
    #   URI.parse(u.to_s) #=> raises URI::InvalidURIError
    #
    #   s = "http://example.com/ABCD"
    #   u1 = p.parse(s) #=> #<URI::HTTP http://example.com/ABCD>
    #   u2 = URI.parse(s) #=> #<URI::HTTP http://example.com/ABCD>
    #   u1 == u2 #=> true
    #   u1.eql?(u2) #=> false
    #
    def initialize(opts = {})
      @pattern = initialize_pattern(opts)
      @pattern.each_value(&:freeze)
      @pattern.freeze

      @regexp = initialize_regexp(@pattern)
      @regexp.each_value(&:freeze)
      @regexp.freeze
    end

    # The Hash of patterns.
    #
    # See also URI::Parser.initialize_pattern.
    attr_reader :pattern

    # The Hash of Regexp.
    #
    # See also URI::Parser.initialize_regexp.
    attr_reader :regexp

    # Returns a split URI against +regexp[:ABS_URI]+.
    def split(uri)
      case uri
      when ''
        # null uri

      when @regexp[:ABS_URI]
        scheme, opaque, userinfo, host, port,
          registry, path, query, fragment = $~[1..-1]

        # URI-reference = [ absoluteURI | relativeURI ] [ "#" fragment ]

        # absoluteURI   = scheme ":" ( hier_part | opaque_part )
        # hier_part     = ( net_path | abs_path ) [ "?" query ]
        # opaque_part   = uric_no_slash *uric

        # abs_path      = "/"  path_segments
        # net_path      = "//" authority [ abs_path ]

        # authority     = server | reg_name
        # server        = [ [ userinfo "@" ] hostport ]

        if !scheme
          raise InvalidURIError,
            "bad URI(absolute but no scheme): #{uri}"
        end
        if !opaque && (!path && (!host && !registry))
          raise InvalidURIError,
            "bad URI(absolute but no path): #{uri}"
        end

      when @regexp[:REL_URI]
        scheme = nil
        opaque = nil

        userinfo, host, port, registry,
          rel_segment, abs_path, query, fragment = $~[1..-1]
        if rel_segment && abs_path
          path = rel_segment + abs_path
        elsif rel_segment
          path = rel_segment
        elsif abs_path
          path = abs_path
        end

        # URI-reference = [ absoluteURI | relativeURI ] [ "#" fragment ]

        # relativeURI   = ( net_path | abs_path | rel_path ) [ "?" query ]

        # net_path      = "//" authority [ abs_path ]
        # abs_path      = "/"  path_segments
        # rel_path      = rel_segment [ abs_path ]

        # authority     = server | reg_name
        # server        = [ [ userinfo "@" ] hostport ]

      else
        raise InvalidURIError, "bad URI(is not URI?): #{uri}"
      end

      path = '' if !path && !opaque # (see RFC2396 Section 5.2)
      ret = [
        scheme,
        userinfo, host, port,         # X
        registry,                     # X
        path,                         # Y
        opaque,                       # Y
        query,
        fragment
      ]
      return ret
    end

    #
    # == Args
    #
    # +uri+::
    #    String
    #
    # == Description
    #
    # Parses +uri+ and constructs either matching URI scheme object
    # (File, FTP, HTTP, HTTPS, LDAP, LDAPS, or MailTo) or URI::Generic.
    #
    # == Usage
    #
    #   p = URI::Parser.new
    #   p.parse("ldap://ldap.example.com/dc=example?user=john")
    #   #=> #<URI::LDAP ldap://ldap.example.com/dc=example?user=john>
    #
    def parse(uri)
      URI.for(*self.split(uri), self)
    end

    #
    # == Args
    #
    # +uris+::
    #    an Array of Strings
    #
    # == Description
    #
    # Attempts to parse and merge a set of URIs.
    #
    def join(*uris)
      uris[0] = convert_to_uri(uris[0])
      uris.inject :merge
    end

    #
    # :call-seq:
    #   extract( str )
    #   extract( str, schemes )
    #   extract( str, schemes ) {|item| block }
    #
    # == Args
    #
    # +str+::
    #    String to search
    # +schemes+::
    #    Patterns to apply to +str+
    #
    # == Description
    #
    # Attempts to parse and merge a set of URIs.
    # If no +block+ given, then returns the result,
    # else it calls +block+ for each element in result.
    #
    # See also URI::Parser.make_regexp.
    #
    def extract(str, schemes = nil)
      if block_given?
        str.scan(make_regexp(schemes)) { yield $& }
        nil
      else
        result = []
        str.scan(make_regexp(schemes)) { result.push $& }
        result
      end
    end

    # Returns Regexp that is default +self.regexp[:ABS_URI_REF]+,
    # unless +schemes+ is provided. Then it is a Regexp.union with +self.pattern[:X_ABS_URI]+.
    def make_regexp(schemes = nil)
      unless schemes
        @regexp[:ABS_URI_REF]
      else
        /(?=#{Regexp.union(*schemes)}:)#{@pattern[:X_ABS_URI]}/x
      end
    end

    #
    # :call-seq:
    #   escape( str )
    #   escape( str, unsafe )
    #
    # == Args
    #
    # +str+::
    #    String to make safe
    # +unsafe+::
    #    Regexp to apply. Defaults to +self.regexp[:UNSAFE]+
    #
    # == Description
    #
    # Constructs a safe String from +str+, removing unsafe characters,
    # replacing them with codes.
    #
    def escape(str, unsafe = @regexp[:UNSAFE])
      unless unsafe.kind_of?(Regexp)
        # perhaps unsafe is String object
        unsafe = Regexp.new("[#{Regexp.quote(unsafe)}]", false)
      end
      str.gsub(unsafe) do
        us = $&
        tmp = ''
        us.each_byte do |uc|
          tmp << sprintf('%%%02X', uc)
        end
        tmp
      end.force_encoding(Encoding::US_ASCII)
    end

    #
    # :call-seq:
    #   unescape( str )
    #   unescape( str, escaped )
    #
    # == Args
    #
    # +str+::
    #    String to remove escapes from
    # +escaped+::
    #    Regexp to apply. Defaults to +self.regexp[:ESCAPED]+
    #
    # == Description
    #
    # Removes escapes from +str+.
    #
    def unescape(str, escaped = @regexp[:ESCAPED])
      enc = str.encoding
      enc = Encoding::UTF_8 if enc == Encoding::US_ASCII
      str.gsub(escaped) { [$&[1, 2]].pack('H2').force_encoding(enc) }
    end

    @@to_s = Kernel.instance_method(:to_s)
    if @@to_s.respond_to?(:bind_call)
      def inspect
        @@to_s.bind_call(self)
      end
    else
      def inspect
        @@to_s.bind(self).call
      end
    end

    private

    # Constructs the default Hash of patterns.
 