# frozen_string_literal: false
require 'test/unit'

class TestString < Test::Unit::TestCase
  WIDE_ENCODINGS = [
     Encoding::UTF_16BE, Encoding::UTF_16LE,
     Encoding::UTF_32BE, Encoding::UTF_32LE,
  ]

  def initialize(*args)
    @cls = String
    @aref_re_nth = true
    @aref_re_silent = false
    @aref_slicebang_silent = true
    super
  end

  def S(*args, **kw)
    @cls.new(*args, **kw)
  end

  def test_s_new
    assert_equal("", S())
    assert_equal(Encoding::ASCII_8BIT, S().encoding)

    assert_equal("", S(""))
    assert_equal(__ENCODING__, S("").encoding)

    src = "RUBY"
    assert_equal(src, S(src))
    assert_equal(__ENCODING__, S(src).encoding)

    src.force_encoding("euc-jp")
    assert_equal(src, S(src))
    assert_equal(Encoding::EUC_JP, S(src).encoding)


    assert_equal("", S(encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S(encoding: "euc-jp").encoding)

    assert_equal("", S("", encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S("", encoding: "euc-jp").encoding)

    src = "RUBY"
    assert_equal(src, S(src, encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S(src, encoding: "euc-jp").encoding)

    src.force_encoding("euc-jp")
    assert_equal(src, S(src, encoding: "utf-8"))
    assert_equal(Encoding::UTF_8, S(src, encoding: "utf-8").encoding)

    assert_equal("", S(capacity: 1000))
    assert_equal(Encoding::ASCII_8BIT, S(capacity: 1000).encoding)

    assert_equal("", S(capacity: 1000, encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S(capacity: 1000, encoding: "euc-jp").encoding)

    assert_equal("", S("", capacity: 1000))
    assert_equal(__ENCODING__, S("", capacity: 1000).encoding)

    assert_equal("", S("", capacity: 1000, encoding: "euc-jp"))
    assert_equal(Encoding::EUC_JP, S("", capacity: 1000, encoding: "euc-jp").encoding)
  end

  def test_initialize
    str = S("").freeze
    assert_equal("", str.__send__(:initialize))
    assert_raise(FrozenError){ str.__send__(:initialize, 'abc') }
    assert_raise(FrozenError){ str.__send__(:initialize, capacity: 1000) }
    assert_raise(FrozenError){ str.__send__(:initialize, 'abc', capacity: 1000) }
    assert_raise(FrozenError){ str.__send__(:initialize, encoding: 'euc-jp') }
    assert_raise(FrozenError){ str.__send__(:initialize, 'abc', encoding: 'euc-jp') }
    assert_raise(FrozenError){ str.__send__(:initialize, 'abc', capacity: 1000, encoding: 'euc-jp') }

    str = S("")
    assert_equal("mystring", str.__send__(:initialize, "mystring"))
    str = S("mystring")
    assert_equal("mystring", str.__send__(:initialize, str))
    str = S("")
    assert_equal("mystring", str.__send__(:initialize, "mystring", capacity: 1000))
    str = S("mystring")
    assert_equal("mystring", str.__send__(:initialize, str, capacity: 1000))
  end

  def test_initialize_shared
    String.new(str = "mystring" * 10).__send__(:initialize, capacity: str.bytesize)
    assert_equal("mystring", str[0, 8])
  end

  def test_initialize_nonstring
    assert_raise(TypeError) {
      S(1)
    }
    assert_raise(TypeError) {
      S(1, capacity: 1000)
    }
  end

  def test_initialize_memory_leak
    assert_no_memory_leak([], <<-PREP, <<-CODE, rss: true)
code = proc {('x'*100000).__send__(:initialize, '')}
1_000.times(&code)
PREP
100_000.times(&code)
CODE
  end

  # Bug #18154
  def test_initialize_nofree_memory_leak
    assert_no_memory_leak([], <<-PREP, <<-CODE, rss: true)
code = proc {0.to_s.__send__(:initialize, capacity: 10000)}
1_000.times(&code)
PREP
100_000.times(&code)
CODE
  end

  def test_AREF # '[]'
    assert_equal("A",  S("AooBar")[0])
    assert_equal("B",  S("FooBaB")[-1])
    assert_equal(nil, S("FooBar")[6])
    assert_equal(nil, S("FooBar")[-7])

    assert_equal(S("Foo"), S("FooBar")[0,3])
    assert_equal(S("Bar"), S("FooBar")[-3,3])
    assert_equal(S(""),    S("FooBar")[6,2])
    assert_equal(nil,      S("FooBar")[-7,10])

    assert_equal(S("Foo"), S("FooBar")[0..2])
    assert_equal(S("Foo"), S("FooBar")[0...3])
    assert_equal(S("Bar"), S("FooBar")[-3..-1])
    assert_equal(S(""),    S("FooBar")[6..2])
    assert_equal(nil,      S("FooBar")[-10..-7])

    assert_equal(S("Foo"), S("FooBar")[/^F../])
    assert_equal(S("Bar"), S("FooBar")[/..r$/])
    assert_equal(nil,      S("FooBar")[/xyzzy/])
    assert_equal(nil,      S("FooBar")[/plugh/])

    assert_equal(S("Foo"), S("FooBar")[S("Foo")])
    assert_equal(S("Bar"), S("FooBar")[S("Bar")])
    assert_equal(nil,      S("FooBar")[S("xyzzy")])
    assert_equal(nil,      S("FooBar")[S("plugh")])

    if @aref_re_nth
      assert_equal(S("Foo"), S("FooBar")[/([A-Z]..)([A-Z]..)/, 1])
      assert_equal(S("Bar"), S("FooBar")[/([A-Z]..)([A-Z]..)/, 2])
      assert_equal(nil,      S("FooBar")[/([A-Z]..)([A-Z]..)/, 3])
      assert_equal(S("Bar"), S("FooBar")[/([A-Z]..)([A-Z]..)/, -1])
      assert_equal(S("Foo"), S("FooBar")[/([A-Z]..)([A-Z]..)/, -2])
      assert_equal(nil,      S("FooBar")[/([A-Z]..)([A-Z]..)/, -3])
    end

    o = Object.new
    def o.to_int; 2; end
    assert_equal("o", "foo"[o])

    assert_raise(ArgumentError) { "foo"[] }
  end

  def test_ASET # '[]='
    s = S("FooBar")
    s[0] = S('A')
    assert_equal(S("AooBar"), s)

    s[-1]= S('B')
    assert_equal(S("AooBaB"), s)
    assert_raise(IndexError) { s[-7] = S("xyz") }
    assert_equal(S("AooBaB"), s)
    s[0] = S("ABC")
    assert_equal(S("ABCooBaB"), s)

    s = S("FooBar")
    s[0,3] = S("A")
    assert_equal(S("ABar"),s)
    s[0] = S("Foo")
    assert_equal(S("FooBar"), s)
    s[-3,3] = S("Foo")
    assert_equal(S("FooFoo"), s)
    assert_raise(IndexError) { s[7,3] =  S("Bar") }
    assert_raise(IndexError) { s[-7,3] = S("Bar") }

    s = S("FooBar")
    s[0..2] = S("A")
    assert_equal(S("ABar"), s)
    s[1..3] = S("Foo")
    assert_equal(S("AFoo"), s)
    s[-4..-4] = S("Foo")
    assert_equal(S("FooFoo"), s)
    assert_raise(RangeError) { s[7..10]   = S("Bar") }
    assert_raise(RangeError) { s[-7..-10] = S("Bar") }

    s = S("FooBar")
    s[/^F../]= S("Bar")
    assert_equal(S("BarBar"), s)
    s[/..r$/] = S("Foo")
    assert_equal(S("BarFoo"), s)
    if @aref_re_silent
      s[/xyzzy/] = S("None")
      assert_equal(S("BarFoo"), s)
    else
      assert_raise(IndexError) { s[/xyzzy/] = S("None") }
    end
    if @aref_re_nth
      s[/([A-Z]..)([A-Z]..)/, 1] = S("Foo")
      assert_equal(S("FooFoo"), s)
      s[/([A-Z]..)([A-Z]..)/, 2] = S("Bar")
      assert_equal(S("FooBar"), s)
      assert_raise(IndexError) { s[/([A-Z]..)([A-Z]..)/, 3] = "None" }
      s[/([A-Z]..)([A-Z]..)/, -1] = S("Foo")
      assert_equal(S("FooFoo"), s)
      s[/([A-Z]..)([A-Z]..)/, -2] = S("Bar")
      assert_equal(S("BarFoo"), s)
      assert_raise(IndexError) { s[/([A-Z]..)([A-Z]..)/, -3] = "None" }
    end

    s = S("FooBar")
    s[S("Foo")] = S("Bar")
    assert_equal(S("BarBar"), s)

    s = S("a string")
    s[0..s.size] = S("another string")
    assert_equal(S("another string"), s)

    o = Object.new
    def o.to_int; 2; end
    s = "foo"
    s[o] = "bar"
    assert_equal("fobar", s)

    assert_raise(ArgumentError) { "foo"[1, 2, 3] = "" }

    assert_raise(IndexError) {"foo"[RbConfig::LIMITS["LONG_MIN"]] = "l"}
  end

  def test_CMP # '<=>'
    assert_equal(1, S("abcdef") <=> S("abcde"))
    assert_equal(0, S("abcdef") <=> S("abcdef"))
    assert_equal(-1, S("abcde") <=> S("abcdef"))

    assert_equal(-1, S("ABCDEF") <=> S("abcdef"))

    assert_nil("foo" <=> Object.new)

    o = Object.new
    def o.to_str; "bar"; end
    assert_equal(1, "foo" <=> o)

    class << o;remove_method :to_str;end
    def o.<=>(x); nil; end
    assert_nil("foo" <=> o)

    class << o;remove_method :<=>;end
    def o.<=>(x); 1; end
    assert_equal(-1, "foo" <=> o)

    class << o;remove_method :<=>;end
    def o.<=>(x); 2**100; end
    assert_equal(-1, "foo" <=> o)
  end

  def test_EQUAL # '=='
    assert_not_equal(:foo, S("foo"))
    assert_equal(S("abcdef"), S("abcdef"))

    assert_not_equal(S("CAT"), S('cat'))
    assert_not_equal(S("CaT"), S('cAt'))
    assert_not_equal(S("cat\0""dog"), S("cat\0"))

    o = Object.new
    def o.to_str; end
    def o.==(x); false; end
    assert_equal(false, "foo" == o)
    class << o;remove_method :==;end
    def o.==(x); true; end
    assert_equal(true, "foo" == o)
  end

  def test_LSHIFT # '<<'
    assert_equal(S("world!"), S("world") << 33)
    assert_equal(S("world!"), S("world") << S("!"))

    s = "a"
    10.times {|i|
      s << s
      assert_equal("a" * (2 << i), s)
    }

    s = ["foo"].pack("p")
    l = s.size
    s << "bar"
    assert_equal(l + 3, s.size)

    bug = '[ruby-core:27583]'
    assert_raise(RangeError, bug) {S("a".force_encoding(Encoding::UTF_8)) << -3}
    assert_raise(RangeError, bug) {S("a".force_encoding(Encoding::UTF_8)) << -2}
    assert_raise(RangeError, bug) {S("a".force_encoding(Encoding::UTF_8)) << -1}
    assert_raise(RangeError, bug) {S("a".force_encoding(Encoding::UTF_8)) << 0x81308130}
    assert_nothing_raised {S("a".force_encoding(Encoding::GB18030)) << 0x81308130}
  end

  def test_MATCH # '=~'
    assert_equal(10,  S("FeeFieFoo-Fum") =~ /Fum$/)
    assert_equal(nil, S("FeeFieFoo-Fum") =~ /FUM$/)

    o = Object.new
    def o.=~(x); x + "bar"; end
    assert_equal("foobar", S("foo") =~ o)

    assert_raise(TypeError) { S("foo") =~ "foo" }
  end

  def test_MOD # '%'
    assert_equal(S("00123"), S("%05d") % 123)
    assert_equal(S("123  |00000001"), S("%-5s|%08x") % [123, 1])
    x = S("%3s %-4s%%foo %.0s%5d %#x%c%3.1f %b %x %X %#b %#x %#X") %
    [S("hi"),
      123,
      S("never seen"),
      456,
      0,
      ?A,
      3.0999,
      11,
      171,
      171,
      11,
      171,
      171]

    assert_equal(S(' hi 123 %foo   456 0A3.1 1011 ab AB 0b1011 0xab 0XAB'), x)
  end

  def test_MUL # '*'
    assert_equal(S("XXX"),  S("X") * 3)
    assert_equal(S("HOHO"), S("HO") * 2)
  end

  def test_PLUS # '+'
    assert_equal(S("Yodel"), S("Yo") + S("del"))
  end

  def casetest(a, b, rev=false)
    msg = proc {"#{a} should#{' not' if rev} match #{b}"}
    case a
    when b
      assert(!rev, msg)
    else
      assert(rev, msg)
    end
  end

  def test_VERY_EQUAL # '==='
    # assert_equal(true, S("foo") === :foo)
    casetest(S("abcdef"), S("abcdef"))

    casetest(S("CAT"), S('cat'), true) # Reverse the test - we don't want to
    casetest(S("CaT"), S('cAt'), true) # find these in the case.
  end

  def test_capitalize
    assert_equal(S("Hello"),  S("hello").capitalize)
    assert_equal(S("Hello"),  S("hELLO").capitalize)
    assert_equal(S("123abc"), S("123ABC").capitalize)
  end

  def test_capitalize!
    a = S("hello"); a.capitalize!
    assert_equal(S("Hello"), a)

    a = S("hELLO"); a.capitalize!
    assert_equal(S("Hello"), a)

    a = S("123ABC"); a.capitalize!
    assert_equal(S("123abc"), a)

    assert_equal(nil,         S("123abc").capitalize!)
    assert_equal(S("123abc"), S("123ABC").capitalize!)
    assert_equal(S("Abc"),    S("ABC").capitalize!)
    assert_equal(S("Abc"),    S("abc").capitalize!)
    assert_equal(nil,         S("Abc").capitalize!)

    a = S("hello")
    b = a.dup
    assert_equal(S("Hello"), a.capitalize!)
    assert_equal(S("hello"), b)

  end

  Bug2463 = '[ruby-dev:39856]'
  def test_center
    assert_equal(S("hello"),       S("hello").center(4))
    assert_equal(S("   hello   "), S("hello").center(11))
    assert_equal(S("ababaababa"), S("").center(10, "ab"), Bug2463)
    assert_equal(S("ababaababab"), S("").center(11, "ab"), Bug2463)
  end

  def test_chomp
    verbose, $VERBOSE = $VERBOSE, nil

    assert_equal(S("hello"), S("hello").chomp("\n"))
    assert_equal(S("hello"), S("hello\n").chomp("\n"))
    save = $/

    $/ = "\n"

    assert_equal(S("hello"), S("hello").chomp)
    assert_equal(S("hello"), S("hello\n").chomp)

    $/ = "!"
    assert_equal(S("hello"), S("hello").chomp)
    assert_equal(S("hello"), S("hello!").chomp)
    $/ = save

    assert_equal(S("a").hash, S("a\u0101").chomp(S("\u0101")).hash, '[ruby-core:22414]')

    s = S("hello")
    assert_equal("hel", s.chomp('lo'))
    assert_equal("hello", s)

    s = S("hello")
    assert_equal("hello", s.chomp('he'))
    assert_equal("hello", s)

    s = S("\u{3053 3093 306b 3061 306f}")
    assert_equal("\u{3053 3093 306b}", s.chomp("\u{3061 306f}"))
    assert_equal("\u{3053 3093 306b 3061 306f}", s)

    s = S("\u{3053 3093 306b 3061 306f}")
    assert_equal("\u{3053 3093 306b 3061 306f}", s.chomp('lo'))
    assert_equal("\u{3053 3093 306b 3061 306f}", s)

    s = S("hello")
    assert_equal("hello", s.chomp("\u{3061 306f}"))
    assert_equal("hello", s)

    # skip if argument is a broken string
    s = S("\xe3\x81\x82")
    assert_equal("\xe3\x81\x82", s.chomp("\x82"))
    assert_equal("\xe3\x81\x82", s)

    s = S("\x95\x5c").force_encoding("Shift_JIS")
    assert_equal("\x95\x5c".force_encoding("Shift_JIS"), s.chomp("\x5c"))
    assert_equal("\x95\x5c".force_encoding("Shift_JIS"), s)

    # clear coderange
    s = S("hello\u{3053 3093}")
    assert_not_predicate(s, :ascii_only?)
    assert_predicate(s.chomp("\u{3053 3093}"), :ascii_only?)

    # argument should be converted to String
    klass = Class.new { def to_str; 'a'; end }
    s = S("abba")
    assert_equal("abb", s.chomp(klass.new))
    assert_equal("abba", s)

    # chomp removes any of "\n", "\r\n", "\r" when "\n" is specified
    s = "foo\n"
    assert_equal("foo", s.chomp("\n"))
    s = "foo\r\n"
    assert_equal("foo", s.chomp("\n"))
    s = "foo\r"
    assert_equal("foo", s.chomp("\n"))
  ensure
    $/ = save
    $VERBOSE = verbose
  end

  def test_chomp!
    verbose, $VERBOSE = $VERBOSE, nil

    a = S("hello")
    a.chomp!(S("\n"))

    assert_equal(S("hello"), a)
    assert_equal(nil, a.chomp!(S("\n")))

    a = S("hello\n")
    a.chomp!(S("\n"))
    assert_equal(S("hello"), a)
    save = $/

    $/ = "\n"
    a = S("hello")
    a.chomp!
    assert_equal(S("hello"), a)

    a = S("hello\n")
    a.chomp!
    assert_equal(S("hello"), a)

    $/ = "!"
    a = S("hello")
    a.chomp!
    assert_equal(S("hello"), a)

    a="hello!"
    a.chomp!
    assert_equal(S("hello"), a)

    $/ = save

    a = S("hello\n")
    b = a.dup
    assert_equal(S("hello"), a.chomp!)
    assert_equal(S("hello\n"), b)

    s = "foo\r\n"
    s.chomp!
    assert_equal("foo", s)

    s = "foo\r"
    s.chomp!
    assert_equal("foo", s)

    s = "foo\r\n"
    s.chomp!("")
    assert_equal("foo", s)

    s = "foo\r"
    s.chomp!("")
    assert_equal("foo\r", s)

    assert_equal(S("a").hash, S("a\u0101").chomp!(S("\u0101")).hash, '[ruby-core:22414]')

    s = S("").freeze
    assert_raise_with_message(FrozenError, /frozen/) {s.chomp!}
    $VERBOSE = nil # EnvUtil.suppress_warning resets $VERBOSE to the original state

    s = S("ax")
    o = Struct.new(:s).new(s)
    def o.to_str
      s.freeze
      "x"
    end
    assert_raise_with_message(FrozenError, /frozen/) {s.chomp!(o)}
    $VERBOSE = nil # EnvUtil.suppress_warning resets $VERBOSE to the original state

    s = S("hello")
    assert_equal("hel", s.chomp!('lo'))
    assert_equal("hel", s)

    s = S("hello")
    assert_equal(nil, s.chomp!('he'))
    assert_equal("hello", s)

    s = S("\u{3053 3093 306b 3061 306f}")
    assert_equal("\u{3053 3093 306b}", s.chomp!("\u{3061 306f}"))
    assert_equal("\u{3053 3093 306b}", s)

    s = S("\u{3053 3093 306b 3061 306f}")
    assert_equal(nil, s.chomp!('lo'))
    assert_equal("\u{3053 3093 306b 3061 306f}", s)

    s = S("hello")
    assert_equal(nil, s.chomp!("\u{3061 306f}"))
    assert_equal("hello", s)

    # skip if argument is a broken string
    s = S("\xe3\x81\x82")
    assert_equal(nil, s.chomp!("\x82"))
    assert_equal("\xe3\x81\x82", s)

    s = S("\x95\x5c").force_encoding("Shift_JIS")
    assert_equal(nil, s.chomp!("\x5c"))
    assert_equal("\x95\x5c".force_encoding("Shift_JIS"), s)

    # clear coderange
    s = S("hello\u{3053 3093}")
    assert_not_predicate(s, :ascii_only