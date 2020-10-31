# frozen_string_literal: false
require_relative "rss-testcase"

require "rss/2.0"

module RSS
  class TestParser20 < TestCase
    def test_rss20
      assert_parse(make_rss20(<<-EOR), :missing_tag, "channel", "rss")
EOR

 