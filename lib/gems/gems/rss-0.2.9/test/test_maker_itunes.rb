# frozen_string_literal: false
require_relative "rss-testcase"

require "rss/maker"

module RSS
  class TestMakerITunes < TestCase
    def test_author
      assert_maker_itunes_author(%w(channel))
      assert_maker_itunes_author(%w(items last))
    end

    def test_block
      assert_maker_itunes_block(%w(channel))
      assert_maker_itunes_block(%w(items last))
    end

    def test_category
      assert_maker_itunes_category(%w(channel))
    end

    def test_image
      assert_maker_itunes_image(%w(channel))
      assert_maker_itunes_image(%w(items last))
    end

    def test_duration
      assert_maker_itunes_duration(%w(items last))
    end

    def test_explicit
      assert_maker_itunes_explicit(%w(channel))
      assert_maker_itunes_explicit(%w(items last))
    end

    def test_keywords
      assert_maker_itunes_keywords(%w(channel))
      assert_maker_itunes_keywords(%w(items last))
    end

    def test_new_feed_url
      assert_maker_itunes_new_feed_url(%w(channel))
    end

    def test_owner
      assert_maker_itunes_owner(%w(channel))
    end

    def test_subtitle
      assert_maker_itunes_subtitle(%w(channel))
      assert_maker_itunes_subtitle(%w(items last))
    end

    def test_