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
    assert_not_predicate(s, :ascii_only?)
    assert_predicate(s.chomp!("\u{3053 3093}"), :ascii_only?)

    # argument should be converted to String
    klass = Class.new { def to_str; 'a'; end }
    s = S("abba")
    assert_equal("abb", s.chomp!(klass.new))
    assert_equal("abb", s)

    # chomp removes any of "\n", "\r\n", "\r" when "\n" is specified
    s = "foo\n"
    assert_equal("foo", s.chomp!("\n"))
    s = "foo\r\n"
    assert_equal("foo", s.chomp!("\n"))
    s = "foo\r"
    assert_equal("foo", s.chomp!("\n"))
  ensure
    $/ = save
    $VERBOSE = verbose
  end

  def test_chop
    assert_equal(S("hell"),    S("hello").chop)
    assert_equal(S("hello"),   S("hello\r\n").chop)
    assert_equal(S("hello\n"), S("hello\n\r").chop)
    assert_equal(S(""),        S("\r\n").chop)
    assert_equal(S(""),        S("").chop)
    assert_equal(S("a").hash,  S("a\u00d8").chop.hash)
  end

  def test_chop!
    a = S("hello").chop!
    assert_equal(S("hell"), a)

    a = S("hello\r\n").chop!
    assert_equal(S("hello"), a)

    a = S("hello\n\r").chop!
    assert_equal(S("hello\n"), a)

    a = S("\r\n").chop!
    assert_equal(S(""), a)

    a = S("").chop!
    assert_nil(a)

    a = S("a\u00d8")
    a.chop!
    assert_equal(S("a").hash, a.hash)

    a = S("hello\n")
    b = a.dup
    assert_equal(S("hello"),   a.chop!)
    assert_equal(S("hello\n"), b)
  end

  def test_clone
    for frozen in [ false, true ]
      a = S("Cool")
      a.freeze if frozen
      b = a.clone

      assert_equal(a, b)
      assert_not_same(a, b)
      assert_equal(a.frozen?, b.frozen?)
    end

    assert_equal("", File.read(IO::NULL).clone, '[ruby-dev:32819] reported by Kazuhiro NISHIYAMA')
  end

  def test_concat
    assert_equal(S("world!"), S("world").concat(33))
    assert_equal(S("world!"), S("world").concat(S('!')))
    b = S("sn")
    assert_equal(S("snsnsn"), b.concat(b, b))

    bug7090 = '[ruby-core:47751]'
    result = S("").force_encoding(Encoding::UTF_16LE)
    result << 0x0300
    expected = S("\u0300".encode(Encoding::UTF_16LE))
    assert_equal(expected, result, bug7090)
    assert_raise(TypeError) { 'foo' << :foo }
    assert_raise(FrozenError) { 'foo'.freeze.concat('bar') }
  end

  def test_concat_literals
    s="." * 50
    assert_equal(Encoding::UTF_8, "#{s}x".encoding)
  end

  def test_count
    a = S("hello world")
    assert_equal(5, a.count(S("lo")))
    assert_equal(2, a.count(S("lo"), S("o")))
    assert_equal(4, a.count(S("hello"), S("^l")))
    assert_equal(4, a.count(S("ej-m")))
    assert_equal(0, S("y").count(S("a\\-z")))
    assert_equal(5, "abc\u{3042 3044 3046}".count("^a"))
    assert_equal(1, "abc\u{3042 3044 3046}".count("\u3042"))
    assert_equal(5, "abc\u{3042 3044 3046}".count("^\u3042"))
    assert_equal(2, "abc\u{3042 3044 3046}".count("a-z", "^a"))
    assert_equal(0, "abc\u{3042 3044 3046}".count("a", "\u3042"))
    assert_equal(0, "abc\u{3042 3044 3046}".count("\u3042", "a"))
    assert_equal(0, "abc\u{3042 3044 3046}".count("\u3042", "\u3044"))
    assert_equal(4, "abc\u{3042 3044 3046}".count("^a", "^\u3044"))
    assert_equal(4, "abc\u{3042 3044 3046}".count("^\u3044", "^a"))
    assert_equal(4, "abc\u{3042 3044 3046}".count("^\u3042", "^\u3044"))

    assert_raise(ArgumentError) { "foo".count }
  end

  def crypt_supports_des_crypt?
    /openbsd/ !~ RUBY_PLATFORM
  end

  def test_crypt
    if crypt_supports_des_crypt?
      pass      = "aaGUC/JkO9/Sc"
      good_salt = "aa"
      bad_salt  = "ab"
    else
      pass      = "$2a$04$0WVaz0pV3jzfZ5G5tpmHWuBQGbkjzgtSc3gJbmdy0GAGMa45MFM2."
      good_salt = "$2a$04$0WVaz0pV3jzfZ5G5tpmHWu"
      bad_salt  = "$2a$04$0WVaz0pV3jzfZ5G5tpmHXu"
    end
    assert_equal(S(pass), S("mypassword").crypt(S(good_salt)))
    assert_not_equal(S(pass), S("mypassword").crypt(S(bad_salt)))
    assert_raise(ArgumentError) {S("mypassword").crypt(S(""))}
    assert_raise(ArgumentError) {S("mypassword").crypt(S("\0a"))}
    assert_raise(ArgumentError) {S("mypassword").crypt(S("a\0"))}
    assert_raise(ArgumentError) {S("poison\u0000null").crypt(S("aa"))}
    WIDE_ENCODINGS.each do |enc|
      assert_raise(ArgumentError) {S("mypassword").crypt(S("aa".encode(enc)))}
      assert_raise(ArgumentError) {S("mypassword".encode(enc)).crypt(S("aa"))}
    end

    @cls == String and
      assert_no_memory_leak([], "s = ''; salt_proc = proc{#{(crypt_supports_des_crypt? ? '..' : good_salt).inspect}}", "#{<<~"begin;"}\n#{<<~'end;'}")

    begin;
      1000.times { s.crypt(-salt_proc.call).clear  }
    end;
  end

  def test_delete
    assert_equal(S("heo"),  S("hello").delete(S("l"), S("lo")))
    assert_equal(S("he"),   S("hello").delete(S("lo")))
    assert_equal(S("hell"), S("hello").delete(S("aeiou"), S("^e")))
    assert_equal(S("ho"),   S("hello").delete(S("ej-m")))

    assert_equal("a".hash, "a\u0101".delete("\u0101").hash, '[ruby-talk:329267]')
    assert_equal(true, "a\u0101".delete("\u0101").ascii_only?)
    assert_equal(true, "a\u3041".delete("\u3041").ascii_only?)
    assert_equal(false, "a\u3041\u3042".delete("\u3041").ascii_only?)

    assert_equal("a", "abc\u{3042 3044 3046}".delete("^a"))
    assert_equal("bc\u{3042 3044 3046}", "abc\u{3042 3044 3046}".delete("a"))
    assert_equal("\u3042", "abc\u{3042 3044 3046}".delete("^\u3042"))

    bug6160 = '[ruby-dev:45374]'
    assert_equal("", '\\'.delete('\\'), bug6160)
  end

  def test_delete!
    a = S("hello")
    a.delete!(S("l"), S("lo"))
    assert_equal(S("heo"), a)

    a = S("hello")
    a.delete!(S("lo"))
    assert_equal(S("he"), a)

    a = S("hello")
    a.delete!(S("aeiou"), S("^e"))
    assert_equal(S("hell"), a)

    a = S("hello")
    a.delete!(S("ej-m"))
    assert_equal(S("ho"), a)

    a = S("hello")
    assert_nil(a.delete!(S("z")))

    a = S("hello")
    b = a.dup
    a.delete!(S("lo"))
    assert_equal(S("he"), a)
    assert_equal(S("hello"), b)

    a = S("hello")
    a.delete!(S("^el"))
    assert_equal(S("ell"), a)

    assert_raise(ArgumentError) { S("foo").delete! }
  end


  def test_downcase
    assert_equal(S("hello"), S("helLO").downcase)
    assert_equal(S("hello"), S("hello").downcase)
    assert_equal(S("hello"), S("HELLO").downcase)
    assert_equal(S("abc hello 123"), S("abc HELLO 123").downcase)
    assert_equal(S("h\0""ello"), S("h\0""ELLO").downcase)
  end

  def test_downcase!
    a = S("helLO")
    b = a.dup
    assert_equal(S("hello"), a.downcase!)
    assert_equal(S("hello"), a)
    assert_equal(S("helLO"), b)

    a=S("hello")
    assert_nil(a.downcase!)
    assert_equal(S("hello"), a)

    a = S("h\0""ELLO")
    b = a.dup
    assert_equal(S("h\0""ello"), a.downcase!)
    assert_equal(S("h\0""ello"), a)
    assert_equal(S("h\0""ELLO"), b)
  end

  def test_dump
    a= S("Test") << 1 << 2 << 3 << 9 << 13 << 10
    assert_equal(S('"Test\\x01\\x02\\x03\\t\\r\\n"'), a.dump)
    b= S("\u{7F}")
    assert_equal(S('"\\x7F"'), b.dump)
    b= S("\u{AB}")
    assert_equal(S('"\\u00AB"'), b.dump)
    b= S("\u{ABC}")
    assert_equal(S('"\\u0ABC"'), b.dump)
    b= S("\uABCD")
    assert_equal(S('"\\uABCD"'), b.dump)
    b= S("\u{ABCDE}")
    assert_equal(S('"\\u{ABCDE}"'), b.dump)
    b= S("\u{10ABCD}")
    assert_equal(S('"\\u{10ABCD}"'), b.dump)
  end

  def test_undump
    a = S("Test") << 1 << 2 << 3 << 9 << 13 << 10
    assert_equal(a, S('"Test\\x01\\x02\\x03\\t\\r\\n"').undump)
    assert_equal(S("\\ca"), S('"\\ca"').undump)
    assert_equal(S("\u{7F}"), S('"\\x7F"').undump)
    assert_equal(S("\u{7F}A"), S('"\\x7FA"').undump)
    assert_equal(S("\u{AB}"), S('"\\u00AB"').undump)
    assert_equal(S("\u{ABC}"), S('"\\u0ABC"').undump)
    assert_equal(S("\uABCD"), S('"\\uABCD"').undump)
    assert_equal(S("\uABCD"), S('"\\uABCD"').undump)
    assert_equal(S("\u{ABCDE}"), S('"\\u{ABCDE}"').undump)
    assert_equal(S("\u{10ABCD}"), S('"\\u{10ABCD}"').undump)
    assert_equal(S("\u{ABCDE 10ABCD}"), S('"\\u{ABCDE 10ABCD}"').undump)
    assert_equal(S(""), S('"\\u{}"').undump)
    assert_equal(S(""), S('"\\u{  }"').undump)

    assert_equal(S("\u3042".encode("sjis")), S('"\x82\xA0"'.force_encoding("sjis")).undump)
    assert_equal(S("\u8868".encode("sjis")), S("\"\\x95\\\\\"".force_encoding("sjis")).undump)

    assert_equal(S("äöü"), S('"\u00E4\u00F6\u00FC"').undump)
    assert_equal(S("äöü"), S('"\xC3\xA4\xC3\xB6\xC3\xBC"').undump)

    assert_equal(Encoding::UTF_8, S('"\\u3042"').encode(Encoding::EUC_JP).undump.encoding)

    assert_equal("abc".encode(Encoding::UTF_16LE),
                 '"a\x00b\x00c\x00".force_encoding("UTF-16LE")'.undump)

    assert_equal('\#', '"\\\\#"'.undump)
    assert_equal('\#{', '"\\\\\#{"'.undump)

    assert_raise(RuntimeError) { S('\u3042').undump }
    assert_raise(RuntimeError) { S('"\x82\xA0\u3042"'.force_encoding("SJIS")).undump }
    assert_raise(RuntimeError) { S('"\u3042\x82\xA0"'.force_encoding("SJIS")).undump }
    assert_raise(RuntimeError) { S('"".force_encoding()').undump }
    assert_raise(RuntimeError) { S('"".force_encoding("').undump }
    assert_raise(RuntimeError) { S('"".force_encoding("UNKNOWN")').undump }
    assert_raise(RuntimeError) { S('"\u3042".force_encoding("UTF-16LE")').undump }
    assert_raise(RuntimeError) { S('"\x00\x00".force_encoding("UTF-16LE")"').undump }
    assert_raise(RuntimeError) { S('"\x00\x00".force_encoding("'+("a"*9999999)+'")"').undump }
    assert_raise(RuntimeError) { S(%("\u00E4")).undump }
    assert_raise(RuntimeError) { S('"').undump }
    assert_raise(RuntimeError) { S('"""').undump }
    assert_raise(RuntimeError) { S('""""').undump }

    assert_raise(RuntimeError) { S('"a').undump }
    assert_raise(RuntimeError) { S('"\u"').undump }
    assert_raise(RuntimeError) { S('"\u{"').undump }
    assert_raise(RuntimeError) { S('"\u304"').undump }
    assert_raise(RuntimeError) { S('"\u304Z"').undump }
    assert_raise(RuntimeError) { S('"\udfff"').undump }
    assert_raise(RuntimeError) { S('"\u{dfff}"').undump }
    assert_raise(RuntimeError) { S('"\u{3042"').undump }
    assert_raise(RuntimeError) { S('"\u{3042 "').undump }
    assert_raise(RuntimeError) { S('"\u{110000}"').undump }
    assert_raise(RuntimeError) { S('"\u{1234567}"').undump }
    assert_raise(RuntimeError) { S('"\x"').undump }
    assert_raise(RuntimeError) { S('"\xA"').undump }
    assert_raise(RuntimeError) { S('"\\"').undump }
    assert_raise(RuntimeError) { S(%("\0")).undump }
    assert_raise_with_message(RuntimeError, /invalid/) {
      '"\\u{007F}".xxxxxx'.undump
    }
  end

  def test_dup
    for frozen in [ false, true ]
      a = S("hello")
      a.freeze if frozen
      b = a.dup

      assert_equal(a, b)
      assert_not_same(a, b)
      assert_not_predicate(b, :frozen?)
    end
  end

  def test_each
    verbose, $VERBOSE = $VERBOSE, nil

    save = $/
    $/ = "\n"
    res=[]
    S("hello\nworld").lines.each {|x| res << x}
    assert_equal(S("hello\n"), res[0])
    assert_equal(S("world"),   res[1])

    res=[]
    S("hello\n\n\nworld").lines(S('')).each {|x| res << x}
    assert_equal(S("hello\n\n"), res[0])
    assert_equal(S("world"),     res[1])

    $/ = "!"
    res=[]
    S("hello!world").lines.each {|x| res << x}
    assert_equal(S("hello!"), res[0])
    assert_equal(S("world"),  res[1])
  ensure
    $/ = save
    $VERBOSE = verbose
  end

  def test_each_byte
    s = S("ABC")

    res = []
    assert_equal s.object_id, s.each_byte {|x| res << x }.object_id
    assert_equal(65, res[0])
    assert_equal(66, res[1])
    assert_equal(67, res[2])

    assert_equal 65, s.each_byte.next
  end

  def test_bytes
    s = S("ABC")
    assert_equal [65, 66, 67], s.bytes

    res = []
    assert_equal s.object_id, s.bytes {|x| res << x }.object_id
    assert_equal(65, res[0])
    assert_equal(66, res[1])
    assert_equal(67, res[2])
    s = S("ABC")
    res = []
    assert_same s, s.bytes {|x| res << x }
    assert_equal [65, 66, 67], res
  end

  def test_each_codepoint
    # Single byte optimization
    assert_equal 65, S("ABC").each_codepoint.next

    s = S("\u3042\u3044\u3046")

    res = []
    assert_equal s.object_id, s.each_codepoint {|x| res << x }.object_id
    assert_equal(0x3042, res[0])
    assert_equal(0x3044, res[1])
    assert_equal(0x3046, res[2])

    assert_equal 0x3042, s.each_codepoint.next
  end

  def test_codepoints
    # Single byte optimization
    assert_equal [65, 66, 67], S("ABC").codepoints

    s = S("\u3042\u3044\u3046")
    assert_equal [0x3042, 0x3044, 0x3046], s.codepoints

    res = []
    assert_equal s.object_id, s.codepoints {|x| res << x }.object_id
    assert_equal(0x3042, res[0])
    assert_equal(0x3044, res[1])
    assert_equal(0x3046, res[2])
    s = S("ABC")
    res = []
    assert_same s, s.codepoints {|x| res << x }
    assert_equal [65, 66, 67], res
  end

  def test_each_char
    s = S("ABC")

    res = []
    assert_equal s.object_id, s.each_char {|x| res << x }.object_id
    assert_equal("A", res[0])
    assert_equal("B", res[1])
    assert_equal("C", res[2])

    assert_equal "A", S("ABC").each_char.next
  end

  def test_chars
    s = S("ABC")
    assert_equal ["A", "B", "C"], s.chars

    res = []
    assert_equal s.object_id, s.chars {|x| res << x }.object_id
    assert_equal("A", res[0])
    assert_equal("B", res[1])
    assert_equal("C", res[2])
  end

  def test_each_grapheme_cluster
    [
      "\u{0D 0A}",
      "\u{20 200d