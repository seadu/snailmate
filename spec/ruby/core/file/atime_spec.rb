require_relative '../../spec_helper'

describe "File.atime" do
  before :each do
    @file = tmp('test.txt')
    touch @file
  end

  after :each do
    rm_r @file
  end

  it "returns the last access time for the named file as a Time object" do
    File.atime(@file)
    File.atime(@file).should be_kind_of(Time)
  end

  platform_is :linux, :windows do
    unless ENV.key?('TRAVIS') # https://bugs.ruby-lang.org/issues/17926
      ## NOTE also that some Linux systems disable atime (e.g. via mount params) for better filesystem speed.
      it "returns the last access time for the named file with microseconds" do
        supports_subseconds = Integer(`stat -c%x