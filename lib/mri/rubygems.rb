# frozen_string_literal: true
#--
# Copyright 2006 by Chad Fowler, Rich Kilmer, Jim Weirich and others.
# All rights reserved.
# See LICENSE.txt for permissions.
#++

require "rbconfig"

module Gem
  VERSION = "3.3.26".freeze
end

# Must be first since it unloads the prelude from 1.9.2
require_relative "rubygems/compatibility"

require_relative "rubygems/defaults"
require_relative "rubygems/deprecate"
require_relative "rubygems/errors"

##
# RubyGems is the Ruby standard for publishing and managing third party
# libraries.
#
# For user documentation, see:
#
# * <tt>gem help</tt> and <tt>gem help [command]</tt>
# * {RubyGems User Guide}[https://guides.rubygems.org/]
# * {Frequently Asked Questions}[https://guides.rubygems.org/faqs]
#
# For gem developer documentation see:
#
# * {Creating Gems}[https://guides.rubygems.org/make-your-own-gem]
# * Gem::Specification
# * Gem::Version for version dependency notes
#
# Further RubyGems documentation can be found at:
#
# * {RubyGems Guides}[https://guides.rubygems.org]
# * {RubyGems API}[https://www.rubydoc.info/github/rubygems/rubygems] (also available from
#   <tt>gem server</tt>)
#
# == RubyGems Plugins
#
# RubyGems will load plugins in the latest version of each installed gem or
# $LOAD_PATH.  Plugins must be named 'rubygems_plugin' (.rb, .so, etc) and
# placed at the root of your gem's #require_path.  Plugins are installed at a
# special location and loaded on boot.
#
# For an example plugin, see the {Graph gem}[https://github.com/seattlerb/graph]
# which adds a <tt>gem graph</tt> command.
#
# == RubyGems Defaults, Packaging
#
# RubyGems defaults are stored in lib/rubygems/defaults.rb.  If you're packaging
# RubyGems or implementing Ruby you can change RubyGems' defaults.
#
# For RubyGems packagers, provide lib/rubygems/defaults/operating_system.rb
# and override any defaults from lib/rubygems/defaults.rb.
#
# For Ruby implementers, provide lib/rubygems/defaults/#{RUBY_ENGINE}.rb and
# override any defaults from lib/rubygems/defaults.rb.
#
# If you need RubyGems to perform extra work on install or uninstall, your
# defaults override file can set pre/post install and uninstall hooks.
# See Gem::pre_install, Gem::pre_uninstall, Gem::post_install,
# Gem::post_uninstall.
#
# == Bugs
#
# You can submit bugs to the
# {RubyGems bug tracker}[https://github.com/rubygems/rubygems/issues]
# on GitHub
#
# == Credits
#
# RubyGems is currently maintained by Eric Hodel.
#
# RubyGems was originally developed at RubyConf 2003 by:
#
# * Rich Kilmer  -- rich(at)infoether.com
# * Chad Fowler  -- chad(at)chadfowler.com
# * David Black  -- dblack(at)wobblini.net
# * Paul Brannan -- paul(at)atdesk.com
# * Jim Weirich   -- jim(at)weirichhouse.org
#
# Contributors:
#
# * Gavin Sinclair     -- gsinclair(at)soyabean.com.au
# * George Marrows     -- george.marrows(at)ntlworld.com
# * Dick Davies        -- rasputnik(at)hellooperator.net
# * Mauricio Fernandez -- batsman.geo(at)yahoo.com
# * Simon Strandgaard  -- neoneye(at)adslhome.dk
# * Dave Glasser       -- glasser(at)mit.edu
# * Paul Duncan        -- pabs(at)pablotron.org
# * Ville Aine         -- vaine(at)cs.helsinki.fi
# * Eric Hodel         -- drbrain(at)segment7.net
# * Daniel Berger      -- djberg96(at)gmail.com
# * Phil Hagelberg     -- technomancy(at)gmail.com
# * Ryan Davis         -- ryand-ruby(at)zenspider.com
# * Evan Phoenix       -- evan(at)fallingsnow.net
# * Steve Klabnik      -- steve(at)steveklabnik.com
#
# (If your name is missing, PLEASE let us know!)
#
# == License
#
# See {LICENSE.txt}[rdoc-ref:lib/rubygems/LICENSE.txt] for permissions.
#
# Thanks!
#
# -The RubyGems Team

module Gem
  RUBYGEMS_DIR = __dir__

  # Taint support is deprecated in Ruby 2.7.
  # This allows switching ".untaint" to ".tap(&Gem::UNTAINT)",
  # to avoid deprecation warnings in Ruby 2.7.
  UNTAINT = RUBY_VERSION < "2.7" ? :untaint.to_sym : proc {}

  # When https://bugs.ruby-lang.org/issues/17259 is available, there is no need to override Kernel#warn
  KERNEL_WARN_IGNORES_INTERNAL_ENTRIES = RUBY_ENGINE == "truffleruby" ||
                                         (RUBY_ENGINE == "ruby" && RUBY_VERSION >= "3.0")

  ##
  # An Array of Regexps that match windows Ruby platforms.

  WIN_PATTERNS = [
    /bccwin/i,
    /cygwin/i,
    /djgpp/i,
    /mingw/i,
    /mswin/i,
    /wince/i,
  ].freeze

  GEM_DEP_FILES = %w[
    gem.deps.rb
    gems.rb
    Gemfile
    Isolate
  ].freeze

  ##
  # Subdirectories in a gem repository

  REPOSITORY_SUBDIRECTORIES = %w[
    build_info
    cache
    doc
    extensions
    gems
    plugins
    specifications
  ].freeze

  ##
  # Subdirectories in a gem repository for default gems

  REPOSITORY_DEFAULT_GEM_SUBDIRECTORIES = %w[
    gems
    specifications/default
  ].freeze

  @@win_platform = nil

  @configuration = nil
  @gemdeps = nil
  @loaded_specs = {}
  LOADED_SPECS_MUTEX = Thread::Mutex.new
  @path_to_default_spec_map = {}
  @platforms = []
  @ruby = nil
  @ruby_api_version = nil
  @sources = nil

  @post_build_hooks     ||= []
  @post_install_hooks   ||= []
  @post_uninstall_hooks ||= []
  @pre_uninstall_hooks  ||= []
  @pre_install_hooks    ||= []
  @pre_reset_hooks      ||= []
  @post_reset_hooks     ||= []

  @default_source_date_epoch = nil

  ##
  # Try to activate a gem containing +path+. Returns true if
  # activation succeeded or wasn't needed because it was already
  # activated. Returns false if it can't find the path in a gem.

  def self.try_activate(path)
    # finds the _latest_ version... regardless of loaded specs and their deps
    # if another gem had a requirement that would mean we shouldn't
    # activate the latest version, then either it would already be activated
    # or if it was ambiguous (and thus unresolved) the code in our custom
    # require will try to activate the more specific version.

    spec = Gem::Specification.find_by_path path
    return false unless spec
    return true if spec.activated?

    begin
      spec.activate
    rescue Gem::LoadError => e # this could fail due to gem dep collisions, go lax
      spec_by_name = Gem::Specification.find_by_name(spec.name)
      if spec_by_name.nil?
        raise e
      else
        spec_by_name.activate
      end
    end

    return true
  end

  def self.needs
    rs = Gem::RequestSet.new

    yield rs

    finish_resolve rs
  end

  def self.finish_resolve(request_set=Gem::RequestSet.new)
    request_set.import Gem::Specification.unresolved_deps.values
    request_set.import Gem.loaded_specs.values.map {|s| Gem::Dependency.new(s.name, s.version) }

    request_set.resolve_current.each do |s|
      s.full_spec.activate
    end
  end

  ##
  # Find the full path to the executable for gem +name+.  If the +exec_name+
  # is not given, an exception will be raised, otherwise the
  # specified executable's path is returned.  +requirements+ allows
  # you to specify specific gem versions.

  def self.bin_path(name, exec_name = nil, *requirements)
    requirements = Gem::Requirement.default if
      requirements.empty?

    find_spec_for_exe(name, exec_name, requirements).bin_file exec_name
  end

  def self.find_spec_for_exe(name, exec_name, requirements)
    raise ArgumentError, "you must supply exec_name" unless exec_name

    dep = Gem::Dependency.new name, requirements

    loaded = Gem.loaded_specs[name]

    return loaded if loaded && dep.matches_spec?(loaded)

    specs = dep.matching_specs(true)

    specs = specs.find_all do |spec|
      spec.executables.include? exec_name
    end if exec_name

    unless spec = specs.first
      msg = "can't find gem #{dep} with executable #{exec_name}"
      raise Gem::GemNotFoundException, msg
    end

    spec
  end
  private_class_method :find_spec_for_exe

  ##
  # Find the full path to the executable for gem +name+.  If the +exec_name+
  # is not given, an exception will be raised, otherwise the
  # specified executable's path is returned.  +requirements+ allows
  # you to specify specific gem versions.
  #
  # A side effect of this method is that it will activate the gem that
  # contains the executable.
  #
  # This method should *only* be used in bin stub files.

  def self.activate_bin_path(name, exec_name = nil, *requirements) # :nodoc:
    spec = find_spec_for_exe name, exec_name, requirements
    Gem::LOADED_SPECS_MUTEX.synchronize do
      spec.activate
      finish_resolve
    end
    spec.bin_file exec_name
  end

  ##
  # The mode needed to read a file as straight binary.

  def s