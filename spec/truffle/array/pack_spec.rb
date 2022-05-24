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
    check_pack_equal_bytes("\x92\x01",([146] << 1).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("v",([] << 118).pack('C*'))
    check_pack_equal_bytes("\x02",([] << 2).pack('C*'))
    check_pack_equal_bytes("\x9A\x01",([154] << 1).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("w",([] << 119).pack('C*'))
    check_pack_equal_bytes("\x02",([] << 2).pack('C*'))
    check_pack_equal_bytes("\xA2\x01",([162] << 1).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("x",([] << 120).pack('C*'))
    check_pack_equal_bytes("\x02",([] << 2).pack('C*'))
    check_pack_equal_bytes("\xA8\x01",([168] << 1).pack('C*'))
    check_pack_equal_bytes("\x03",[3].pack('C'))
    check_pack_equal_bytes("\xB0\x01",([176] << 1).pack('C*'))
    check_pack_equal_bytes("\x06",[6].pack('C'))
    check_pack_equal_bytes("\xB8\x01",([184] << 1).pack('C*'))
    check_pack_equal_bytes("\t",[9].pack('C'))
    check_pack_equal_bytes("\xC2\x01",([194] << 1).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\xCA\x01",([202] << 1).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\xD2\x01",([210] << 1).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("~",([] << 126).pack('C*'))
    check_pack_equal_bytes("\x02",([] << 2).pack('C*'))
    check_pack_equal_bytes("\xDA\x01",([218] << 1).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("\x7F",([] << 127).pack('C*'))
    check_pack_equal_bytes("\x02",([] << 2).pack('C*'))
    check_pack_equal_bytes("\xF8\x01",([248] << 1).pack('C*'))
    check_pack_equal_bytes("\xC9\x01",([201] << 1).pack('C*'))
    check_pack_equal_bytes("\xF8\x01",([248] << 1).pack('C*'))
    check_pack_equal_bytes("\xAD\x02",([173] << 2).pack('C*'))
    check_pack_equal_bytes("\x80\x02",([128] << 2).pack('C*'))
    check_pack_equal_bytes("\xCA\x01",([202] << 1).pack('C*'))
    check_pack_equal_bytes("\x80\x02",([128] << 2).pack('C*'))
    check_pack_equal_bytes("\xAE\x02",([174] << 2).pack('C*'))
    check_pack_equal_bytes("\x88\x02",([136] << 2).pack('C*'))
    check_pack_equal_bytes("\xCB\x01",[203, 1].pack('C*'))
    check_pack_equal_bytes("\x88\x02",([136] << 2).pack('C*'))
    check_pack_equal_bytes("\xAF\x02",[175, 2].pack('C*'))
    check_pack_equal_bytes("\x90\x02",([144] << 2).pack('C*'))
    check_pack_equal_bytes("\xCC\x01",[204, 1].pack('C*'))
    check_pack_equal_bytes("\x90\x02",([144] << 2).pack('C*'))
    check_pack_equal_bytes("\xB0\x02",[176, 2].pack('C*'))
    check_pack_equal_bytes("\x98\x02",([152] << 2).pack('C*'))
    check_pack_equal_bytes("\x9A\x03",([154] << 3).pack('C*'))
    check_pack_equal_bytes("\x98\x02",([152] << 2).pack('C*'))
    check_pack_equal_bytes("\xE2\x04",([226] << 4).pack('C*'))
    check_pack_equal_bytes("\xA0\x02",([160] << 2).pack('C*'))
    check_pack_equal_bytes("\x9C\x03",([156] << 3).pack('C*'))
    check_pack_equal_bytes("\xA0\x02",([160] << 2).pack('C*'))
    check_pack_equal_bytes("\xE4\x04",([228] << 4).pack('C*'))
    check_pack_equal_bytes("\xAD\x02",([173] << 2).pack('C*'))
    check_pack_equal_bytes("\xCF\x00\x00\x00",[207].pack('V'))
    check_pack_equal_bytes("\xAD\x02",([173] << 2).pack('C*'))
    check_pack_equal_bytes("3\x01\x00\x00",[307].pack('V'))
    check_pack_equal_bytes("\xB1\x02",([177] << 2).pack('C*'))
    check_pack_equal_bytes("\xD0\x00\x00\x00\x00\x00\x00\x00",[208 & 0xffff_ffff, 208 >> 32].pack('VV'))
    check_pack_equal_bytes("\xB1\x02",([177] << 2).pack('C*'))
    check_pack_equal_bytes("4\x01\x00\x00\x00\x00\x00\x00",[308 & 0xffff_ffff, 308 >> 32].pack('VV'))
    check_pack_equal_bytes("\xBD\x02",([189] << 2).pack('C*'))
    check_pack_equal_bytes("\xD1\x00\x00\x00", [209].pack('V'))
    check_pack_equal_bytes("\xBD\x02",([189] << 2).pack('C*'))
    check_pack_equal_bytes("5\x01\x00\x00", [309].pack('V'))
    check_pack_equal_bytes("\xC1\x02",([193] << 2).pack('C*'))
    check_pack_equal_bytes("\xD2\x00\x00\x00\x00\x00\x00\x00",[210 & 0xffff_ffff, 210 >> 32].pack('VV'))
    check_pack_equal_bytes("\xC1\x02",([193] << 2).pack('C*'))
    check_pack_equal_bytes("6\x01\x00\x00\x00\x00\x00\x00",[310 & 0xffff_ffff, 310 >> 32].pack('VV'))
    check_pack_equal_bytes("\xCD\x02",([205] << 2).pack('C*'))
    check_pack_equal_bytes("\x00\x00SC", [211].pack('e'))
    check_pack_equal_bytes("\xCD\x02",([205] << 2).pack('C*'))
    check_pack_equal_bytes("\x00\x80\x9BC", [311].pack('e'))
    check_pack_equal_bytes("\xD1\x02",([209] << 2).pack('C*'))
    check_pack_equal_bytes("\x00\x00\x00\x00\x00\x80j@",[212].pack('E'))
    check_pack_equal_bytes("\xD1\x02",([209] << 2).pack('C*'))
    check_pack_equal_bytes("\x00\x00\x00\x00\x00\x80s@",[312].pack('E'))
    check_pack_equal_bytes("\xD8\x02",([216] << 2).pack('C*'))
    check_pack_equal_bytes("\xD8\x02",([216] << 2).pack('C*'))
    check_pack_equal_bytes("\xE2\x02",([226] << 2).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\xE2\x02",([226] << 2).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\xEA\x02",([234] << 2).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\xEA\x02",([234] << 2).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\x82\x03",([130] << 3).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("\xDA\x01",([218] << 1).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\x82\x03",([130] << 3).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("\xBE\x02",([190] << 2).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\x8A\x03",([138] << 3).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("\xDB\x01",([219] << 1).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\x8A\x03",([138] << 3).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("\xBF\x02",([191] << 2).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\x92\x03",([146] << 3).pack('C*'))
    check_pack_equal_bytes("\b",([] << 8).pack('C*'))
    check_pack_equal_bytes("\xDC\x01",([220] << 1).pack('C*'))
    check_pack_equal_bytes("\x03",([] << 3).pack('C*'))
    check_pack_equal_bytes("\x92\x03",([146] << 3).pack('C*'))
    check_pack_equal_