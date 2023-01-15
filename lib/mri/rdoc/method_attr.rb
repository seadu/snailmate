# frozen_string_literal: true
##
# Abstract class representing either a method or an attribute.

class RDoc::MethodAttr < RDoc::CodeObject

  include Comparable

  ##
  # Name of this method/attribute.

  attr_accessor :name

  ##
  # public, protected, private

  attr_accessor :visibility

  ##
  # Is this a singleton method/attribute?

  attr_accessor :singleton

  ##
  # Source file token stream

  attr_reader :text

  ##
  # Array of other names for this method/attribute

  attr_reader :aliases

  ##
  # The method/attribute we're aliasing

  attr_accessor :is_alias_for

  #--
  # The attributes below are for AnyMethod only.
  # They are left here for the time being to
  # allow ri to operate.
  # TODO modify ri to avoid calling these on attributes.
  #++

  ##
  # Parameters yielded by the called block

  attr_reader :block_params

  ##
  # Parameters for this method

  attr_accessor :params

  ##
  # Different ways to call this method

  attr_accessor :call_seq

  ##
  # The call_seq or the param_seq with method name, if there is no call_seq.

  attr_reader :arglists

  ##
  # Pretty parameter list for this method

  attr_reader :param_seq


  ##
  # Creates a new MethodAttr from token stream +text+ and method or attribute
  # name +name+.
  #
  # Usually this is called by super from a subclass.

  def initialize text, name
    super()

    @text = text
    @name = name

    @aliases      = []
    @is_alias_for = nil
    @parent_name  = nil
    @singleton    = nil
    @visibility   = :public
    @see = false

    @arglists     = nil
    @block_params = nil
    @call_seq     = nil
    @param_seq    = nil
    @params       = nil
  end

  ##
  # Resets cached data for the object so it can be rebuilt by accessor methods

  def initialize_copy other # :nodoc:
    @full_name = nil
  end

  def initialize_visibility # :nodoc:
    super
    @see = nil
  end

  ##
  # Order by #singleton then #name

  def <=>(other)
    return unless other.respond_to?(:singleton) &&
                  other.respond_to?(:name)

    [     @singleton ? 0 : 1,       name] <=>
    [other.singleton ? 0 : 1, other.name]
  end

  def == other # :nodoc:
    equal?(other) or self.class == other.class and full_name == other.full_name
  end

  ##
  # A method/attribute is documented if any of the following is true:
  # - it was marked with :nodoc:;
  # - it has a comment;
  # - it is an alias for a documented method;
  # - it has a +#see+ method that is documented.

  def documented?
    super or
      (is_alias_for and is_alias_for.documented?) or
      (see and see.documented?)
  end

  ##
  # A method/attribute to look at,
  # in particular if this method/attribute has no documentation.
  #
  # It can be a method/attribute of the superclass or of an included module,
  # including the Kernel module, which is always appended to the included
  # modules.
  #
  # Returns +nil+ if there is no such method/attribute.
  # The +#is_alias_for+ method/attribute, if any, is not included.
  #
  # Templates may generate a "see also ..." if this method/attribute
  # has documentation, and "see ..." if it does not.

  def see
    @see = find_see if @see == false
    @see
  end

  ##
  # Sets the store for this class or module and its contained code objects.

  def store= store
    super

    @file = @store.add_file @file.full_name if @file
  end

  def find_see # :nodoc:
    return nil if singleton || is_alias_for

    # look for the method
    other = find_method_or_attribute name
    return other if other

    # if it is a setter, look for a getter
    return nil unless name =~ /[a-z_]=$/i   # avoid == or ===
    return find_method_or_attribute name[0..-2]
  end

  def find_method_or_attribute name # :nodoc:
    return nil unless parent.respond_to? :ancestors

    searched = parent.ancestors
    kernel = @store.modules_hash['Kernel']

    searched << kernel if kernel &&
      parent != kernel && !searched.include?(kernel)

    searched.each do |ancestor|
      next if String === ancestor
      next if parent == ancestor

      other = ancestor.find_method_named('#' + name) ||
              ancestor.find_attribute_named(name)

      return other if other
    end

    nil
  end

  ##
  # Abstract method. Contexts in their building phase call this
  # to register a new alias for this known method/attribute.
  #
  # - creates a new AnyMethod/Attribute named <tt>an_alias.new_name</tt>;
  # - adds +self+ as an alias for the new method or attribute
  # - adds the method or attribute to #aliases
  # - adds the method or attribute to +context+.

  def add_alias(an_alias, context)
    raise NotImplementedError
  end

  ##
  # HTML fragment reference for this method

  def aref
 