#
# This file is part of ruby-ffi.
# For licensing, see LICENSE.SPECS
#

require File.expand_path(File.join(File.dirname(__FILE__), "spec_helper"))

describe "Function with primitive integer arguments" do
  module LibTest
    extend FFI::Library
    ffi_lib TestLibrary::PATH
    attach_function :ret_s8, [ :char ], :char
    attach_function :ret_u8, [ :uchar ], :uchar
    attach_function :ret_s16, [ :short ], :short
    attach_function :ret_u16, [ :ushort ], :ushort
    attach_function :ret_s32, [ :int ], :int
    attach_function :ret_u32, [ :uint ], :uint
    attach_function :ret_s64, [ :long_long ], :long_long
    attach_function :ret_u64, [ :ulong_long ], :ulong_long
    attach_function :ret_long, [ :long ], :long
    attach_function :ret_ulong, [ :ulong ], :ulong
    attach_function :set_s8, [ :char ], :void
    attach_function :get_s8, [ ], :char
    attach_function :set_float, [ :float ], :void
    attach_function :get_float, [ ], :float
    attach_function :set_double, [ :double ], :void
    attach_function :get_double, [ ], :double
  end

  it "int8.size" do
    expect(FFI::TYPE_INT8.size).to eq(1)
  end

  it "uint8.size" do
    expect(FFI::TYPE_UINT8.size).to eq(1)
  end

  it "int16.size" do
    expect(FFI::TYPE_INT16.size).to eq(2)
  end

  it "uint16.size" do
    expect(FFI::TYPE_UINT16.size).to eq(2)
  end

  it "int32.size" do
    expect(FFI::TYPE_INT32.size).to eq(4)
  end

  it "uint32.size" do
    expect(FFI::TYPE_UINT32.size).to eq(4)
  end

  it "int64.size" do
    expect(FFI::TYPE_INT64.size).to eq(8)
  end

  it "uint64.size" do
    expect(FFI::TYPE_UINT64.size).to eq(8)
  end

  it "float.size" do
    expect(FFI::TYPE_FLOAT32.size).to eq(4)
  end

  it "double.size" do
    expect(FFI::TYPE_FLOAT64.size).to eq(8)
  end
  [ 0, 127, -128, -1 ].each do |i|
    it ":char call(:char (#{i}))" do
      expect(LibTest.ret_s8(i)).to eq(i)
    end
  end
  [ 0, 0x7f, 0x80, 0xff ].each do |i|
    it ":uchar call(:uchar (#{i}))" do
      expect(LibTest.ret_u8(i)).to eq(i)
    end
  end
  [ 0, 0x7fff, -0x8000, -1 ].each do |i|
    it ":short call(:short (#{i}))" do
      expect(LibTest.ret_s16(i)).to eq(i)
    end
  end
  [ 0, 0x7fff, 0x8000, 0xffff ].each do |i|
    it ":ushort call(:ushort (#{i}))" do
      expect(LibTest.ret_u16(i)).to eq(i)
    end
  end
  [ 0, 0x7fffffff, -0x80000000, -1 ].each do |i|
    it ":int call(:int (#{i}))" do
      expect(LibTest.ret_s32(i)).to eq(i)
    end
  end
  [ 0, 0x7fffffff, 0x80000000, 0xffffffff ].each do |i|
    it ":uint call(:uint (#{i}))" do
      expect(LibTest.ret_u32(i)).to eq(i)
    end
  end
  [ 0, 0x7fffffffffffffff, -0x8000000000000000, -1 ].each do |i|
    it ":long_long call(:long_long (#{i}))" do
      expect(LibTest.ret_s64(i)).to eq(i)
    end
  end
  [ 0, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff ].each do |i|
    it ":ulong_long call