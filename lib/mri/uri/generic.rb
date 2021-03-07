# frozen_string_literal: true

# = uri/generic.rb
#
# Author:: Akira Yamada <akira@ruby-lang.org>
# License:: You can redistribute it and/or modify it under the same term as Ruby.
#
# See URI for general documentation
#

require_relative 'common'
autoload :IPSocket, 'socket'
autoload :IPAddr, 'ipaddr'

module URI

  #
  # Base class for all URI classes.
  # Implements generic URI syntax as per RFC 2396.
  #
  class Generic
    include URI

    #
    # A Default port of nil for URI::Generic.
    #
    DEFAULT_PORT = nil

    #
    # Returns default port.
    #
    def self.default_port
      self::DEFAULT_PORT
    end

    #
    # Returns default port.
    #
    def default_port
      self.class.default_port
    end

    #
    # An Array of the available components for URI::Generic.
    #
    COMPONENT = [
      :scheme,
      :userinfo, :host, :port, :registry,
      :path, :opaque,
      :query,
      :fragment
    ].freeze

    #
    # Components of the URI in the order.
    #
    def self.component
      self::COMPONENT
    end

    USE_REGISTRY = false # :nodoc:

    def self.use_registry # :nodoc:
      self::USE_REGISTRY
    end

    #
    # == Synopsis
    #
    # See ::new.
    #
    # == Description
    #
    # At first, tries to create a new URI::Generic instance using
    # URI::Generic::build. But, if exception URI::InvalidComponentError is raised,
    # then it does URI::Escape.escape all URI components and tries again.
    #
    def self.build2(args)
      begin
        return self.build(args)
      rescue InvalidComponentError
        if args.kind_of?(Array)
          return self.build(args.collect{|x|
            if x.is_a?(String)
              DEFAULT_PARSER.escape(x)
            else
              x
            end
          })
        elsif args.kind_of?(Hash)
          tmp = {}
          args.each do |key, value|
            tmp[key] = if value
                DEFAULT_PARSER.escape(value)
              else
                value
              end
          end
          return self.build(tmp)
        end
      end
    end

    #
    # == Synopsis
    #
    # See ::new.
    #
    # == Description
    #
    # Creates a new URI::Generic instance from components of URI::Generic
    # with check.  Components are: scheme, userinfo, host, port, registry, path,
    # opaque, query, and fragment. You can provide arguments either by an Array or a Hash.
    # See ::new for hash keys to use or for order of array items.
    #
    def self.build(args)
      if args.kind_of?(Array) &&
          args.size == ::URI::Generic::COMPONENT.size
        tmp = args.dup
      elsif args.kind_of?(Hash)
        tmp = ::URI::Generic::COMPONENT.collect do |c|
          if args.include?(c)
            args[c]
          else
            nil
          end
        end
      else
        component = self.class.component rescue ::URI::Generic::COMPONENT
        raise ArgumentError,
        "expected Array of or Hash of components of #{self.class} (#{component.join(', ')})"
      end

      tmp << nil
      tmp << true
      return self.new(*tmp)
    end

    #
    # == Args
    #
    # +scheme+::
    #   Protocol scheme, i.e. 'http','ftp','mailto' and so on.
    # +userinfo+::
    #   User name and password, i.e. 'sdmitry:bla'.
    # +host+::
    #   Server host name.
    # +port+::
    #   Server port.
    # +registry+::
    #   Registry of naming authorities.
    # +path+::
    #   Path on server.
    # +opaque+::
    #   Opaque part.
    # +query+::
    #   Query data.
    # +fragment+::
    #   Part of the URI after '#' character.
    # +parser+::
    #   Parser for internal use [URI::DEFAULT_PARSER by default].
    # +arg_check+::
    #   Check arguments [false by default].
    #
    # == Description
    #
    # Creates a new URI::Generic instance from ``generic'' components without check.
    #
    def initialize(scheme,
                   userinfo, host, port, registry,
                   path, opaque,
                   query,
                   fragment,
                   parser = DEFAULT_PARSER,
                   arg_check = false)
      @scheme = nil
      @user = nil
      @password = nil
      @host = nil
      @port = nil
      @path = nil
      @query = nil
      @opaque = nil
      @fragment = nil
      @parser = parser == DEFAULT_PARSER ? nil : parser

      if arg_check
        self.scheme = scheme
        self.userinfo = userinfo
        self.hostname = host
        self.port = port
        self.path = path
        self.query = query
        self.opaque = opaque
        self.fragment = fragment
      else
        self.set_scheme(scheme)
        self.set_userinfo(userinfo)
        self.set_host(host)
        self.set_port(port)
        self.set_path(path)
        self.query = query
        self.set_opaque(opaque)
        self.fragment=(fragment)
      end
      if registry
        raise InvalidURIError,
          "the scheme #{@scheme} does not accept registry part: #{registry} (or bad hostname?)"
      end

      @scheme&.freeze
      self.set_path('') if !@path && !@opaque # (see RFC2396 Section 5.2)
      self.set_port(self.default_port) if self.default_port && !@port
    end

    #
    # Returns the scheme component of the URI.
    #
    #   URI("http://foo/bar/baz").scheme #=> "http"
    #
    attr_reader :scheme

    # Returns the host component of the URI.
    #
    #   URI("http://foo/bar/baz").host #=> "foo"
    #
    # It returns nil if no host component exists.
    #
    #   URI("mailto:foo@example.org").host #=> nil
    #
    # The component does not contain the port number.
    #
    #   URI("http://foo:8080/bar/baz").host #=> "foo"
    #
    # Since IPv6 addresses are wrapped with brackets in URIs,
    # this method returns IPv6 addresses wrapped with brackets.
    # This form is not appropriate to pass to socket methods such as TCPSocket.open.
    # If unwrapped host names are required, use the #hostname method.
    #
    #   URI("http://[::1]/bar/baz").host     #=> "[::1]"
    #   URI("http://[::1]/bar/baz").hostname #=> "::1"
    #
    attr_reader :host

    # Returns the port component of the URI.
    #
    #   URI("http://foo/bar/baz").port      #=> 80
    #   URI("http://foo:8080/bar/baz").port #=> 8080
    #
    attr_reader :port

    def registry # :nodoc:
      nil
    end

    # Returns the path component of the URI.
    #
    #   URI("http://foo/bar/baz").path #=> "/bar/baz"
    #
    attr_reader :path

    # Returns the query component of the URI.
    #
    #   URI("http://foo/bar/baz?search=FooBar").query #=> "search=FooBar"
    #
    attr_reader :query

    # Returns the opaque part of the URI.
    #
    #   URI("mailto:foo@example.org").opaque #=> "foo@example.org"
    #   URI("http://foo/bar/baz").opaque     #=> nil
    #
    # The portion of the path that does not make use of the slash '/'.
    # The path typically refers to an absolute path or an opaque part.
    # (See RFC2396 Section 3 and 5.2.)
    #
    attr_reader :opaque

    # Returns the fragment component of the URI.
    #
    #   URI("http://foo/bar/baz?search=FooBar#ponies").fragment #=> "ponies"
    #
    attr_reader :fragment

    # Returns the parser to be used.
    #
    # Unless a URI::Parser is defined, DEFAULT_PARSER is used.
    #
    def parser
      if !defined?(@parser) || !@parser
        DEFAULT_PARSER
      else
        @parser || DEFAULT_PARSER
      end
    end

    # Replaces self by other URI object.
    #
    def replace!(oth)
      if self.class != oth.class
        raise ArgumentError, "expected #{self.class} object"
      end

      component.each do |c|
        self.__send__("#{c}=", oth.__send__(c))
      end
    end
    private :replace!

    #
    # Components of the URI in the order.
    #
    def component
      self.class.component
    end

    #
    # Checks the scheme +v+ component against the URI::Parser Regexp for :SCHEME.
    #
    de