# Copyright (c) 2016, 2023 Oracle and/or its affiliates. All rights reserved. This
# code is released under a tri EPL/GPL/LGPL license. You can use it,
# redistribute it and/or modify it under the terms of the:
#
# Eclipse Public License version 2.0, or
# GNU General Public License version 2, or
# GNU Lesser General Public License version 2.1.
# OTHER DEALINGS IN THE SOFTWARE.

require_relative '../../ruby/spec_helper'

def check_pack_equal(expected, actual)
  actual.should == expected
end

def check_pack_equal_bytes(expected, actual)
  actual.should == expected.b
end

describe "Array#pack" do

  it "should pack expressions with loops" do
    ([1] * 1024).pack('l' * 1024).sum.should == 1024
  end

  it "should be able to pack a tarball entry" do
    ary = ["metadata.gz", "0000444", "0000000", "0000000", "00000001244", "00000000044", "        ", " ", "0", nil,
           "ustar", "00", "wheel", "wheel", "0000000", "0000000", ""]
    pack_format = "a100a8a8a8a12a12a7aaa100a6a2a32a32a8a8a155"
    packed = ary.pack(pack_format)
    path = fixture __FILE__, "tarball_entry.tar"
    packed.should == File.binread(path)
  end

  # Real-world usage of Array#pack, gathered by Brandon Fish, not designed
  # to exercise anything in particular, just found in some real code and
  # another good layer of testing.

  it "should correctly pack real-world examples" do
    check_pack_equal_bytes("z",([] << 122).pack('C*'))
    check_pack_equal_bytes("\x12",([] << 18).pack('C*'))
    check_pack_equal_bytes("\x04",([] << 4).pack('C*'))
    check_pack_equal_bytes("\x06",([] << 6).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("e",([] << 101).pack('C*'))
    check_pack_equal_bytes("\x10",([] << 16).pack('C*'))
    check_pack_equal_bytes("f",([] << 102).pack('C*'))
    check_pack_equal_bytes("\x18",([] << 24).pack('C*'))
    check_pack_equal_bytes("g",[103].pack('C'))
    check_pack_equal_bytes(" ",([] << 32).pack('C*'))
    check_pack_equal_bytes("h",[104].pack('C'))
    check_pack_equal_bytes("(",([] << 40).pack('C*'))
    check_pack_equal_bytes("\xD2\x01",([210] << 1).pack('C*'))
    check_pack_equal_bytes("0",([] << 48).pack('C*'))
    check_pack_equal_bytes("\xD4\x01",([212] << 1).pack('C*'))
    check_pack_equal_bytes("=",([] << 61).pack('C*'))
    check_pack_equal_bytes("k\x00\x00\x00",[107].pack('V'))
    check_pack_equal_bytes("A",([] << 65).pack('C*'))
    check_pack_equal_bytes("l\x00\x00\x00\x00\x00\x00\x00",[108 & 0xffff_ffff, 108 >> 32].pack('VV'))
    check_pack_equal_bytes("M",([] << 77).pack('C*'))
    check_pack_equal_bytes("m\x00\x00\x00", [109].pack('V'))
    check_pack_equal_bytes("Q",([] << 81).pack('C*'))
    check_pack_equal_bytes("n\x00\x00\x00\x00\x00\x00\x00",[110 & 0xffff_ffff, 110 >> 32].pack('VV'))
    check_pack_equal_bytes("]",([] << 93).pack('C*'))
    check_pack_equal_bytes("\x00\x00\xDEB", [111.0].pack('e'))
    check_pack_equal_bytes("a",([] << 97).pack('C*'))
    check_pack_equal_bytes("\x00\x00\x00\x00\x00\x00\\@",[112.0].pack('E'))
    check_pack_equal_bytes("h",([] << 104).pack('C*'))
    check_pack_equal_bytes("r",([] << 114).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("z",([] << 122).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes(