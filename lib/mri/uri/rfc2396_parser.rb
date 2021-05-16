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
    #   * :DOMLABEL (URI