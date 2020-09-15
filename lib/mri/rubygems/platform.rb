# frozen_string_literal: true
require_relative "deprecate"

##
# Available list of platforms for targeting Gem installations.
#
# See `gem help platform` for information on platform matching.

class Gem::Platform
  @local = nil

  attr_accessor :cpu, :os, :version

  def self.local
    arch = RbConfig::CONFIG["arch"]
    arch = "#{arch}_60" if arch =~ /mswin(?:32|64)$/
    @local ||= new(arch)
  end

  def self.match(platform)
    warn 'Gem::Platform.match should not be used on TruffleRuby, use match_spec? instead', uplevel: 1
    match_platforms?(platform, Gem.platforms)
  end

  def self.match_platforms?(platform, platforms)
    platform = Gem::Platform.new(platform) unless platform.is_a?(Gem::Platform)
    platforms.any? do |local_platform|
      platform.nil? ||
        local_platform == platform ||
        (local_platform != Gem::Platform::RUBY && platform =~ local_platform)
    end
  end
  private_class_method :match_platforms?

  def self.match_spec?(spec)
    match_gem?(spec.platform, spec.name)
  end

  REUSE_AS_BINARY_ON_TRUFFLERUBY = %w[libv8 libv8-node sorbet-static]

  def self.match_gem?(platform, gem_name)
    raise unless String === gem_name
    if REUSE_AS_BINARY_ON_TRUFFLERUBY.include?(gem_name)
      match_platforms?(platform, [Gem::Platform::RUBY, Gem::Platform.local])
    else
      match_platforms?(platform, Gem.platforms)
    end
  end

  def self.sort_priority(platform)
    platform == Gem::Platform::RUBY ? -1 : 1
  end

  def self.installable?(spec)
    if spec.respond_to? :installable_platform?
      spec.installable_platform?
    else
      match_spec? spec
    end
  end

  def self.new(arch) # :nodoc:
    case arch
    when Gem::Platform::CURRENT then
      Gem::Platform.local
    when Gem::Platform::RUBY, nil, "" then
      Gem::Platform::RUBY
    else
      super
    end
  end

  def initialize(arch)
    case arch
    when Array then
      @cpu, @os, @version = arch
    when String then
      arch = arch.split "-"

      if arch.length > 2 && arch.last !~ /\d+(\.\d+)?$/ # reassemble x86-linux-{libc}
        extra = arch.pop
        arch.last << "-#{extra}"
      end

      cpu = arch.shift

      @cpu = case cpu
      when /i\d86/ then "x86"
      else cpu
      end

      if arch.length == 2 && arch.last =~ /^\d+(\.\d+)?$/ # for command-line
        @os, @version = arch
        return
      end

      os, = arch
      @cpu, os = nil, cpu if os.nil? # legacy jruby

      @os, @version = case os
      when /aix(\d+)?/ then             [ "aix",       $1  ]
      when /cygwin/ then                [ "cygwin",    nil ]
      when /darwin(\d+)?/ then          [ "darwin",    $1  ]
      when /^macruby$/ then             [ "macruby",   nil ]
      when /freebsd(\d+)?/ then         [ "freebsd",   $1  ]
      when /hpux(\d+)?/ then            [ "hpux",      $1  ]
      when /^java$/, /^jruby$/ then     [ "java",      nil ]
      when /^java([\d.]*)/ then         [ "java",      $1  ]
      when /^dalvik(\d+)?$/ then        [ "dalvik",    $1  ]
      when /^