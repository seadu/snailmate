require_relative '../../spec_helper'
require_relative 'fixtures/classes'

describe "String#crypt" do
  platform_is :openbsd do
    it "returns a cryptographic hash of self by applying the bcrypt algorithm with the specified salt" do
      "mypassword".crypt("$2a$04$0WVaz0pV3jzfZ5G5tpmHWu").should == "$2a$04$0WVaz0pV3jzfZ5G5tpmHWuBQGbkjzgtSc3gJbmdy0GAGMa45MFM2."

      # Only uses first 72 characters of string
      ("12345678"*9).crypt("$2a$04$0WVaz0pV3jzfZ5G5tpmHWu").should == "$2a$04$0WVaz0pV3jzfZ5G5tpmHWukj/ORBnsMjCGpST/zCJnAypc7eAbutK"
      ("12345678"*10).crypt("$2a$04$0WVaz0pV3jzfZ5G5tpmHWu").should == "$2a$04$0WVaz0pV3jzfZ5G5tpmHWukj/ORBnsMjCGpST/zCJnAypc7eAbutK"

      # Only uses first 29 characters of salt
      "mypassword".crypt("$2a$04$0WVaz0pV3jzfZ5G5tpmHWuB").should == "$2a$04$0WVaz0pV3jzfZ5G5tpmHWuBQGbkjzgtSc3gJbmdy0GAGMa45MFM2."
    end

    it "raises Errno::EINVAL when the salt is shorter than 29 characters" do
      -> { "mypassword".crypt("$2a$04$0WVaz0pV3jzfZ5G5tpmHW") }.should raise_error(Errno::EINVAL)
    end

    it "calls #to_str to converts the salt arg to a String" do
      obj = mock('$2a$04$0WVaz0pV3jzfZ5G5tpmHWu')
      obj.should_receive(:to_str).and_return("$2a$04$0WVaz0pV3jzfZ5G5tpmHWu")

      "mypassword".crypt(obj).should == "$2a$04$0WVaz0pV3jzfZ5G5tpmHWuBQGbkjzgtSc3gJbmdy0GAGMa45MFM2."
    end

    it "doesn't return subclass instances" do
      StringSpecs::MyString.new("mypassword").crypt("$2a$04$0WVaz0pV3jzfZ5G5tpmHWu").should be_an_instance_of(String)
      "mypassword".crypt(StringSpecs::MyString.new("$2a$04$0WVaz0pV3jzfZ5G5tpmHWu")).should be_an_instance_of(String)
      StringSpecs::MyString.new("mypassword").crypt(StringSpecs::MyString.new("$2a$04$0WVaz0pV3jzfZ5G5tpmHWu")).should be_an_instance_of(String)
    end
  end

  platform_is_not :openbsd do
    # Note: MRI's documentation just says that the C stdlib function crypt() is
    # called.
    #
    # I'm not sure if crypt() is guaranteed to produce the same result across
    # different platforms. It seems that there is one standard UNIX implementation
    # of crypt(), but that alternative implementations are possible. See
    # http://www.unix.org.ua/orelly/networking/puis/ch08_06.htm
    it "returns a cryptographic hash of self by applying the UNIX crypt algorithm with the specified s