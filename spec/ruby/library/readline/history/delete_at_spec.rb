require_relative '../spec_helper'

with_feature :readline do
  describe "Readline::HISTORY.delete_at" do
    it "deletes and returns the history entry at the specified index" do
      Readline::HISTORY.push("1", "2", "3")

      Readline::HISTORY.delete_at(1).should == "2"
      Readline::HISTORY.size.should == 2

      Readline::HISTORY.delete_at(1).should == "3"
      Readline::HISTORY.size.should == 1

      Readline::HISTORY.delete_at(0).should == "1"
      Readline::HISTORY.size.should == 0


      Readline::HISTORY.push("1", "2", "3", "4")

      Readline::HISTORY.delete_at(-2).should == "3"
      Readline::HISTORY.size.should 