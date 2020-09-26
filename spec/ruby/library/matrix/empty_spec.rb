require_relative '../../spec_helper'

ruby_version_is ""..."3.1" do
  require_relative 'fixtures/classes'
  require 'matrix'

  describe "Matrix#empty?" do
    it "returns true when the Matrix is empty" do
      Matrix[ ].empty?.should be_true
      Matrix[ [], [], [] ].empty?.should be_true
      Matrix[ [], [], [] ].transpose.empty?.should be_true
    end

    it "returns false when the Matrix has elements" do
      Matrix[ [1, 2] ].empty?.should be_false
      Matrix[ [1], [2] ].empty?.should be_false
    end

    it "doesn't accept any parameter" do
      ->{
        Matrix[ [1, 2] ].empty?(42)
      }.should raise_error(ArgumentError)
    end
  end

  describe "Matrix.empty" do
    it "returns an empty matrix of the requested size" do
      m = Matrix.empty(3, 0)
      m.row_size.should == 3
      m.column_size.should == 0

      m = Matrix.empty(