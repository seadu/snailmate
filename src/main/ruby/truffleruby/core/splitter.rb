# frozen_string_literal: true

# Copyright (c) 2016, 2023 Oracle and/or its affiliates. All rights reserved. This
# code is released under a tri EPL/GPL/LGPL license. You can use it,
# redistribute it and/or modify it under the terms of the:
#
# Eclipse Public License version 2.0, or
# GNU General Public License version 2, or
# GNU Lesser General Public License version 2.1.
#
# Copyright (c) 2007-2015, Evan Phoenix and contributors
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# * Neither the name of Rubinius nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
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

module Truffle
  class Splitter
    DEFAULT_PATTERN = ' '

    class << self
      def split(string, pattern, limit, &orig_block)
        result = []
        # To simplify the code, we present a single block
        # that either calls user (origin) block or adds a substring to the resulting array
        # See motivation: https://github.com/oracle/truffleruby/pull/2052#issuecomment-663449395
        block = orig_block || result.method(:<<).to_proc

        return (orig_block ? string : result) if string.empty?

        if Primitive.undefined?(limit)
          limit = 0
        else
          limit = Primitive.rb_num2int(limit)
        end

        if limit == 1
          dup_string = string.dup

          block.call(dup_string)
          return orig_block ? dup_string : result
        end

        if Primitive.nil?(pattern)
          pattern = ($; || DEFAULT_PATTERN)
        end

        if pattern == DEFAULT_PATTERN
          awk_limit = limit < 0 ? -1 : limit

          # We pass the original block to string_awk_split to handle it on Java side
          # See motivation: https://github.com/oracle/truffleruby/pull/2052#issuecomment-663494235
          return Primitive.string_awk_split string, awk_limit, orig_block
        elsif Primitive.object_kind_of?(pattern, Regexp)
          split