# frozen_string_literal: false
require 'test/unit'

class TestUTF16 < Test::Unit::TestCase
  def encdump(obj)
    case obj
    when String
      d = obj.dump
      if /\.force_encoding\("[A-Za-z0-9.:_+-]*"\)\z/ =~ d
        d
      else
        "#{d}.force_encoding(#{obj.encoding.name.dump})"
      end
    when Regexp
      "Regexp.new(#{encdump(obj.source)}, #{obj.options})"
    else
      raise Argument, "unexpected: #{obj.inspect}"
    end
  end

  def enccall(recv, meth, *args)
    desc = ''
    if String === recv
      desc << encdump(recv)
    else
      desc << recv.inspect
    end
    desc << '.' << meth.to_s
    if !args.empty?
      desc << '('
      args.each_with_index {|a, i|
        desc << ',' if 0 < i
        if String === a
          desc << encdump(a)
        else
          desc << a.inspect
        end
      }
      desc << ')'
    end
    result = nil
    assert_nothing_raised(desc) {
      result = recv.send(meth, *args)
    }
    result
  end

  def assert_str_equal(expected, actual, message=nil)
    full_message = build_message(message, <<EOT)
#{encdump expected} expected but not equal to
#{encdump actual}.
EOT
    assert_equal(expected, actual, full_message)
  end

  # tests start

  def test_utf16be_valid_encoding
    all_assertions do |a|
      [
        "\x00\x00",
        "\xd7\xff",
        "\xd8\x00\xdc\x00",
        "\xdb\xff\xdf\xff",
        "\xe0\x00",
        "\xff\xff",
      ].each {|s|
        s.force_encoding("utf-16be")
        a.for(s) {
          assert_predicate(s, :valid_encoding?, "#{encdump s}.valid_encoding?")
        }
      }
      [
        "\x00",
        "\xd7",
        "\xd8\x00",
        "\xd8\x00\xd8\x00",
        "\xdc\x00",
        "\xdc\x00\xd8\x00",
        "\xdc\x00\xdc\x00",
        "\xe0",
        "\xff",
      ].each {|s|
        s.force_encoding("utf-16be")
        a.for(s) {
          assert_not_predicate(s, :valid_encoding?, "#{encdump s}.valid_encoding?")
        }
      }
    end
  end

  def test_utf16le_valid_encoding
    all_assertions do |a|
      [
        "\x00\x00",
        "\xff\xd7",
        "\x00\xd8\x00\xdc",
        "\xff\xdb\xff\xdf",
        "\x00\xe0",
        "\xff\xff",
      ].each {|s|
        s.force_encoding("utf-16le")
        a.for(s) {
          assert_predicate(s, :valid_encoding?, "#{encdump s}.valid_encoding?")
        }
      }
      [
        # "\x00", # TruffleString: UTF-16 string byte length is not a multiple of 2
        # "\xd7",
        "\x00\xd8",
        "\x00\xd8\x00\xd8",
        "\x00\xdc",
        "\x00\xdc\x00\xd8",
        "\x00\xdc\x00\xdc",
        # "\xe0",
        # "\xff",
      ].each {|s|
        s.force_encoding("utf-16le")
        a.for(s) {
          assert_not_predicate(s, :valid_encoding?, "#{encdump s}.valid_encoding?")
        }
      }
    end
  end

  def test_strftime
    s = "aa".force_encoding("utf-16be")
    assert_raise(ArgumentError, "Time.now.strftime(#{encdump s})") { Time.now.strftime(s) }
  end

  def test_intern
    s = "aaaa".force_encoding("utf-16be")
    assert_equal(s.encoding, s.intern.to_s.encoding, "#{encdump s}.intern.to_s.encoding")
  end

  def test_sym_eq
    s = "aa".force_encoding("utf-16le")
    assert_not_equal(:aa, s.intern, "#{encdump s}.intern != :aa")
  end

  def test_compatible
    s1 = "aa".force_encoding("utf-16be")
    s2 = "z".force_encoding("us-ascii")
    assert_nil(Encoding.compatible?(s1, s2), "Encoding.compatible?(#{encdump s1}, #{encdump s2})")
  end

  def test_casecmp
    s1 = "aa".force_encoding("utf-16be")
    s2 = "AA"
    assert_not_equal(0, s1.casecmp(s2), "#{encdump s1}.casecmp(#{encdump s2})")
  end

  def test_end_with
    s1 = "ab".force_encoding("utf-16be")
    s2 = "b".force_encoding("utf-16be")
    assert_equal(false, s1.end_with?(s2), "#{encdump s1}.end_with?(#{encdump s2})")
  end

  def test_hex
    assert_raise(Encoding::CompatibilityError) {
      "ff".encode("utf-16le").hex
    }
    assert_raise(Encoding::CompatibilityError) {
      "ff".encode("utf-16be").hex
    }
  end

  def test_oct
    assert_raise(Encoding::CompatibilityError) {
      "77".encode("utf-16le").oct
    }
    assert_raise(Encoding::CompatibilityError) {
      "77".encode("utf-16be").oct
    }
  end

  def test_count
    s1 = "aa".force_encoding("utf-16be")
    s2 = "aa"
    assert_raise(Encoding::CompatibilityError, "#{encdump s1}.count(#{encdump s2})") {
      s1.count(s2)
    }
  end

  def test_plus
    s1 = "a".force_encoding("us-ascii")
    s2 = "aa".force_encoding("utf-16be")
    assert_raise(Encoding::CompatibilityError, "#