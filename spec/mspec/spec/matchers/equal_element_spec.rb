require 'spec_helper'
require 'mspec/expectations/expectations'
require 'mspec/matchers'

RSpec.describe EqualElementMatcher do
  it "matches if it finds an element with the passed name, no matter what attributes/content" do
    expect(EqualElementMatcher.new("A").matches?('<A></A>')).to be_truthy
    expect(EqualElementMatcher.new("A").matches?('<A HREF="http://example.com"></A>')).to be_truthy
    expect(EqualElementMatcher.new("A").matches?('<A HREF="http://example.com"></A>')).to be_truthy

    expect(EqualElementMatcher.new("BASE").matches?('<BASE></A>')).to be_falsey
    expect(EqualElementMatcher.new("BASE").matches?('<A></BASE>')).to be_falsey
    expect(EqualElementMatcher.new("BASE").matches?('<A></A>')).to be_falsey
    expect(EqualElementMatcher.new("BASE").matches?('<A HREF="http://example.com"></A>')).to be_falsey
    expect(EqualElementMatcher.new("BASE").matches?('<A HREF="http://example.com"></A>')).to be_falsey
  end

  it "matches if it finds an element with the passed name and the passed attributes" do
    expect(EqualElementMatcher.new("A", {}).matches?('<A></A>')).to be_truthy
    expect(EqualElementMatcher.new("A", nil).matches?('<A HREF="http://example.com"></A>')).to be_truthy
    expect(EqualElementMatcher.new("A", "HREF" => "http://example.com").matches?('<A HREF="http://example.com"></A>')).to be_truthy

    expect(EqualElementMatcher.new("A", {}).matches?('<A HREF="http://example.com"></A>')).to be_falsey
    expect(EqualElementMatcher.new("A", "HREF" => "http://example.com").matches?('<A></A>')).to be_falsey
    expect(EqualElementMatcher.new("A", "HREF" => "http://example.com").matches?('<A HREF="http://test.com"></A>')).to be_falsey
    expect(EqualElementMatcher.new("A", "HREF" => "http://example.com").matches?('<A H