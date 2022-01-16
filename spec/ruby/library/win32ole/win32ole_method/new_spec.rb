require_relative "../../../spec_helper"
platform_is :windows do
  require 'win32ole'

  describe "WIN32OLE_METHOD.new" do
    before :each do
      @ole_type = WIN32OLE_TYPE.new("Microsoft Shell Controls And Automation", "Shell")
    end

    it "raises TypeError when given non-strings" do
      -> { WIN32OLE_METHOD.new(1, 2) }.should raise_error TypeError
    end

    it "raises ArgumentError if only 1 argument is given" do
      -> { WIN32OLE_METHOD.new("hello") }.should raise_error ArgumentError
      -> { W