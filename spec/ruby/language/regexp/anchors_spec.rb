require_relative '../../spec_helper'
require_relative '../fixtures/classes'

describe "Regexps with anchors" do
  it "supports ^ (line start anchor)" do
    # Basic matching
    /^foo/.match("foo").to_a.should == ["foo"]
    /^bar/.match("foo\nbar").to_a.should == ["bar"]
    # Basic non-matching
    /^foo/.match(" foo").should be_nil
    /foo^/.match("foo\n\n\n").should be_nil

    # A bit advanced
    /^^^foo/.match("foo").to_a.should == ["foo"]
    (/^[^f]/ =~ "foo\n\n").should == "foo\n".size and $~.to_a.should == ["\n"]
    (/($^)($^)/ =~ "foo\n\n").should == "foo\n".size and $~.to_a.should == ["", "", ""]

    # Different start of line chars
    /^bar/.match("foo\rbar").should be_nil
    /^bar/.match("foo\0bar").should be_nil

    # Trivial
    /^/.match("foo").to_a.should == [""]

    # Grouping
    /(^foo)/.match("foo").to_a.should == ["foo", "foo"]
    /(^)/.match("foo").to_a.should == ["", ""]
    /(foo\n^)(^bar)/.match("foo\nbar").to_a.should == ["foo\nbar", "foo\n", "bar"]
  end

  it "does not match ^ after trailing \\n" do
    /^(?!\A)/.match("foo\n").should be_nil # There is no (empty) line after a trailing \n
  end

  it "supports $ (line end anchor)" do
    # Basic  matching
    /foo$/.match("foo").to_a.should == ["foo"]
    /foo$/.match("foo\nbar").to_a.should == ["foo"]
    # Basic non-matching
    /foo$/.match("foo ").should be_nil
    /$foo/.match("\n\n\nfoo").should be_nil

    # A bit advanced
    /foo$$$/.match("foo").to_a.should == ["foo"]
    (/[^o]$/ =~ "foo\n\n").should == ("foo\n".size - 1) and $~.to_a.should == ["\n"]

    # Different end of line chars
    /foo$/.match("foo\r\nbar").should be_nil
    /foo$/.match("foo\0bar").should be_nil

    # Trivial
    (/$/ =~ "foo").should == "foo".size and $~.to_a.should == [""]

    # Grouping
    /(foo$)/.match("foo").to_a.should == ["foo", "foo"]
    (/($)/ =~ "foo").should == "foo".size and $~.to_a.should == ["", ""]
    /(foo$)($\nbar)/.match("foo\nbar").to_a.should == ["foo\nbar", "foo", "\nbar"]
  end

  it "supports \\A (string start anchor)" do
    # Basic matching
    /\Afoo/.match("foo").to_a.should == ["foo"]
    # Basic non-matching
    /\Abar/.match("foo\nbar").should be_nil
    /\Afoo/.match(" foo").should be_nil

    # A bit advanced
    /\A\A\Afoo/.match("foo").to_a.should == ["foo"]
    /(\A\Z)(\A\Z)/.match("").to_a.should == ["", "", ""]

    # Different start of line chars
    /\Abar/.match("foo\0bar").should be_nil

    # Grouping
    /(\Afoo)/.match("foo").to_a.should == ["foo", "foo"]
    /(\A)/.match("foo").to_a.should == ["", ""]
  end

  it "supports \\Z (string end anchor, including before trailing \\n)" do
    # Basic matching
    /foo\Z/.match("foo").to_a.should == ["foo"]
    /foo\Z/.match("foo\n").to_a.should == ["foo"]
    # Basic non-matching
    /foo\Z/.match("foo\nbar").should be_nil
    /foo\Z/.match("foo ").should be_nil

    # A bit advanced
    /foo\Z\Z\Z/.match("foo\n").to_a.should == ["foo"]
    (/($\Z)($\Z)/ =~ "foo\n").should == "foo".size and $~.to_a.should == ["", "", ""]
    (/(\z\Z)(\z\Z)/ =~ "foo\n").should == "foo\n".size and $~.to_a.should == ["", "", ""]

    # Different end of line chars
    /foo\Z/.match("fo