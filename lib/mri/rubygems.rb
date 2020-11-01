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
# For RubyGems packagers, provide li