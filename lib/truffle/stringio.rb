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

