# frozen_string_literal: true
##
# A TopLevel context is a representation of the contents of a single file

class RDoc::TopLevel < RDoc::Context

  MARSHAL_VERSION = 0 # :nodoc:

  ##
  # This TopLevel's File::Stat struct

  attr_accessor :file_stat

  ##
  # Relative name of this file

  attr_accessor :relative_name

  ##
  # Absolute name of this file

  attr_accessor :absolute_name

  ##
  # All the classes or modules that were declared in
  # this file. These are assigned to either +#classes_hash+
  # or +#modules_hash+ once we know what they really are.

  attr_reader :classes_or_modules

  attr_accessor :diagram # :nodoc:

  ##
  # The parser class that processed this file

  attr_reader :parser

  ##
  # Creates a new TopLevel for the file at +absolute_name+.  If documentation
  # is being generated outside the source dir +relative_name+ is relative to
  # the source directory.

  def initialize absolute_name, relative_name = absolute_name
    super()
    @name = nil
    @absolute_name = absolute_name
    @relative_name = relative_name
    @file_stat     = File.stat(absolute_name) rescue nil # HACK for testing
    @diagram       = nil
    @parser        = nil

    @classes_or_modules = []
  end

  def parser=(val)
    @parser = val
    @store.update_parser_of_file(absolute_name, val) if @store
    @parser
  end

  ##
  # An RDoc::TopLevel is equal to another with the same relative_name

  def == other
    self.class === other and @relative_name == other.relative_name
  end

  alias eql? ==

  ##
  # Adds +an_alias+ to +Object+ instead of +self+.

  def add_alias(an_alias)
    object_class.record_location self
    return an_alias unless @document_self
    object_class.add_alias an_alias
  end

  ##
  # Adds +constant+ to +Object+ instead of +self+.

  def add_constant constant
    object_class.record_location self
    return constant unless @document_self
    object_class.add_constant constant
  end

  ##
  # Adds +include+ to +Object+ instead of +self+.

  def add_include(include)
    object_class.record_location self
    return include unless @document_self
    object_class.add_include include
  end

  ##
  # Adds +method+ to +Object+ instead of +self+.

  def add_method(method)
    object_class.record_location self
    return method unless @document_self
    object_class.add_method method
  end

  ##
  # Adds class or module +mod+. Used in the building phase
  # by the Ruby parser.

  def add_to_classes_or_modules mod
    @classes_or_modules << mod
  end

  ##
  # Base name of this file

  def base_name
    File.basename @relative_name
  end

  alias name base_name

  ##
  # Only a TopLevel that contains text file) will be displayed.  See also
  # RDoc::CodeObject#display?

  def display?
    text? and super
  end

  ##
  # See RDoc::TopLevel::find_class_or_module
  #--
  # TODO Why do we search through all classes/modules found, not just the
  #       ones of this instance?

  def