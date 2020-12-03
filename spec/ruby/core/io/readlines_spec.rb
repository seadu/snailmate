# -*- encoding: utf-8 -*-
require_relative '../../spec_helper'
require_relative 'fixtures/classes'
require_relative 'shared/readlines'

describe "IO#readlines" do
  before :each do
    @io = IOSpecs.io_fixture "lines.txt"
    @orig_extenc = Encoding.default_external
    Encoding.default_external = Encoding::UTF_8
  end

  after :each do
    @io.close unless @io.closed?
    Encoding.default_external = @orig_extenc
  end

  it "raises an IOError if the stream is closed" do
    @io.close
    -> { @io.readlin