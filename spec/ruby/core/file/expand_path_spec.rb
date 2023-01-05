# -*- encoding: utf-8 -*-

require_relative '../../spec_helper'
require_relative 'fixtures/common'
require 'etc'

describe "File.expand_path" do
  before :each do
    platform_is :windows do
      @base = `cd`.chomp.tr '\\', '/'
      @tmpdir = "c:/tmp"
      @rootdir = "c:/"
    end

    platform_is_not :windows do
      @base = Dir.pwd
      @tmpdir = "/tmp"
      @rootdir = "/"
    end
  end

  before :each do
    @external = Encoding.default_external
  end

  after :each do
    Encoding.default_external = @external
  end

  it "converts a pathname to an absolute pathnam