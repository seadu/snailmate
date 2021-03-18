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

    def test_summary
      assert_maker_itunes_summary(%w(channel))
      assert_maker_itunes_summary(%w(items last))
    end

    private

    def assert_maker_itunes_author(maker_readers, feed_readers=nil)
      _wrap_assertion do
        feed_readers ||= maker_readers
        author = "John Doe"
        rss20 = ::RSS::Maker.make("rss2.0") do |maker|
          setup_dummy_channel(maker)
          setup_dummy_item(maker)

          target = chain_reader(maker, maker_readers)
          target.itunes_author = author
        end
        target = chain_reader(rss20, feed_readers)
        assert_equal(author, target.itunes_author)
      end
    end

    def _assert_maker_itunes_block(value, boolean_value, maker_readers,
                                   feed_readers)
      rss20 = ::RSS::Maker.make("rss2.0") do |maker|
        setup_dummy_channel(maker)
        setup_dummy_item(maker)

        target = chain_reader(maker, maker_readers)
        target.itunes_block = value
        assert_equal(value, target.itunes_block)
        assert_equal(boolean_value, target.itunes_block?)
      end
      target = chain_reader(rss20, feed_readers)
      if [true, false].include?(value)
        value = value ? "yes" : "no"
      end
      assert_equal(value, target.itunes_block)
      assert_equal(boolean_value, target.itunes_block?)
    end

    def assert_maker_itunes_block(maker_readers, feed_readers=nil)
      _wrap_assertion do
        feed_readers ||= maker_readers
        _assert_maker_itunes_block("yes", true, maker_readers, feed_readers)
        _assert_maker_itunes_block("Yes", true, maker_readers, feed_readers)
        _assert_maker_itunes_block("no", false, maker_readers, feed_readers)
        _assert_maker_itunes_block("", false, maker_readers, feed_readers)
        _assert_maker_itunes_block(true, true, maker_readers, feed_readers)
        _assert_maker_itunes_block(false, false, maker_readers, feed_readers)
        _assert_maker_itunes_block(nil, false, maker_readers, feed_readers)
      end
    end

    def _assert_maker_itunes_category(categories, maker_readers, feed_readers)
      rss20 = ::RSS::Maker.make("rss2.0") do |maker|
        setup_dummy_channel(maker)
        setup_dummy_item(maker)

        target = chain_reader(maker, maker_readers)
        categories.each do |category|
          sub_target = target.itunes_categories
          if category.is_a?(Array)
            category.each do |sub_category|
              sub_target = sub_target.new_category
              sub_target.text = sub_category
            end
          else
            sub_target.new_category.text = category
          end
        end
      end

      target = chain_reader(rss20, feed_readers)
      actual_categories = target.itunes_categories.collect do |category|
        cat = category.text
        if category.itunes_categories.empty?
          cat
        else
          [cat, *category.itunes_categories.collect {|c| c.text}]
        end
      end
      assert_equal(categories, actual_categories)
    end

    def assert_maker_itunes_category(maker_readers, feed_readers=nil)
      _wrap_assertion do
        feed_readers ||= maker_readers
        _assert_maker_itunes_category(["Audio Blogs"],
                                      maker_readers, feed_readers)
        _assert_maker_itunes_category([["Arts & Entertainment", "Games"]],
                                      maker_readers, feed_readers)
        _assert_maker_itunes_category([["Arts & Entertainment", "Games"],
                                       ["Technology", "Computers"],
                                       "Audio Blogs"],
                                      maker_readers, feed_readers)
      end
    end

    def assert_maker_itunes_image(maker_readers, feed_readers=nil)
      _wrap_assertion do
        feed_readers ||= maker_readers
        url = "http://example.com/podcasts/everything/AllAboutEverything.jpg"

        rss20 = ::RSS::Maker.make("rss2.0") do |maker|
          setup_dummy_channel(maker)
          setup_dummy_item(maker)

          target = chain_reader(maker, maker_readers)
          target.itunes_image = url
        end

        target = chain_reader(rss20, feed_readers)
        assert_not_nil(target.itunes_image)
        assert_equal(url, target.itunes_image.href)
      end
    end

    def _assert_maker_itunes_duration(hour, minute, second, value,
                                      maker_readers, feed_readers)
      _assert_maker_itunes_duration_by_value(hour, minute, second, value,
                                             maker_readers, feed_readers)
      _assert_maker_itunes_duration_by_hour_minute_second(hour, minute, second,
                                                          value,
                                                          maker_readers,
                                                          feed_readers)
    end

    def _assert_maker_itunes_duration_by(hour, minute, second, value,
                                         maker_readers, feed_readers)
      expected_value = nil
      rss20 = ::RSS::Maker.make("rss2.0") do |maker|
        setup_dummy_channel(maker)
        setup_dummy_item(maker)

        target = chain_reader(maker, maker_readers)
        expected_value = yield(target)
        assert_equal(expected_value, target.itunes_duration)
        target.itunes_duration do |duration|
          assert_equal([hour, minute, second, expected_value],
                       [duration.hour, duration.minute,
                        duration.second, duration.content])
        end
      end
      target = chain_reader(rss20, feed_readers)
      duration = target.itunes_duration
      assert_not_nil(duration)
      assert_equal([hour, minute, second, expected_value],
                   [duration.hour, duration.minute,
                    duration.second, duration.content])
    end

    def _assert_maker_itunes_duration_by_value(hour, minute, second, value,
                                               maker_readers, feed_readers)
      _assert_maker_itunes_duration_by(hour, minute, second, value,
                                       maker_readers, feed_readers) do |target|
        target.itunes_duration = value
        value
      end
    end

    def _assert_maker_itunes_duration_by_hour_minute_second(hour, minute, second,
                                                            value,
                                                            maker_readers,
                                                            feed_readers)
      _assert_maker_itunes_duration_by(hour, minute, second, value,
                                       maker_readers, feed_r