# frozen_string_literal: true
require_relative 'helper'

class TestRDocMarkupToHtmlSnippet < RDoc::Markup::FormatterTestCase

  add_visitor_tests

  def setup
    super

    @to = RDoc::Markup::ToHtmlSnippet.new @options, 100, 100
    @ellipsis = @to.to_html '...'
  end

  def accept_blank_line
    assert_empty @to.res.join
  end

  def accept_block_quote
    assert_equal "\n<blockquote><p>quote\n</blockquote>\n", @to.res.join

    assert_equal 5, @to.characters
  end

  def accept_document
    assert_equal "<p>hello\n", @to.res.join
    assert_equal 5, @to.characters
  end

  def accept_heading
    assert_equal "<p>Hello\n", @to.res.join
    assert_equal 5, @to.characters
  end

  def accept_heading_1
    assert_equal "<p>Hello\n", @to.res.join
    assert_equal 5, @to.characters
  end

  def accept_heading_2
    assert_equal "<p>Hello\n", @to.res.join
    assert_equal 5, @to.characters
  end

  def accept_heading_3
    assert_equal "<p>Hello\n", @to.res.join
    assert_equal 5, @to.characters
  end

  def accept_heading_4
    assert_equal "<p>Hello\n", @to.res.join
    assert_equal 5, @to.characters
  end

  def accept_heading_b
    assert_equal "<p><strong>Hello</strong>\n",
                 @to.res.join
    assert_equal 5, @to.characters
  end

  def accept_heading_suppressed_crossref
    assert_equal "<p>Hello\n", @to.res.join
    assert_equal 5, @to.characters
  end

  def accept_list_end_bullet
    assert_equal [], @to.list
    assert_equal [], @to.in_list_entry

    assert_equal "\n", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_end_label
    assert_equal [], @to.list
    assert_equal [], @to.in_list_entry

    assert_equal "\n", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_end_lalpha
    assert_equal [], @to.list
    assert_equal [], @to.in_list_entry

    assert_equal "\n", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_end_number
    assert_equal [], @to.list
    assert_equal [], @to.in_list_entry

    assert_equal "\n", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_end_note
    assert_equal [], @to.list
    assert_equal [], @to.in_list_entry

    assert_equal "\n", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_end_ualpha
    assert_equal [], @to.list
    assert_equal [], @to.in_list_entry

    assert_equal "\n", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_item_end_bullet
    assert_equal [''], @to.in_list_entry
    assert_equal 0, @to.characters
  end

  def accept_list_item_end_label
    assert_equal [''], @to.in_list_entry
    assert_equal 4, @to.characters
  end

  def accept_list_item_end_lalpha
    assert_equal [''], @to.in_list_entry
    assert_equal 0, @to.characters
  end

  def accept_list_item_end_note
    assert_equal [''], @to.in_list_entry
    assert_equal 4, @to.characters
  end

  def accept_list_item_end_number
    assert_equal [''], @to.in_list_entry
    assert_equal 0, @to.characters
  end

  def accept_list_item_end_ualpha
    assert_equal [''], @to.in_list_entry
    assert_equal 0, @to.characters
  end

  def accept_list_item_start_bullet
    assert_equal "<p>", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_item_start_label
    assert_equal "<p>cat &mdash; ", @to.res.join
    assert_equal 4, @to.characters
  end

  def accept_list_item_start_lalpha
    assert_equal "<p>", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_item_start_note
    assert_equal "<p>cat &mdash; ",
                 @to.res.join
    assert_equal 4, @to.characters
  end

  def accept_list_item_start_note_2
    expected = <<-EXPECTED
<p><code>teletype</code> &mdash; teletype description

    EXPECTED

    assert_equal expected, @to.res.join
    assert_equal 29, @to.characters
  end

  def accept_list_item_start_note_multi_description
    expected = <<-EXPECTED
<p>label &mdash; description one
<p>description two

    EXPECTED

    assert_equal expected, @to.res.join
    assert_equal 37, @to.characters
  end

  def accept_list_item_start_note_multi_label
    expected = <<-EXPECTED
<p>one, two &mdash; two headers

    EXPECTED

    assert_equal expected, @to.res.join
    assert_equal 18, @to.characters
  end

  def accept_list_item_start_number
    assert_equal "<p>", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_item_start_ualpha
    assert_equal "<p>", @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_start_bullet
    assert_equal [:BULLET], @to.list
    assert_equal [''], @to.in_list_entry

    assert_equal '', @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_start_label
    assert_equal [:LABEL], @to.list
    assert_equal [''], @to.in_list_entry

    assert_equal '', @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_start_lalpha
    assert_equal [:LALPHA], @to.list
    assert_equal [''], @to.in_list_entry

    assert_equal '', @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_start_note
    assert_equal [:NOTE], @to.list
    assert_equal [''], @to.in_list_entry

    assert_equal '', @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_start_number
    assert_equal [:NUMBER], @to.list
    assert_equal [''], @to.in_list_entry

    assert_equal '', @to.res.join
    assert_equal 0, @to.characters
  end

  def accept_list_start_ualpha
    assert_equal [:UALPHA], @to.list
    assert_eq