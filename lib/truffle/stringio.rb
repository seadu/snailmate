# frozen_string_literal: false
# truffleruby_primitives: true

# Copyright (c) 2013, Brian Shirai
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. Neither the name of the library nor the names of its contributors may be
#    used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
require 'truffle/cext'

Truffle::CExt.rb_define_module_under(IO, 'generic_readable').module_eval do
  # This is why we need undefined in Ruby
  Undefined = Object.new

  def readchar
    raise EOFError, 'end of file reached' if eof?
    getc
  end

  def readbyte
    readchar.getbyte(0)
  end

  def readline(sep=$/, limit=::Undefined, chomp: false)
    check_readable
    raise EOFError, 'end of file reached' if eof?

    Primitive.io_last_line_set(Primitive.caller_special_variables, getline(false, sep, limit, chomp))
  end

  def sysread(length = nil, buffer = nil)
    str = read(length, buffer)

    if str.nil?
      raise EOFError, 'end of file reached'
    end

    str
  end
  alias_method :readpartial, :sysread

  def read_nonblock(length, buffer = nil, exception: true)
    str = read(length, buffer)

    if exception and str.nil?
      raise EOFError, 'end of file reached'
    end

    str
  end

end

Truffle::CExt.rb_define_module_under(IO, 'generic_writable').module_eval do
  def <<(str)
    write(str)
    self
  end

  def print(*args)
    check_writable
    args << Primitive.io_last_line_get(Primitive.caller_special_variables) if args.empty?
    write((args << $\).flatten.join)
    nil
  end

  def printf(*args)
    check_writable

    write(sprintf(*args))

    nil
  end

  def puts(*args)
    Truffle::IOOperations.puts(self, *args)
  end

  def write_nonblock(str, exception: true)
    write(str)
  end

  def syswrite(str)
    write(str)
  end
end

class StringIO

  include Enumerable
  include Truffle::CExt.rb_define_module_under(IO, 'generic_readable')
  include Truffle::CExt.rb_define_module_under(IO, 'generic_writable')

  DEFAULT_RECORD_SEPARATOR = "\n" unless defined?(::DEFAULT_RECORD_SEPARATOR)

  class Data
    attr_accessor :string, :pos, :lineno, :encoding

    def initialize(string)
      @string = string
      @encoding = string.encoding
      @pos = @lineno = 0
    end
  end

  def self.open(*args)
    io = new(*args)
    return io unless block_given?

    begin
      yield io
    ensure
      io.close
      io.__data__.string = nil
      self
    end
  end

  attr_reader :__data__

  def initialize(string=nil, mode=nil, **options)
    if block_given?
      warn 'StringIO::new() does not take block; use StringIO::open() instead'
    end

    mode, _binary, _external, _internal, _autoclose, _perm = IO.normalize_options(mode, nil, options)

    if string.nil?
      mode ||= IO::RDWR
      @__data__ = Data.new ''.force_encoding(Encoding.default_external)
    else
      string = Truffle::Type.coerce_to string, String, :to_str
      @__data__ = Data.new string
    end

    if mode
      if mode.is_a?(Integer)
        mode_from_integer(mode)
      else
        mode = StringValue(mode)
        mode_from_string(mode)
      end
    else
      mode_from_string(string.frozen? ? 'r' : 'r+')
    end

    self
  end

  def initialize_copy(from)
    from = Truffle::Type.coerce_to(from, StringIO, :to_strio)

    @append = from.instance_variable_get(:@append)
    @readable = from.instance_variable_get(:@readable)
    @writable = from.instance_variable_get(:@writable)
    @__data__ = from.instance_variable_get(:@__data__)

    self
  end

  # StringIO#inspect should not include the contents of the StringIO
  alias_method :inspect, :to_s

  private def check_readable
    raise IOError, 'not opened for reading' unless @readable
  end

  private def check_writable
    raise IOError, 'not opened for writing' unless @writable
    raise IOError, 'unable to modify data' if @__data__.string.frozen?
  end

  def set_encoding(external, internal=nil, options=nil)
    encoding = external || Encoding.default_external
    d = @__data__
    TruffleRuby.synchronized(d) do
      d.encoding = encoding
      d.string.force_encoding(encoding) if @writable
    end

    self
  end

  def external_encoding
    @__data__.encoding
  end

  def internal_encoding
    nil
  end

  def each_byte
    return to_enum :each_byte unless block_given?
    check_readable

    d = @__data__
    string = d.string

    while d.pos < string.bytesize
      check_readable
      byte = string.getbyte d.pos
      d.pos += 1
      yield byte
    end

    self
  end

  def each_char
    return to_enum :each_char unless block_given?
    while s = getc
      yield s
    end

    self
  end

  def each_codepoint(&block)
    return to_enum :each_codepoint unless block_given?
    check_readable

    d = @__data__
    string = d.string

    while d.pos < string.bytesize
      check_readable
      char = Primitive.string_chr_at(string, d.pos)

      unless char
        raise ArgumentError, "invalid byte sequence in #{d.encoding}"
      end

      d.pos += char.bytesize
      yield char.ord
    end

    self
  end

  def each(sep=$/, limit=Undefined, chomp: false)
    return to_enum :each, sep, limit, chomp: chomp unless block_given?
    check_readable

    while line = getline(true, sep, limit, chomp)
      yield line
    end

    self
  end
  alias_method :each_line, :each

  def binmode
    set_encoding(Encoding::BINARY)
    self
  end

  def write(str)
    check_writable

    str = String(str)
    return 0 if str.empty?

    d = @__data__
    TruffleRuby.synchronized(d) do
      pos = d.pos
      string = d.string
      bytesize = string.bytesize

      if @append || pos == bytesize
        Primitive.string_byte_append(string, str)
        d.pos = string.bytesize
      elsif pos > bytesize
        replacement = "\000" * (pos - bytesize)
        Primitive.string_byte_append(string, replacement)
        Primitive.string_byte_append(string, str)
        d.pos = string.bytesize
      else
        bytes_to_replace = str.bytesize
        bytes_after = bytesize - pos
        if bytes_to_replace > bytes_after
          bytes_to_replace = bytes_after
        end

        enc = string.encoding
        str_in_enc = str.dup.force_encoding(enc)
        Primitive.string_splice(string, str_in_enc, pos, bytes_to_replace, enc)
        d.pos += str.bytesize
      end

      str.bytesize
    end
  end

  def close
    @readable = @writable = nil
  end

  def closed?
    !@readable && !@writable
  end

  def close_read
    check_readable
    @readable = nil
  end

  def closed_read?
    !@readable
  end

  def close_write
    check_writable
    @writable = nil
  end

  def closed_write?
    !@writable
  end

  def eof?
    d = @__data__
    TruffleRuby.synchronized(d) do
      d.pos >= d.string.bytesize
    end
  end
  alias_method :eof, :eof?

  def fcntl
    raise NotImplementedError, 'StringIO#fcntl is not implemented'
  end

  def fileno
    nil
  end

  def flush
    self
  end

  def fsync
    0
  end

  def getc
    check_readable
    return nil if eof?

    d = @__data__
    TruffleRuby.synchronized(d) do
      char = Primitive.string_find_character(d.string, d.pos)
      d.pos += char.bytesize
      char
    end
  end

  def getbyte
    check_readable
    return nil if eof?

    d = @__data__
    TruffleRuby.synchronized(d) do
      byte = d.string.getbyte(d.pos)
      d.pos += 1
      byte
    end
  end

  def gets(sep=$/, limit=Undefined, chomp: false)
    check_readable

    # Truffle: $_ is thread and frame local, so we use a primitive to
    # set it in the caller's frame.
    Primitive.io_last_line_set(Primitive.caller_special_variables, getline(false, sep, limit, chomp))
  end

  def isatty
    false
  end
  alias_method :tty?, :isatty

  def lineno
    @__data__.lineno
  end

  def lineno=(line)
    @__data__.lineno = line
  end

  def pid
    nil
  end

  def pos
    @__data__.pos
  end

  def pos=(pos)
    raise Errno::EINVAL if pos < 0
    @__data__.pos = pos
  end

  def putc(obj)
    check_writable

    if obj.is_a?(String)
      char = obj[0]
    else
      c = Truffle::Type.coerce_to obj, Integer, :to_int
      char = (c & 0xff).chr
    end

    write(char)
    obj
  end

  def read(length = nil, buffer = nil)
    check_readable
    d = @__data__
    TruffleRuby.synchronized(d) do
      pos = d.pos
      string = d.string

      if length
        length = Truffle::Type.coerce_to length, Integer, :to_int
        raise Argu