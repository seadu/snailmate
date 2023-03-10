
require_relative '../../spec_helper'

ruby_version_is ""..."3.1" do
  require 'net/ftp'

  describe "Net::FTPProtoError" do
    it "is an Exception" do
      Net::FTPProtoError.should < Exception
    end

    it "is a subclass of Net::FTPError" do
      Net::FTPPermError.should < Net::FTPError
    end
  end
end