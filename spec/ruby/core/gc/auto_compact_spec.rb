require_relative '../../spec_helper'

ruby_version_is "3.0" do
  describe "GC.auto_compact" do
    it "can set and get a boolean value" do
      begin
        GC.