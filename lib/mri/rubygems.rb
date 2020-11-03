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

  def self.binary_mode
    "rb"
  end

  ##
  # The path where gem executables are to be installed.

  def self.bindir(install_dir=Gem.dir)
    return File.join install_dir, "bin" unless
      install_dir.to_s == Gem.default_dir.to_s
    Gem.default_bindir
  end

  ##
  # The path were rubygems plugins are to be installed.

  def self.plugindir(install_dir=Gem.dir)
    File.join install_dir, "plugins"
  end

  ##
  # Reset the +dir+ and +path+ values.  The next time +dir+ or +path+
  # is requested, the values will be calculated from scratch.  This is
  # mainly used by the unit tests to provide test isolation.

  def self.clear_paths
    @paths         = nil
    @user_home     = nil
    Gem::Specification.reset
    Gem::Security.reset if defined?(Gem::Security)
  end

  ##
  # The standard configuration object for gems.

  def self.configuration
    @configuration ||= Gem::ConfigFile.new []
  end

  ##
  # Use the given configuration object (which implements the ConfigFile
  # protocol) as the standard configuration object.

  def self.configuration=(config)
    @configuration = config
  end

  ##
  # The path to the data directory specified by the gem name.  If the
  # package is not available as a gem, return nil.

  def self.datadir(gem_name)
    spec = @loaded_specs[gem_name]
    return nil if spec.nil?
    spec.datadir
  end

  ##
  # A Zlib::Deflate.deflate wrapper

  def self.deflate(data)
    require "zlib"
    Zlib::Deflate.deflate data
  end

  # Retrieve the PathSupport object that RubyGems uses to
  # lookup files.

  def self.paths
    @paths ||= Gem::PathSupport.new(ENV)
  end

  # Initialize the filesystem paths to use from +env+.
  # +env+ is a hash-like object (typically ENV) that
  # is queried for 'GEM_HOME', 'GEM_PATH', and 'GEM_SPEC_CACHE'
  # Keys for the +env+ hash should be Strings, and values of the hash should
  # be Strings or +nil+.

  def self.paths=(env)
    clear_paths
    target = {}
    env.each_pair do |k,v|
      case k
      when "GEM_HOME", "GEM_PATH", "GEM_SPEC_CACHE"
        case v
        when nil, String
          target[k] = v
        when Array
          unless Gem::Deprecate.skip
            warn <<-EOWARN
Array values in the parameter to `Gem.paths=` are deprecated.
Please use a String or nil.
An Array (#{env.inspect}) was passed in from #{caller[3]}
            EOWARN
          end
          target[k] = v.join File::PATH_SEPARATOR
        end
      else
        target[k] = v
      end
    end
    @paths = Gem::PathSupport.new ENV.to_hash.merge(target)
    Gem::Specification.dirs = @paths.path
  end

  ##
  # The path where gems are to be installed.

  def self.dir
    paths.home
  end

  def self.path
    paths.path
  end

  def self.spec_cache_dir
    paths.spec_cache_dir
  end

  ##
  # Quietly ensure the Gem directory +dir+ contains all the proper
  # subdirectories.  If we can't create a directory due to a permission
  # problem, then we will silently continue.
  #
  # If +mode+ is given, missing directories are created with this mode.
  #
  # World-writable directories will never be created.

  def self.ensure_gem_subdirectories(dir = Gem.dir, mode = nil)
    ensure_subdirectories(dir, mode, REPOSITORY_SUBDIRECTORIES)
  end

  ##
  # Quietly ensure the Gem directory +dir+ contains all the proper
  # subdirectories for handling default gems.  If we can't create a
  # directory due to a permission problem, then we will silently continue.
  #
  # If +mode+ is given, missing directories are created with this mode.
  #
  # World-writable directories will never be created.

  def self.ensure_default_gem_subdirectories(dir = Gem.dir, mode = nil)
    ensure_subdirectories(dir, mode, REPOSITORY_DEFAULT_GEM_SUBDIRECTORIES)
  end

  def self.ensure_subdirectories(dir, mode, subdirs) # :nodoc:
    old_umask = File.umask
    File.umask old_umask | 002

    options = {}

    options[:mode] = mode if mode

    subdirs.each do |name|
      subdir = File.join dir, name
      next if File.exist? subdir

      require "fileutils"

      begin
        FileUtils.mkdir_p subdir, **options
      rescue SystemCallError
      end
    end
  ensure
    File.umask old_umask
  end

  ##
  # The extension API version of ruby.  This includes the static vs non-static
  # distinction as extensions cannot be shared between the two.

  def self.extension_api_version # :nodoc:
    if "no" == RbConfig::CONFIG["ENABLE_SHARED"]
      "#{ruby_api_version}-static"
    else
      ruby_api_version
    end
  end

  ##
  # Returns a list of paths matching +glob+ that can be used by a gem to pick
  # up features from other gems.  For example:
  #
  #   Gem.find_files('rdoc/discover').each do |path| load path end
  #
  # if +check_load_path+ is true (the default), then find_files also searches
  # $LOAD_PATH for files as well as gems.
  #
  # Note that find_files will return all files even if they are from different
  # versions of the same gem.  See also find_latest_files

  def self.find_files(glob, check_load_path=true)
    files = []

    files = find_files_from_load_path glob if check_load_path

    gem_specifications = @gemdeps ? Gem.loaded_specs.values : Gem::Specification.stubs

    files.concat gem_specifications.map {|spec|
      spec.matches_for_glob("#{glob}#{Gem.suffix_pattern}")
    }.flatten

    # $LOAD_PATH might contain duplicate entries or reference
    # the spec dirs directly, so we prune.
    files.uniq! if check_load_path

    return files
  end

  def self.find_files_from_load_path(glob) # :nodoc:
    glob_with_suffixes = "#{glob}#{Gem.suffix_pattern}"
    $LOAD_PATH.map do |load_path|
      Gem::Util.glob_files_in_dir(glob_with_suffixes, load_path)
    end.flatten.select {|file| File.file? file.tap(&Gem::UNTAINT) }
  end

  ##
  # Returns a list of paths matching +glob+ from the latest gems that can be
  # used by a gem to pick up features from other gems.  For example:
  #
  #   Gem.find_latest_files('rdoc/discover').each do |path| load path end
  #
  # if +check_load_path+ is true (the default), then find_latest_files also
  # searches $LOAD_PATH for files as well as gems.
  #
  # Unlike find_files, find_latest_files will return only files from the
  # latest version of a gem.

  def self.find_latest_files(glob, check_load_path=true)
    files = []

    files = find_files_from_load_path glob if check_load_path

    files.concat Gem::Specification.latest_specs(true).map {|spec|
      spec.matches_for_glob("#{glob}#{Gem.suffix_pattern}")
    }.flatten

    # $LOAD_PATH might contain duplicate entries or reference
    # the spec dirs directly, so we prune.
    files.uniq! if check_load_path

    return files
  end

  ##
  # Top level install helper method. Allows you to install gems interactively:
  #
  #   % irb
  #   >> Gem.install "minitest"
  #   Fetching: minitest-5.14.0.gem (100%)
  #   => [#<Gem::Specification:0x1013b4528 @name="minitest", ...>]

  def self.install(name, version = Gem::Requirement.default, *options)
    require_relative "rubygems/dependency_installer"
    inst = Gem::DependencyInstaller.new(*options)
    inst.install name, version
    inst.installed_gems
  end

  ##
  # Get the default RubyGems API host. This is normally
  # <tt>https://rubygems.org</tt>.

  def self.host
    @host ||= Gem::DEFAULT_HOST
  end

  ## Set the default RubyGems API host.

  def self.host=(host)
    @host = host
  end

  ##
  # The index to insert activated gem paths into the $LOAD_PATH. The activated
  # gem's paths are inserted before site lib directory by default.

  def self.load_path_insert_index
    $LOAD_PATH.each_with_index do |path, i|
      return i if path.instance_variable_defined?(:@gem_prelude_index)
    end

    index = $LOAD_PATH.index RbConfig::CONFIG["sitelibdir"]

    index || 0
  end

  ##
  # The number of paths in the +$LOAD_PATH+ from activated gems. Used to
  # prioritize +-I+ and +ENV['RUBYLIB']+ entries during +require+.

  def self.activated_gem_paths
    @activated_gem_paths ||= 0
  end

  ##
  # Add a list of paths to the $LOAD_PATH at the proper place.

  def self.add_to_load_path(*paths)
    @activated_gem_paths = activated_gem_paths + paths.size

    # gem directories must come after -I and ENV['RUBYLIB']
    $LOAD_PATH.insert(Gem.load_path_insert_index, *paths)
  end

  @yaml_loaded = false

  ##
  # Loads YAML, preferring Psych

  def self.load_yaml
    return if @yaml_loaded

    require "psych"
    require_relative "rubygems/psych_tree"

    require_relative "rubygems/safe_yaml"

    @yaml_loaded = true
  end

  ##
  # The file name and line number of the caller of the caller of this method.
  #
  # +depth+ is how many layers up the call stack it should go.
  #
  # e.g.,
  #
  # def a; Gem.location_of_caller; end
  # a #=> ["x.rb", 2]  # (it'll vary depending on file name and line number)
  #
  # def b; c; end
  # def c; Gem.location_of_caller(2); end
  # b #=> ["x.rb", 6]  # (it'll vary depending on file name and line number)

  def self.location_of_caller(depth = 1)
    caller[depth] =~ /(.*?):(\d+).*?$/i
    file = $1
    lineno = $2.to_i

    [file, lineno]
  end

  ##
  # The version of the Marshal format for your Ruby.

  def self.marshal_version
    "#{Marshal::MAJOR_VERSION}.#{Marshal::MINOR_VERSION}"
  end

  ##
  # Set array of platforms this RubyGems supports (primarily for testing).

  def self.platforms=(platforms)
    @platforms = platforms
  end

  ##
  # Array of platforms this RubyGems supports.

  def self.platforms
    @platforms ||= []
    if @platforms.empty?
      @platforms = [Gem::Platform::RUBY, Gem::Platform.local]
    end
    @platforms
  end

  ##
  # Adds a post-build hook that will be passed an Gem::Installer instance
  # when Gem::Installer#install is called.  The hook is called after the gem
  # has been extracted and extensions have been built but before the
  # executables or gemspec has been written.  If the hook returns +false+ then
  # the gem's files will be removed and the install will be aborted.

  def self.post_build(&hook)
    @post_build_hooks << hook
  end

  ##
  # Adds a post-install hook that will be passed an Gem::Installer instance
  # when Gem::Installer#install is called

  def self.post_install(&hook)
    @post_install_hooks << hook
  end

  ##
  # Adds a post-installs hook that will be passed a Gem::DependencyInstaller
  # and a list of installed specifications when
  # Gem::DependencyInstaller#install is complete

  def self.done_installing(&hook)
    @done_installing_hooks << hook
  end

  ##
  # Adds a hook that will get run after Gem::Specification.reset is
  # run.

  def self.post_reset(&hook)
    @post_reset_hooks << hook
  end

  ##
  # Adds a post-uninstall hook that will be passed a Gem::Uninstaller instance
  # and the spec that was uninstalled when Gem::Uninstaller#uninstall is
  # called

  def self.post_uninstall(&hook)
    @post_uninstall_hooks << hook
  end

  ##
  # Adds a pre-install hook that will be passed an Gem::Installer instance
  # when Gem::Installer#install is called.  If the hook returns +false+ then
  # the install will be aborted.

  def self.pre_install(&hook)
    @pre_install_hooks << hook
  end

  ##
  # Adds a hook that will get run before Gem::Specification.reset is
  # run.

  def self.pre_reset(&hook)
    @pre_reset_hooks << hook
  end

  ##
  # Adds a pre-uninstall hook that will be passed an Gem::Uninstaller instance
  # and the spec that will be uninstalled when Gem::Uninstaller#uninstall is
  # called

  def self.pre_uninstall(&hook)
    @pre_uninstall_hooks << hook
  end

  ##
  # The directory prefix this RubyGems was installed at. If your
  # prefix is in a standard location (ie, rubygems is installed where
  # you'd expect it to be), then prefix returns nil.

  def self.prefix
    prefix = File.dirname RUBYGEMS_DIR

    if prefix != File.expand_path(RbConfig::CONFIG["sitelibdir"]) &&
       prefix != File.expand_path(RbConfig::CONFIG["libdir"]) &&
       "lib" == File.basename(RUBYGEMS_DIR)
      prefix
    end
  end

  ##
  # Refresh available gems from disk.

  def self.refresh
    Gem::Specification.reset
  end

  ##
  # Safely read a file in binary mode on all platforms.

  def self.read_binary(path)
    open_file(path, "rb+") do |io|
      io.read
    end
  rescue Errno::EACCES, Errno::EROFS
    open_file(path, "rb") do |io|
      io.read
    end
  end

  ##
  # Safely write a file in binary mode on all platforms.
  def self.write_binary(path, data)
    open_file(path, "wb") do |io|
      io.write data
    end
  end

  ##
  # Open a file with given flags, and on Windows protect access with flock

  def self.open_file(path, flags, &block)
    File.open(path, flags) do |io|
      if !java_platform? && win_platform?
        begin
          io.flock(File::LOCK_EX)
        rescue Errno::ENOSYS, Errno::ENOTSUP
        end
      end
      yield io
    end
  rescue Errno::ENOLCK # NFS
    if Thread.main != Thread.current
      raise
    else
      File.open(path, flags) do |io|
        yield io
      end
    end
  end

  ##
  # The path to the running Ruby interpreter.

  def self.ruby
    if @ruby.nil?
      @ruby = RbConfig.ruby

      @ruby = "\"#{@ruby}\"" if @ruby =~ /\s/
    end

    @ruby
  end

  ##
  # Returns a String containing the API compatibility version of Ruby

  def self.rub