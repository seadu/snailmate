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
    def check_scheme(v)
      if v && parser.regexp[:SCHEME] !~ v
        raise InvalidComponentError,
          "bad component(expected scheme component): #{v}"
      end

      return true
    end
    private :check_scheme

    # Protected setter for the scheme component +v+.
    #
    # See also URI::Generic.scheme=.
    #
    def set_scheme(v)
      @scheme = v&.downcase
    end
    protected :set_scheme

    #
    # == Args
    #
    # +v+::
    #    String
    #
    # == Description
    #
    # Public setter for the scheme component +v+
    # (with validation).
    #
    # See also URI::Generic.check_scheme.
    #
    # == Usage
    #
    #   require 'uri'
    #
    #   uri = URI.parse("http://my.example.com")
    #   uri.scheme = "https"
    #   uri.to_s  #=> "https://my.example.com"
    #
    def scheme=(v)
      check_scheme(v)
      set_scheme(v)
      v
    end

    #
    # Checks the +user+ and +password+.
    #
    # If +password+ is not provided, then +user+ is
    # split, using URI::Generic.split_userinfo, to
    # pull +user+ and +password.
    #
    # See also URI::Generic.check_user, URI::Generic.check_password.
    #
    def check_userinfo(user, password = nil)
      if !password
        user, password = split_userinfo(user)
      end
      check_user(user)
      check_password(password, user)

      return true
    end
    private :check_userinfo

    #
    # Checks the user +v+ component for RFC2396 compliance
    # and against the URI::Parser Regexp for :USERINFO.
    #
    # Can not have a registry or opaque component defined,
    # with a user component defined.
    #
    def check_user(v)
      if @opaque
        raise InvalidURIError,
          "can not set user with opaque"
      end

      return v unless v

      if parser.regexp[:USERINFO] !~ v
        raise InvalidComponentError,
          "bad component(expected userinfo component or user component): #{v}"
      end

      return true
    end
    private :check_user

    #
    # Checks the password +v+ component for RFC2396 compliance
    # and against the URI::Parser Regexp for :USERINFO.
    #
    # Can not have a registry or opaque component defined,
    # with a user component defined.
    #
    def check_password(v, user = @user)
      if @opaque
        raise InvalidURIError,
          "can not set password with opaque"
      end
      return v unless v

      if !user
        raise InvalidURIError,
          "password component depends user component"
      end

      if parser.regexp[:USERINFO] !~ v
        raise InvalidComponentError,
          "bad password component"
      end

      return true
    end
    private :check_password

    #
    # Sets userinfo, argument is string like 'name:pass'.
    #
    def userinfo=(userinfo)
      if userinfo.nil?
        return nil
      end
      check_userinfo(*userinfo)
      set_userinfo(*userinfo)
      # returns userinfo
    end

    #
    # == Args
    #
    # +v+::
    #    String
    #
    # == Description
    #
    # Public setter for the +user+ component
    # (with validation).
    #
    # See also URI::Generic.check_user.
    #
    # == Usage
    #
    #   require 'uri'
    #
    #   uri = URI.parse("http://john:S3nsit1ve@my.example.com")
    #   uri.user = "sam"
    #   uri.to_s  #=> "http://sam:V3ry_S3nsit1ve@my.example.com"
    #
    def user=(user)
      check_user(user)
      set_user(user)
      # returns user
    end

    #
    # == Args
    #
    # +v+::
    #    String
    #
    # == Description
    #
    # Public setter for the +password+ component
    # (with validation).
    #
    # See also URI::Generic.check_password.
    #
    # == Usage
    #
    #   require 'uri'
    #
    #   uri = URI.parse("http://john:S3nsit1ve@my.example.com")
    #   uri.password = "V3ry_S3nsit1ve"
    #   uri.to_s  #=> "http://john:V3ry_S3nsit1ve@my.example.com"
    #
    def password=(password)
      check_password(password)
      set_password(password)
      # returns password
    end

    # Protected setter for the +user+ component, and +password+ if available
    # (with validation).
    #
    # See also URI::Generic.userinfo=.
    #
    def set_userinfo(user, password = nil)
      unless password
        user, password = split_userinfo(user)
      end
      @user     = user
      @password = password if password

      [@user, @password]
    end
    protected :set_userinfo

    # Protected setter for the user component +v+.
    #
    # See also URI::Generic.user=.
    #
    def set_user(v)
      set_userinfo(v, @password)
      v
    end
    protected :set_user

    # Protected setter for the password component +v+.
    #
    # See also URI::Generic.password=.
    #
    def set_password(v)
      @password = v
      # returns v
    end
    protected :set_password

    # Returns the userinfo +ui+ as <code>[user, password]</code>
    # if properly formatted as 'user:password'.
    def split_userinfo(ui)
      return nil, nil unless ui
      user, password = ui.split(':', 2)

      return user, password
    end
    private :split_userinfo

    # Escapes 'user:password' +v+ based on RFC 1738 section 3.1.
    def escape_userpass(v)
      parser.escape(v, /[@:\/]/o) # RFC 1738 section 3.1 #/
    end
    private :escape_userpass

    # Returns the userinfo, either as 'user' or 'user:password'.
    def userinfo
      if @user.nil?
        nil
      elsif @password.nil?
        @user
      else
        @user + ':' + @password
      end
    end

    # Returns the user component.
    def user
      @user
    end

    # Returns the password component.
    def password
      @password
    end

    #
    # Checks the host +v+ component for RFC2396 compliance
    # and against the URI::Parser Regexp for :HOST.
    #
    # Can not have a registry or opaque component defined,
    # with a host component defined.
    #
    def check_host(v)
      return v unless v

      if @opaque
        raise InvalidURIError,
          "can not set host with registry or opaque"
      else
        if defined?(::TruffleRuby)
          bad = !parser.regexp[:HOST].match(v)
        else
          bad = parser.regexp[:HOST] !~ v
        end
        
        if bad
          raise InvalidComponentError,
            "bad component(expected host component): #{v}"
        end
      end

      return true
    end
    private :check_host

    # Protected setter for the host component +v+.
    #
    # See also URI::Generic.host=.
    #
    def set_host(v)
      @host = v
    end
    protected :set_host

    #
    # == Args
    #
    # +v+::
    #    String
    #
    # == Description
    #
    # Public 