# -*- encoding: utf-8 -*-
require_relative '../../spec_helper'
require_relative 'fixtures/classes'

describe "String#chomp" do
  describe "when passed no argument" do
    before do
      # Ensure that $/ is set to the default value
      @verbose, $VERBOSE = $VERBOSE, nil
      @dollar_slash, $/ = $/, "\n"
    end

    after do
      $/ = @dollar_slash
      $VERBOSE = @verbose
    end

    it "does not modify a String with no trailing carriage return or newline" do
      "abc".chomp.should == "abc"
    end

    it "returns a copy of the String when it is not modified" do
      str = "abc"
      str.chomp.should_not equal(str)
    end

    it "removes one trailing newline" do
      "abc\n\n".chomp.should == "abc\n"
    end

    it "removes one trailing carriage return" do
      "abc\r\r".chomp.should == "abc\r"
    end

    it "removes one trailing carriage return, newline pair" do
      "abc\r\n\r\n".chomp.should == "abc\r\n"
    end

    it "returns an empty String when self is empty" do
      "".chomp.should == ""
    end

    it "returns a String in the same encoding as self" do
      "abc\n\n".encode("US-ASCII").chomp.encoding.should == Encoding::US_ASCII
    end

    ruby_version_is ''...'3.0' do
      it "returns subclass instances when called on a subclass" do
        str = StringSpecs::MyString.new("hello\n").chomp
        str.should be_an_instance_of(StringSpecs::MyString)
      end
    end

    ruby_version_is '3.0' do
      it "returns String instances when called on a subclass" do
        str = StringSpecs::MyString.new("hello\n").chomp
        str.should be_an_instance_of(String)
      end
    end

    it "removes trailing characters that match $/ when it has been assigned a value" do
      $/ = "cdef"
      "abcdef".chomp.should == "ab"
    end

    it "removes one trailing newline for string with invalid encoding" do
      "\xa0\xa1\n".chomp.should == "\xa0\xa1"
    end
  end

  describe "when passed nil" do
    it "does not modify the String" do
      "abc\r\n".chomp(nil).should == "abc\r\n"
    end

    it "returns a copy of the String" do
      str = "abc"
      st