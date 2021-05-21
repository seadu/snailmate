# frozen_string_literal: true
# -*- mode: ruby; ruby-indent-level: 2; tab-width: 2 -*-

require 'erb'
require 'fileutils'
require 'pathname'
require_relative 'markup'

##
# Darkfish RDoc HTML Generator
#
# $Id: darkfish.rb 52 2009-01-07 02:08:11Z deveiant $
#
# == Author/s
# * Michael Granger (ged@FaerieMUD.org)
#
# == Contributors
# * Mahlon E. Smith (mahlon@martini.nu)
# * Eric Hodel (drbrain@segment7.net)
#
# == License
#
# Copyright (c) 2007, 2008, Michael Granger. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the author/s, nor the names of the project's
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# == Attributions
#
# Darkfish uses the {Silk Icons}[http://www.famfamfam.com/lab/icons/silk/] set
# by Mark James.

class RDoc::Generator::Darkfish

  RDoc::RDoc.add_generator self

  include ERB::Util

  ##
  # Stylesheets, fonts, etc. that are included in RDoc.

  BUILTIN_STYLE_ITEMS = # :nodoc:
    %w[
      css/fonts.css
      fonts/Lato-Light.ttf
      fonts/Lato-LightItalic.ttf
      fonts/Lato-Regular.ttf
      fonts/Lato-RegularItalic.ttf
      fonts/SourceCodePro-Bold.ttf
      fonts/SourceCodePro-Regular.ttf
      css/rdoc.css
  ]

  ##
  # Path to this file's parent directory. Used to find templates and other
  # resources.

  GENERATOR_DIR = File.join 'rdoc', 'generator'

  ##
  # Release Version

  VERSION = '3'

  ##
  # Description of this generator

  DESCRIPTION = 'HTML generator, written by Michael Granger'

  ##
  # The relative path to style sheets and javascript.  By default this is set
  # the same as the rel_prefix.

  attr_accessor :asset_rel_path

  ##
  # The path to generate files into, combined with <tt>--op</tt> from the
  # options for a full path.

  attr_reader :base_dir

  ##
  # Classes and modules to be used by this generator, not necessarily
  # displayed.  See also #modsort

  attr_reader :classes

  ##
  # No files will be written when dry_run is true.

  attr_accessor :dry_run

  ##
  # When false the generate methods return a String instead of writing to a
  # file.  The default is true.

  attr_accessor :file_output

  ##
  # Files to be displayed by this generator

  attr_reader :files

  ##
  # The JSON index generator for this Darkfish generator

  attr_reader :json_index

  ##
  # Methods to be displayed by this generator

  attr_reader :methods

  ##
  # Sorted list of classes and modules to be displayed by this generator

  attr_reader :modsort

  ##
  # The RDoc::Store that is the source of the generated content

  attr_reader :store

  ##
  # The directory where the template files live

  attr_reader :template_dir # :nodoc:

  ##
  # The output directory

  attr_reader :outputdir

  ##
  # Initialize a few instance variables before we start

  def initialize store, options
    @store   = store
    @options = options

    @asset_rel_path = ''
    @base_dir       = Pathname.pwd.expand_path
    @dry_run        = @options.dry_run
    @file_output    = true
    @template_dir   = Pathname.new options.template_dir
    @template_cache = {}

    @classes = nil
    @context