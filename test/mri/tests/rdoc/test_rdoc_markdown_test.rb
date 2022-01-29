
# frozen_string_literal: true
require_relative 'helper'
require 'pp'

require 'rdoc'
require 'rdoc/markdown'

class TestRDocMarkdownTest < RDoc::TestCase

  MARKDOWN_TEST_PATH = File.expand_path '../MarkdownTest_1.0.3/', __FILE__

  def setup
    super

    @parser = RDoc::Markdown.new
  end

  def test_amps_and_angle_encoding
    input = File.read "#{MARKDOWN_TEST_PATH}/Amps and angle encoding.text"

    doc = @parser.parse input

    expected =
      doc(
        para("AT&T has an ampersand in their name."),
        para("AT&T is another way to write it."),
        para("This & that."),
        para("4 < 5."),
        para("6 > 5."),
        para("Here's a {link}[http://example.com/?foo=1&bar=2] with " +
             "an ampersand in the URL."),
        para("Here's a link with an amersand in the link text: " +
             "{AT&T}[http://att.com/]."),
        para("Here's an inline {link}[/script?foo=1&bar=2]."),
        para("Here's an inline {link}[/script?foo=1&bar=2]."))

    assert_equal expected, doc
  end

  def test_auto_links
    input = File.read "#{MARKDOWN_TEST_PATH}/Auto links.text"

    doc = @parser.parse input

    # TODO verify rdoc auto-links too
    expected =
      doc(
        para("Link: http://example.com/."),
        para("With an ampersand: http://example.com/?foo=1&bar=2"),
        list(:BULLET,
          item(nil, para("In a list?")),
          item(nil, para("http://example.com/")),
          item(nil, para("It should."))),
        block(
          para("Blockquoted: http://example.com/")),
        para("Auto-links should not occur here: " +
             "<code><http://example.com/></code>"),
        verb("or here: <http://example.com/>\n"))

    assert_equal expected, doc
  end

  def test_backslash_escapes
    input = File.read "#{MARKDOWN_TEST_PATH}/Backslash escapes.text"

    doc = @parser.parse input

    expected =
      doc(
        para("These should all get escaped:"),

        para("Backslash: \\"),
        para("Backtick: `"),
        para("Asterisk: *"),
        para("Underscore: _"),
        para("Left brace: {"),
        para("Right brace: }"),
        para("Left bracket: ["),
        para("Right bracket: ]"),
        para("Left paren: ("),
        para("Right paren: )"),
        para("Greater-than: >"),
        para("Hash: #"),
        para("Period: ."),
        para("Bang: !"),
        para("Plus: +"),
        para("Minus: -"),

        para("These should not, because they occur within a code block:"),

        verb("Backslash: \\\\\n",
             "\n",
             "Backtick: \\`\n",
             "\n",
             "Asterisk: \\*\n",
             "\n",
             "Underscore: \\_\n",
             "\n",
             "Left brace: \\{\n",
             "\n",
             "Right brace: \\}\n",
             "\n",
             "Left bracket: \\[\n",
             "\n",
             "Right bracket: \\]\n",
             "\n",
             "Left paren: \\(\n",
             "\n",
             "Right paren: \\)\n",
             "\n",
             "Greater-than: \\>\n",
             "\n",
             "Hash: \\#\n",
             "\n",
             "Period: \\.\n",
             "\n",
             "Bang: \\!\n",
             "\n",
             "Plus: \\+\n",
             "\n",
             "Minus: \\-\n"),

        para("Nor should these, which occur in code spans:"),

        para("Backslash: <code>\\\\</code>"),
        para("Backtick: <code>\\`</code>"),
        para("Asterisk: <code>\\*</code>"),
        para("Underscore: <code>\\_</code>"),
        para("Left brace: <code>\\{</code>"),
        para("Right brace: <code>\\}</code>"),
        para("Left bracket: <code>\\[</code>"),
        para("Right bracket: <code>\\]</code>"),
        para("Left paren: <code>\\(</code>"),
        para("Right paren: <code>\\)</code>"),
        para("Greater-than: <code>\\></code>"),
        para("Hash: <code>\\#</code>"),
        para("Period: <code>\\.</code>"),
        para("Bang: <code>\\!</code>"),
        para("Plus: <code>\\+</code>"),
        para("Minus: <code>\\-</code>"),

        para("These should get escaped, even though they're matching pairs for\n" +
             "other Markdown constructs:"),

        para("\*asterisks\*"),
        para("\_underscores\_"),
        para("`backticks`"),

        para("This is a code span with a literal backslash-backtick " +
             "sequence: <code>\\`</code>"),

        para("This is a tag with unescaped backticks " +
             "<span attr='`ticks`'>bar</span>."),

        para("This is a tag with backslashes " +
             "<span attr='\\\\backslashes\\\\'>bar</span>."))

    assert_equal expected, doc
  end

  def test_blockquotes_with_code_blocks
    input = File.read "#{MARKDOWN_TEST_PATH}/Blockquotes with code blocks.text"

    doc = @parser.parse input

    expected =
      doc(
        block(
          para("Example:"),
          verb("sub status {\n",
               "    print \"working\";\n",
               "}\n"),
          para("Or:"),
          verb("sub status {\n",
               "    return \"working\";\n",
               "}\n")))

    assert_equal expected, doc
  end

  def test_code_blocks
    input = File.read "#{MARKDOWN_TEST_PATH}/Code Blocks.text"

    doc = @parser.parse input

    expected =
      doc(
        verb("code block on the first line\n"),
        para("Regular text."),

        verb("code block indented by spaces\n"),
        para("Regular text."),

        verb("the lines in this block  \n",
             "all contain trailing spaces  \n"),
        para("Regular Text."),

        verb("code block on the last line\n"))

    assert_equal expected, doc
  end

  def test_code_spans
    input = File.read "#{MARKDOWN_TEST_PATH}/Code Spans.text"

    doc = @parser.parse input

    expected = doc(
      para("<code><test a=\"</code> content of attribute <code>\"></code>"),
      para("Fix for backticks within HTML tag: " +
           "<span attr='`ticks`'>like this</span>"),
      para("Here's how you put <code>`backticks`</code> in a code span."))

    assert_equal expected, doc
  end

  def test_hard_wrapped_paragraphs_with_list_like_lines
    input = File.read "#{MARKDOWN_TEST_PATH}/Hard-wrapped paragraphs with list-like lines.text"

    doc = @parser.parse input

    expected =
      doc(
        para("In Markdown 1.0.0 and earlier. Version\n" +
             "8. This line turns into a list item.\n"   +
             "Because a hard-wrapped line in the\n"     +
             "middle of a paragraph looked like a\n"    +
             "list item."),
        para("Here's one with a bullet.\n" +
             "* criminey."))

    assert_equal expected, doc
  end

  def test_horizontal_rules
    input = File.read "#{MARKDOWN_TEST_PATH}/Horizontal rules.text"

    doc = @parser.parse input

    expected =
      doc(
        para("Dashes:"),

        rule(1),
        rule(1),
        rule(1),
        rule(1),
        verb("---\n"),

        rule(1),
        rule(1),
        rule(1),
        rule(1),
        verb("- - -\n"),

        para("Asterisks:"),

        rule(1),
        rule(1),
        rule(1),
        rule(1),
        verb("***\n"),

        rule(1),
        rule(1),
        rule(1),
        rule(1),
        verb("* * *\n"),

        para("Underscores:"),

        rule(1),
        rule(1),
        rule(1),
        rule(1),
        verb("___\n"),

        rule(1),
        rule(1),
        rule(1),
        rule(1),
        verb("_ _ _\n"))

    assert_equal expected, doc
  end

  def test_inline_html_advanced
    input = File.read "#{MARKDOWN_TEST_PATH}/Inline HTML (Advanced).text"

    @parser.html = true

    doc = @parser.parse input

    expected =
      doc(
        para("Simple block on one line:"),
        raw("<div>foo</div>"),
        para("And nested without indentation:"),
        raw(<<-RAW.chomp))
<div>
<div>
<div>
foo
</div>
<div style=">"/>
</div>
<div>bar</div>
</div>
        RAW

    assert_equal expected, doc
  end

  def test_inline_html_simple
    input = File.read "#{MARKDOWN_TEST_PATH}/Inline HTML (Simple).text"

    @parser.html = true

    doc = @parser.parse input

    expected =
      doc(
       para("Here's a simple block:"),
       raw("<div>\n\tfoo\n</div>"),

       para("This should be a code block, though:"),
       verb("<div>\n",
            "\tfoo\n",
            "</div>\n"),

       para("As should this:"),
       verb("<div>foo</div>\n"),

       para("Now, nested:"),
       raw("<div>\n\t<div>\n\t\t<div>\n\t\t\tfoo\n" +
           "\t\t</div>\n\t</div>\n</div>"),

       para("This should just be an HTML comment:"),
       raw("<!-- Comment -->"),

       para("Multiline:"),
       raw("<!--\nBlah\nBlah\n-->"),

       para("Code block:"),
       verb("<!-- Comment -->\n"),

       para("Just plain comment, with trailing spaces on the line:"),
       raw("<!-- foo -->"),

       para("Code:"),
       verb("<hr />\n"),

       para("Hr's:"),
       raw("<hr>"),
       raw("<hr/>"),
       raw("<hr />"),

       raw("<hr>"),
       raw("<hr/>"),
       raw("<hr />"),

       raw("<hr class=\"foo\" id=\"bar\" />"),
       raw("<hr class=\"foo\" id=\"bar\"/>"),
       raw("<hr class=\"foo\" id=\"bar\" >"))

    assert_equal expected, doc
  end

  def test_inline_html_comments
    input = File.read "#{MARKDOWN_TEST_PATH}/Inline HTML comments.text"

    doc = @parser.parse input

    expected =
      doc(
        para("Paragraph one."),

        raw("<!-- This is a simple comment -->"),

        raw("<!--\n\tThis is another comment.\n-->"),

        para("Paragraph two."),

        raw("<!-- one comment block -- -- with two comments -->"),

        para("The end."))

    assert_equal expected, doc
  end

  def test_links_inline_style
    input = File.read "#{MARKDOWN_TEST_PATH}/Links, inline style.text"

    doc = @parser.parse input

    expected =
      doc(
        para("Just a {URL}[/url/]."),
        para("{URL and title}[/url/]."),
        para("{URL and title}[/url/]."),
        para("{URL and title}[/url/]."),
        para("{URL and title}[/url/]."),
        para("{Empty}[]."))

    assert_equal expected, doc
  end

  def test_links_reference_style
    input = File.read "#{MARKDOWN_TEST_PATH}/Links, reference style.text"

    doc = @parser.parse input

    expected =
      doc(
        para("Foo {bar}[/url/]."),
        para("Foo {bar}[/url/]."),
        para("Foo {bar}[/url/]."),

        para("With {embedded [brackets]}[/url/]."),

        para("Indented {once}[/url]."),
        para("Indented {twice}[/url]."),
        para("Indented {thrice}[/url]."),
        para("Indented [four][] times."),

        verb("[four]: /url\n"),

        rule(1),

        para("{this}[foo] should work"),
        para("So should {this}[foo]."),
        para("And {this}[foo]."),
        para("And {this}[foo]."),
        para("And {this}[foo]."),

        para("But not [that] []."),
        para("Nor [that][]."),
        para("Nor [that]."),

        para("[Something in brackets like {this}[foo] should work]"),
        para("[Same with {this}[foo].]"),

        para("In this case, {this}[/somethingelse/] points to something else."),
        para("Backslashing should suppress [this] and [this]."),

        rule(1),

        para("Here's one where the {link breaks}[/url/] across lines."),
        para("Here's another where the {link breaks}[/url/] across lines, " +
             "but with a line-ending space."))

    assert_equal expected, doc
  end

  def test_links_shortcut_references
    input = File.read "#{MARKDOWN_TEST_PATH}/Links, shortcut references.text"

    doc = @parser.parse input

    expected =
      doc(
        para("This is the {simple case}[/simple]."),
        para("This one has a {line break}[/foo]."),
        para("This one has a {line break}[/foo] with a line-ending space."),
        para("{this}[/that] and the {other}[/other]"))

    assert_equal expected, doc
  end

  def test_literal_quotes_in_titles
    input = File.read "#{MARKDOWN_TEST_PATH}/Literal quotes in titles.text"

    doc = @parser.parse input

    # TODO support title attribute
    expected =
      doc(
        para("Foo {bar}[/url/]."),
        para("Foo {bar}[/url/]."))

    assert_equal expected, doc
  end

  def test_markdown_documentation_basics
    input = File.read "#{MARKDOWN_TEST_PATH}/Markdown Documentation - Basics.text"

    doc = @parser.parse input

    expected =
      doc(
        head(1, "Markdown: Basics"),

        raw(<<-RAW.chomp),
<ul id="ProjectSubmenu">
    <li><a href="/projects/markdown/" title="Markdown Project Page">Main</a></li>
    <li><a class="selected" title="Markdown Basics">Basics</a></li>
    <li><a href="/projects/markdown/syntax" title="Markdown Syntax Documentation">Syntax</a></li>
    <li><a href="/projects/markdown/license" title="Pricing and License Information">License</a></li>
    <li><a href="/projects/markdown/dingus" title="Online Markdown Web Form">Dingus</a></li>
</ul>
        RAW

        head(2, "Getting the Gist of Markdown's Formatting Syntax"),

        para("This page offers a brief overview of what it's like to use Markdown.\n" +
             "The {syntax page}[/projects/markdown/syntax] provides complete, detailed documentation for\n" +
             "every feature, but Markdown should be very easy to pick up simply by\n" +
             "looking at a few examples of it in action. The examples on this page\n" +
             "are written in a before/after style, showing example syntax and the\n" +
             "HTML output produced by Markdown."),

        para("It's also helpful to simply try Markdown out; the {Dingus}[/projects/markdown/dingus] is a\n" +
             "web application that allows you type your own Markdown-formatted text\n" +
             "and translate it to XHTML."),

        para("<b>Note:</b> This document is itself written using Markdown; you\n" +
             "can {see the source for it by adding '.text' to the URL}[/projects/markdown/basics.text]."),

        head(2, "Paragraphs, Headers, Blockquotes"),

        para("A paragraph is simply one or more consecutive lines of text, separated\n" +
             "by one or more blank lines. (A blank line is any line that looks like a\n" +
             "blank line -- a line containing nothing spaces or tabs is considered\n" +
             "blank.) Normal paragraphs should not be intended with spaces or tabs."),

        para("Markdown offers two styles of headers: _Setext_ and _atx_.\n" +
             "Setext-style headers for <code><h1></code> and <code><h2></code> are created by\n" +
             "\"underlining\" with equal signs (<code>=</code>) and hyphens (<code>-</code>), respectively.\n" +
             "To create an atx-style header, you put 1-6 hash marks (<code>#</code>) at the\n" +