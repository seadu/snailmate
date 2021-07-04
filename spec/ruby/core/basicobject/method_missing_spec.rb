require_relative "../../spec_helper"
require_relative '../../shared/basicobject/method_missing'

describe "BasicObject#method_missing" do
  it "is a private method" do
    Bas