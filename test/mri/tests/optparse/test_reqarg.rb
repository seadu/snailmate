# frozen_string_literal: false
require_relative 'test_optparse'

module TestOptionParserReqArg
  def setup
    super
    @opt.def_option "--with_underscore=VAL" do |x| @flag = x end
    @opt.def_option "--with-hyphen=VAL" do |x| @flag = x end
  end

  class Def1 < TestOptionParser
    include TestOptionParserReqArg
    def setup
      super
      @opt.def_option("-xVAL") {|x| @flag = x}
      @opt.def_option("--option=VAL") {|x| @flag = x}
      @opt.def_option("--regexp=REGEXP", Regexp) {|x| @reopt = x}
      @reopt = nil
    end
  end
  class Def2 < TestOptionParser
    include TestOptionParserReqArg
    def setup
      super
      @opt.def_option("-x", "--option=VAL") {|x| @flag = x}
    end
  end
  class Def3 < TestOptionParser
    include TestOptionParserReqArg
    def setup
      super
      @opt.def_option("--option=VAL", "-x") {|x| @flag = x}
    end
  end
  class Def4 < TestOptionParser
    include TestOptionParserReqArg
    def setup
      super
      @opt.def_option("-xVAL", "--option=VAL") {|x| @flag = x}
    end
  end

  def test_short
    assert_raise(OptionParser::MissingArgument) {@opt.parse!(%w"-x")}
    assert_equal(%w"", no_error {@opt.parse!(%w"-x foo")})
    assert_equal("foo", @flag)
    assert_equal(%w"", no_error {@opt.parse!(%w"-xbar")})
    assert_equal("bar", @flag)
    assert_equal(%w"", no_error {@opt.parse!(%w"-x=")})
    assert_equal("=", @flag)
  end

  def test_abbrev
    assert_raise(OptionParser::MissingArgument) {@opt.parse!(%w"-o")}
    assert_equal(%w"", no_error {@opt.parse!(%w"-o foo")})
    assert_equal("foo", @flag)
    assert_equal(%w"