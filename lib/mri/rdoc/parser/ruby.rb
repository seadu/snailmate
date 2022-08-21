# frozen_string_literal: true
##
# This file contains stuff stolen outright from:
#
#   rtags.rb -
#   ruby-lex.rb - ruby lexcal analyzer
#   ruby-token.rb - ruby tokens
#       by Keiju ISHITSUKA (Nippon Rational Inc.)
#

##
# Extracts code elements from a source file returning a TopLevel object
# containing the constituent file elements.
#
# This file is based on rtags
#
# RubyParser understands how to document:
# * classes
# * modules
# * methods
# * constants
# * aliases
# * private, public, protected
# * private_class_function, public_class_function
# * private_constant, public_constant
# * module_function
# * attr, attr_reader, attr_writer, attr_accessor
# * extra accessors given on the command line
# * metaprogrammed methods
# * require
# * include
#
# == Method Arguments
#
#--
# NOTE: I don't think this works, needs tests, remove the paragraph following
# this block when known to work
#
# The parser extracts the arguments from the method definition.  You can
# override this with a custom argument definition using the :args: directive:
#
#   ##
#   # This method tries over and over until it is tired
#
#   def go_go_go(thing_to_try, tries = 10) # :args: thing_to_try
#     puts thing_to_try
#     go_go_go thing_to_try, tries - 1
#   end
#
# If you have a more-complex set of overrides you can use the :call-seq:
# directive:
#++
#
# The parser extracts the arguments from the method definition.  You can
# override this with a custom argument definition using the :call-seq:
# directive:
#
#   ##
#   # This method can be called with a range or an offset and length
#   #
#   # :call-seq:
#   #   my_method(Range)
#   #   my_method(offset, length)
#
#   def my_method(*args)
#   end
#
# The parser extracts +yield+ expressions from method bodies to gather the
# yielded argument names.  If your method manually calls a block instead of
# yielding or you want to override the discovered argument names use
# the :yields: directive:
#
#   ##
#   # My method is awesome
#
#   def my_method(&block) # :yields: happy, times
#     block.call 1, 2
#   end
#
# == Metaprogrammed Methods
#
# To pick up a metaprogrammed method, the parser looks for a comment starting
# with '##' before an identifier:
#
#   ##
#   # This is a meta-programmed method!
#
#   add_my_method :meta_method, :arg1, :arg2
#
# The parser looks at the token after the identifier to determine the name, in
# this example, :meta_method.  If a name cannot be found, a warning is printed
# and 'unknown is used.
#
# You can force the name of a method using the :method: directive:
#
#   ##
#   # :method: some_method!
#
# By default, meta-methods are instance methods.  To indicate that a method is
# a singleton method instead use the :singleton-method: directive:
#
#   ##
#   # :singleton-method:
#
# You can also use the :singleton-method: directive with a name:
#
#   ##
#   # :singleton-method: some_method!
#
# You can define arguments for metaprogrammed methods via either the
# :call-seq:, :arg: or :args: directives.
#
# Additionally you can mark a method as an attribute by
# using :attr:, :attr_reader:, :attr_writer: or :attr_accessor:.  Just like
# for :method:, the name is optional.
#
#   ##
#   # :attr_reader: my_attr_name
#
# == Hidden methods and attributes
#
# You can provide documentation for methods that don't appear using
# the :method:, :singleton-method: and :attr: directives:
#
#   ##
#   # :attr_writer: ghost_writer
#   # There is an attribute here, but you can't see it!
#
#   ##
#   # :method: ghost_method
#   # There is a method here, but you can't see it!
#
#   ##
#   # this is a comment for a regular method
#
#   def regular_method() end
#
# Note that by default, the :method: directive will be ignored if there is a
# standard rdocable item following it.

require 'ripper'
require_relative 'ripper_state_lex'

class RDoc::Parser::Ruby < RDoc::Parser

  parse_files_matching(/\.rbw?$/)

  include RDoc::TokenStream
  include RDoc::Parser::RubyTools

  ##
  # RDoc::NormalClass type

  NORMAL = "::"

  ##
  # RDoc::SingleClass type

  SINGLE = "<<"

  ##
  # Creates a new Ruby parser.

  def initialize(top_level, file_name, content, options, stats)
    super

    if /\t/ =~ content then
      tab_width = @options.tab_width
      content = content.split(/\n/).map do |line|
        1 while line.gsub!(/\t+/) {
          ' ' * (tab_width*$&.length - $`.length % tab_width)
        }  && $~
        line
      end.join("\n")
    end

    @size = 0
    @token_listeners = nil
    content = RDoc::Encoding.remove_magic_comment content
    @scanner = RDoc::Parser::RipperStateLex.parse(content)
    @content = content
    @scanner_point = 0
    @prev_seek = nil
    @markup = @options.markup
    @track_visibility = :nodoc != @options.visibility
    @encoding = @options.encoding

    reset
  end

  def tk_nl?(tk)
    :on_nl == tk[:kind] or :on_ignored_nl == tk[:kind]
  end

  ##
  # Retrieves the read token stream and replaces +pattern+ with +replacement+
  # using gsub.  If the result is only a ";" returns an empty string.

  def get_tkread_clean pattern, replacement # :nodoc:
    read = get_tkread.gsub(pattern, replacement).strip
    return '' if read == ';'
    read
  end

  ##
  # Extracts the visibility information for the visibility token +tk+
  # and +single+ class type identifier.
  #
  # Returns the visibility type (a string), the visibility (a symbol) and
  # +singleton+ if the methods following should be converted to singleton
  # methods.

  def get_visibility_information tk, single # :nodoc:
    vis_type  = tk[:text]
    singleton = single == SINGLE

    vis =
      case vis_type
      when 'private'   then :private
      when 'protected' then :protected
      when 'public'    then :public
      when 'private_class_method' then
        singleton = true
        :private
      when 'public_class_method' then
        singleton = true
        :public
      when 'module_function' then
        singleton = true
        :public
      else
        raise RDoc::Error, "Invalid visibility: #{tk.name}"
      end

    return vis_type, vis, singleton
  end

  ##
  # Look for the first comment in a file that isn't a shebang line.

  def collect_first_comment
    skip_tkspace
    comment = ''.dup
    comment = RDoc::Encoding.change_encoding comment, @encoding if @encoding
    first_line = true
    first_comment_tk_kind = nil
    line_no = nil

    tk = get_tk

    while tk && (:on_comment == tk[:kind] or :on_embdoc == tk[:kind])
      comment_body = retrieve_comment_body(tk)
      if first_line and comment_body =~ /\A#!/ then
        skip_tkspace
        tk = get_tk
      elsif first_line and comment_body =~ /\A#\s*-\*-/ then
        first_line = false
        skip_tkspace
        tk = get_tk
      else
        break if first_comment_tk_kind and not first_comment_tk_kind === tk[:kind]
        first_comment_tk_kind = tk[:kind]

        line_no = tk[:line_no] if first_line
        first_line = false
        comment << comment_body
        tk = get_tk

        if :on_nl === tk then
          skip_tkspace_without_nl
          tk = get_tk
        end
      end
    end

    unget_tk tk

    new_comment comment, line_no
  end

  ##
  # Consumes trailing whitespace from the token stream

  def consume_trailing_spaces # :nodoc:
    skip_tkspace_without_nl
  end

  ##
  # Creates a new attribute in +container+ with +name+.

  def create_attr container, single, name, rw, comment # :nodoc:
    att = RDoc::Attr.new get_tkread, name, rw, comment, single == SINGLE
    record_location att

    container.add_attribute att
    @stats.add_attribute att

    att
  end

  ##
  # Creates a module alias in +container+ at +rhs_name+ (or at the top-level
  # for "::") with the name from +constant+.

  def create_module_alias container, constant, rhs_name # :nodoc:
    mod = if rhs_name =~ /^::/ then
            @store.find_class_or_module rhs_name
          else
            container.find_module_named rhs_name
          end

    container.add_module_alias mod, rhs_name, constant, @top_level
  end

  ##
  # Aborts with +msg+

  def error(msg)
    msg = make_message msg

    abort msg
  end

  ##
  # Looks for a true or false token.

  def get_bool
    skip_tkspace
    tk = get_tk
    if :on_kw == tk[:kind] && 'true' == tk[:text]
      true
    elsif :on_kw == tk[:kind] && ('false' == tk[:text] || 'nil' == tk[:text])
      false
    else
      unget_tk tk
      true
    end
  end

  ##
  # Look for the name of a class of module (optionally with a leading :: or
  # with :: separated named) and return the ultimate name, the associated
  # container, and the given name (with the ::).

  def get_class_or_module container, ignore_constants = false
    skip_tkspace
    name_t = get_tk
    given_name = ''.dup

    # class ::A -> A is in the top level
    if :on_op == name_t[:kind] and '::' == name_t[:text] then # bug
      name_t = get_tk
      container = @top_level
      given_name << '::'
    end

    skip_tkspace_without_nl
    given_name << name_t[:text]

    is_self = name_t[:kind] == :on_op && name_t[:text] == '<<'
    new_modules = []
    while !is_self && (tk = peek_tk) and :on_op == tk[:kind] and '::' == tk[:text] do
      prev_container = container
      container = container.find_module_named name_t[:text]
      container ||=
        if ignore_constants then
          c = RDoc::NormalModule.new name_t[:text]
          c.store = @store
          new_modules << [prev_container, c]
          c
        else
          c = prev_container.add_module RDoc::NormalModule, name_t[:text]
          c.ignore unless prev_container.document_children
          @top_level.add_to_classes_or_modules c
          c
        end

      record_location container

      get_tk
      skip_tkspace
      if :on_lparen == peek_tk[:kind] # ProcObjectInConstant::()
        parse_method_or_yield_parameters
        break
      end
      name_t = get_tk
      unless :on_const == name_t[:kind] || :on_ident == name_t[:kind]
        raise RDoc::Error, "Invalid class or module definition: #{given_name}"
      end
      if prev_container == container and !ignore_constants
        given_name = name_t[:text]
      else
        given_name << '::' + name_t[:text]
      end
    end

    skip_tkspace_without_nl

    return [container, name_t, given_name, new_modules]
  end

  ##
  # Return a superclass, which can be either a constant of an expression

  def get_class_specification
    tk = peek_tk
    if tk.nil?
      return ''
    elsif :on_kw == tk[:kind] && 'self' == tk[:text]
      return 'self'
    elsif :on_gvar == tk[:kind]
      return ''
    end

    res = get_constant

    skip_tkspace_without_nl

    get_tkread # empty out read buffer

    tk = get_tk
    return res unless tk

    case tk[:kind]
    when :on_nl, :on_comment, :on_embdoc, :on_semicolon then
      unget_tk(tk)
      return res
    end

    res += parse_call_parameters(tk)
    res
  end

  ##
  # Parse a constant, which might be qualified by one or more class or module
  # names

  def get_constant
    res = ""
    skip_tkspace_without_nl
    tk = get_tk

    while tk && ((:on_op == tk[:kind] && '::' == tk[:text]) || :on_const == tk[:kind]) do
      res += tk[:text]
      tk = get_tk
    end

    unget_tk(tk)
    res
  end

  ##
  # Get an included module that may be surrounded by parens

  def get_included_module_with_optional_parens
    skip_tkspace_without_nl
    get_tkread
    tk = get_tk
    end_token = get_end_token tk
    return '' unless end_token

    nest = 0
    continue = false
    only_constant = true

    while tk != nil do
      is_element_of_constant = false
      case tk[:kind]
      when :on_semicolon then
        break if nest == 0
      when :on_lbracket then
        nest += 1
      when :on_rbracket then
        nest -= 1
      when :on_lbrace then
        nest += 1
      when :on_rbrace then
        nest -= 1
        if nest <= 0
          # we might have a.each { |i| yield i }
          unget_tk(tk) if nest < 0
          break
        end
      when :on_lparen then
        nest += 1
      when end_token[:kind] then
        if end_token[:kind] == :on_rparen
          nest -= 1
          break if nest <= 0
        else
          break if nest <= 0
        end
      when :on_rparen then
        nest -= 1
      when :on_comment, :on_embdoc then
        @read.pop
        if :on_nl == end_token[:kind] and "\n" == tk[:text][-1] and
          (!continue or (tk[:state] & RDoc::Parser::RipperStateLex::EXPR_LABEL) != 0) then
          break if !continue and nest <= 0
        end
      when :on_comma then
        continue = true
      when :on_ident then
        continue = false if continue
      when :on_kw then
        case tk[:text]
        when 'def', 'do', 'case', 'for', 'begin', 'class', 'module'
          nest += 1
        when 'if', 'unless', 'while', 'until', 'rescue'
          # postfix if/unless/while/until/rescue must be EXPR_LABEL
          nest += 1 unless (tk[:state] & RDoc::Parser::RipperStateLex::EXPR_LABEL) != 0
        when 'end'
          nest -= 1
          break if nest == 0
        end
      when :on_const then
        is_element_of_constant = true
      when :on_op then
        is_element_of_constant = true if '::' == tk[:text]
      end
      only_constant = false unless is_element_of_constant
      tk = get_tk
    end

    if only_constant
      get_tkread_clean(/\s+/, ' ')
    else
      ''
    end
  end

  ##
  # Little hack going on here. In the statement:
  #
  #   f = 2*(1+yield)
  #
  # We see the RPAREN as the next token, so we need to exit early.  This still
  # won't catch all cases (such as "a = yield + 1"

  def get_end_token tk # :nodoc:
    case tk[:kind]
    when :on_lparen
      token = RDoc::Parser::RipperStateLex::Token.new
      token[:kind] = :on_rparen
      token[:text] = ')'
      token
    when :on_rparen
      nil
    else
      token = RDoc::Parser::RipperStateLex::Token.new
      token[:kind] = :on_nl
      token[:text] = "\n"
      token
    end
  end

  ##
  # Retrieves the method container for a singleton method.

  def get_method_container container, name_t # :nodoc:
    prev_container = container
    container = container.find_module_named(name_t[:text])

    unless container then
      constant = prev_container.constants.find do |const|
        const.name == name_t[:text]
      end

      if constant then
        parse_method_dummy prev_container
        return
      end
    end

    unless container then
      # TODO seems broken, should starting at Object in @store
      obj = name_t[:text].split("::").inject(Object) do |state, item|
        state.const_get(item)
      end rescue nil

      type = obj.class == Class ? RDoc::NormalClass : RDoc::NormalModule

      unless [Class, Module].include?(obj.class) then
        warn("Couldn't find #{name_t[:text]}. Assuming it's a module")
      end

      if type == RDoc::NormalClass then
        sclass = obj.superclass ? obj.superclass.name : nil
        container = prev_container.add_class type, name_t[:text], sclass
      else
        container = prev_container.add_module type, name_t[:text]
      end

      record_location container
    end

    container
  end

  ##
  # Extracts a name or symbol from the token stream.

  def get_symbol_or_name
    tk = get_tk
    case tk[:kind]
    when :on_symbol then
      text = tk[:text].sub(/^:/, '')

      next_tk = peek_tk
      if next_tk && :on_op == next_tk[:kind] && '=' 