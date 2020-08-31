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
      "\u{20 200d}",
      "\u{600 600}",
      "\u{600 20}",
      "\u{261d 1F3FB}",
      "\u{1f600}",
      "\u{20 308}",
      "\u{1F477 1F3FF 200D 2640 FE0F}",
      "\u{1F468 200D 1F393}",
      "\u{1F46F 200D 2642 FE0F}",
      "\u{1f469 200d 2764 fe0f 200d 1f469}",
    ].each do |g|
      assert_equal [g], g.each_grapheme_cluster.to_a
      assert_equal 1, g.each_grapheme_cluster.size
    end

    [
      ["\u{a 324}", ["\u000A", "\u0324"]],
      ["\u{d 324}", ["\u000D", "\u0324"]],
      ["abc", ["a", "b", "c"]],
    ].each do |str, grapheme_clusters|
      assert_equal grapheme_clusters, str.each_grapheme_cluster.to_a
      assert_equal grapheme_clusters.size, str.each_grapheme_cluster.size
    end

    s = ("x"+"\u{10ABCD}"*250000)
    assert_empty(s.each_grapheme_cluster {s.clear})
  end

  def test_grapheme_clusters
    [
      "\u{20 200d}",
      "\u{600 600}",
      "\u{600 20}",
      "\u{261d 1F3FB}",
      "\u{1f600}",
      "\u{20 308}",
      "\u{1F477 1F3FF 200D 2640 FE0F}",
      "\u{1F468 200D 1F393}",
      "\u{1F46F 200D 2642 FE0F}",
      "\u{1f469 200d 2764 fe0f 200d 1f469}",
    ].product([Encoding::UTF_8, *WIDE_ENCODINGS]) do |g, enc|
      g = g.encode(enc)
      assert_equal [g], g.grapheme_clusters
    end

    [
      "\u{a 324}",
      "\u{d 324}",
      "abc",
    ].product([Encoding::UTF_8, *WIDE_ENCODINGS]) do |g, enc|
      g = g.encode(enc)
      assert_equal g.chars, g.grapheme_clusters
    end
    assert_equal ["a", "b", "c"], "abc".b.grapheme_clusters

    s = "ABC".b
    res = []
    assert_same s, s.grapheme_clusters {|x| res << x }
    assert_equal(3, res.size)
    assert_equal("A", res[0])
    assert_equal("B", res[1])
    assert_equal("C", res[2])
  end

  def test_each_line
    verbose, $VERBOSE = $VERBOSE, nil

    save = $/
    $/ = "\n"
    res=[]
    S("hello\nworld").each_line {|x| res << x}
    assert_equal(S("hello\n"), res[0])
    assert_equal(S("world"),   res[1])

    res=[]
    S("hello\n\n\nworld").each_line(S('')) {|x| res << x}
    assert_equal(S("hello\n\n"), res[0])
    assert_equal(S("world"),     res[1])

    res=[]
    S("hello\r\n\r\nworld").each_line(S('')) {|x| res << x}
    assert_equal(S("hello\r\n\r\n"), res[0])
    assert_equal(S("world"),         res[1])

    $/ = "!"

    res=[]
    S("hello!world").each_line {|x| res << x}
    assert_equal(S("hello!"), res[0])
    assert_equal(S("world"),  res[1])

    $/ = "ab"

    res=[]
    S("a").lines.each {|x| res << x}
    assert_equal(1, res.size)
    assert_equal(S("a"), res[0])

    $/ = save

    s = nil
    "foo\nbar".each_line(nil) {|s2| s = s2 }
    assert_equal("foo\nbar", s)

    assert_equal "hello\n", S("hello\nworld").each_line.next
    assert_equal "hello\nworld", S("hello\nworld").each_line(nil).next

    bug7646 = "[ruby-dev:46827]"
    assert_nothing_raised(bug7646) do
      "\n\u0100".each_line("\n") {}
    end
  ensure
    $/ = save
    $VERBOSE = verbose
  end

  def test_each_line_chomp
    res = []
    S("hello\nworld").each_line("\n", chomp: true) {|x| res << x}
    assert_equal(S("hello"), res[0])
    assert_equal(S("world"), res[1])

    res = []
    S("hello\n\n\nworld").each_line(S(''), chomp: true) {|x| res << x}
    assert_equal(S("hello\n"), res[0])
    assert_equal(S("world"),   res[1])

    res = []
    S("hello\r\n\r\nworld").each_line(S(''), chomp: true) {|x| res << x}
    assert_equal(S("hello\r\n"), res[0])
    assert_equal(S("world"),     res[1])

    res = []
    S("hello!world").each_line(S('!'), chomp: true) {|x| res << x}
    assert_equal(S("hello"), res[0])
    assert_equal(S("world"), res[1])

    res = []
    S("a").each_line(S('ab'), chomp: true).each {|x| res << x}
    assert_equal(1, res.size)
    assert_equal(S("a"), res[0])

    s = nil
    "foo\nbar".each_line(nil, chomp: true) {|s2| s = s2 }
    assert_equal("foo\nbar", s)

    assert_equal "hello", S("hello\nworld").each_line(chomp: true).next
    assert_equal "hello\nworld", S("hello\nworld").each_line(nil, chomp: true).next

    res = []
    S("").each_line(chomp: true) {|x| res << x}
    assert_equal([], res)

    res = []
    S("\n").each_line(chomp: true) {|x| res << x}
    assert_equal([S("")], res)

    res = []
    S("\r\n").each_line(chomp: true) {|x| res << x}
    assert_equal([S("")], res)

    res = []
    S("a\n b\n").each_line(" ", chomp: true) {|x| res << x}
    assert_equal([S("a\n"), S("b\n")], res)
  end

  def test_lines
    s = S("hello\nworld")
    assert_equal ["hello\n", "world"], s.lines
    assert_equal ["hello\nworld"], s.lines(nil)

    res = []
    assert_equal s.object_id, s.lines {|x| res << x }.object_id
    assert_equal(S("hello\n"), res[0])
    assert_equal(S("world"),  res[1])
  end

  def test_empty?
    assert_empty(S(""))
    assert_not_empty(S("not"))
  end

  def test_end_with?
    assert_send([S("hello"), :end_with?, S("llo")])
    assert_not_send([S("hello"), :end_with?, S("ll")])
    assert_send([S("hello"), :end_with?, S("el"), S("lo")])
    assert_send([S("hello"), :end_with?, S("")])
    assert_not_send([S("hello"), :end_with?])

    bug5536 = '[ruby-core:40623]'
    assert_raise(TypeError, bug5536) {S("str").end_with? :not_convertible_to_string}
  end

  def test_eql?
    a = S("hello")
    assert_operator(a, :eql?, S("hello"))
    assert_operator(a, :eql?, a)
  end

  def test_gsub
    assert_equal(S("h*ll*"),     S("hello").gsub(/[aeiou]/, S('*')))
    assert_equal(S("h<e>ll<o>"), S("hello").gsub(/([aeiou])/, S('<\1>')))
    assert_equal(S("h e l l o "),
                 S("hello").gsub(/./) { |s| s[0].to_s + S(' ')})
    assert_equal(S("HELL-o"),
                 S("hello").gsub(/(hell)(.)/) { |s| $1.upcase + S('-') + $2 })
    assert_equal(S("<>h<>e<>l<>l<>o<>"), S("hello").gsub(S(''), S('<\0>')))

    assert_equal("z", "abc".gsub(/./, "a" => "z"), "moved from btest/knownbug")

    assert_raise(ArgumentError) { "foo".gsub }
  end

  def test_gsub_encoding
    a = S("hello world")
    a.force_encoding Encoding::UTF_8

    b = S("hi")
    b.force_encoding Encoding::US_ASCII

    assert_equal Encoding::UTF_8, a.gsub(/hello/, b).encoding

    c = S("everybody")
    c.force_encoding Encoding::US_ASCII

    assert_equal Encoding::UTF_8, a.gsub(/world/, c).encoding

    assert_equal S("a\u{e9}apos&lt;"), S("a\u{e9}'&lt;").gsub("'", "apos")

    bug9849 = '[ruby-core:62669] [Bug #9849]'
    assert_equal S("\u{3042 3042 3042}!foo!"), S("\u{3042 3042 3042}/foo/").gsub("/", "!"), bug9849
  end

  def test_gsub!
    a = S("hello")
    b = a.dup
    a.gsub!(/[aeiou]/, S('*'))
    assert_equal(S("h*ll*"), a)
    assert_equal(S("hello"), b)

    a = S("hello")
    a.gsub!(/([aeiou])/, S('<\1>'))
    assert_equal(S("h<e>ll<o>"), a)

    a = S("hello")
    a.gsub!(/./) { |s| s[0].to_s + S(' ')}
    assert_equal(S("h e l l o "), a)

    a = S("hello")
    a.gsub!(/(hell)(.)/) { |s| $1.upcase + S('-') + $2 }
    assert_equal(S("HELL-o"), a)

    a = S("hello")
    assert_nil(a.sub!(S('X'), S('Y')))
  end

  def test_sub_hash
    assert_equal('azc', 'abc'.sub(/b/, "b" => "z"))
    assert_equal('ac', 'abc'.sub(/b/, {}))
    assert_equal('a1c', 'abc'.sub(/b/, "b" => 1))
    assert_equal('aBc', 'abc'.sub(/b/, Hash.new {|h, k| k.upcase }))
    assert_equal('a[\&]c', 'abc'.sub(/b/, "b" => '[\&]'))
    assert_equal('aBcabc', 'abcabc'.sub(/b/, Hash.new {|h, k| h[k] = k.upcase }))
    assert_equal('aBcdef', 'abcdef'.sub(/de|b/, "b" => "B", "de" => "DE"))
  end

  def test_gsub_hash
    assert_equal('azc', 'abc'.gsub(/b/, "b" => "z"))
    assert_equal('ac', 'abc'.gsub(/b/, {}))
    assert_equal('a1c', 'abc'.gsub(/b/, "b" => 1))
    assert_equal('aBc', 'abc'.gsub(/b/, Hash.new {|h, k| k.upcase }))
    assert_equal('a[\&]c', 'abc'.gsub(/b/, "b" => '[\&]'))
    assert_equal('aBcaBc', 'abcabc'.gsub(/b/, Hash.new {|h, k| h[k] = k.upcase }))
    assert_equal('aBcDEf', 'abcdef'.gsub(/de|b/, "b" => "B", "de" => "DE"))
  end

  def test_hash
    assert_equal(S("hello").hash, S("hello").hash)
    assert_not_equal(S("hello").hash, S("helLO").hash)
    bug4104 = '[ruby-core:33500]'
    assert_not_equal(S("a").hash, S("a\0").hash, bug4104)
    bug9172 = '[ruby-core:58658] [Bug #9172]'
    assert_not_equal(S("sub-setter").hash, S("discover").hash, bug9172)
  end

  def test_hex
    assert_equal(255,  S("0xff").hex)
    assert_equal(-255, S("-0xff").hex)
    assert_equal(255,  S("ff").hex)
    assert_equal(-255, S("-ff").hex)
    assert_equal(0,    S("-ralph").hex)
    assert_equal(-15,  S("-fred").hex)
    assert_equal(15,   S("fred").hex)
  end

  def test_include?
    assert_include(S("foobar"), ?f)
    assert_include(S("foobar"), S("foo"))
    assert_not_include(S("foobar"), S("baz"))
    assert_not_include(S("foobar"), ?z)
  end

  def test_index
    assert_equal(0, S("hello").index(?h))
    assert_equal(1, S("hello").index(S("ell")))
    assert_equal(2, S("hello").index(/ll./))

    assert_equal(3, S("hello").index(?l, 3))
    assert_equal(3, S("hello").index(S("l"), 3))
    assert_equal(3, S("hello").index(/l./, 3))

    assert_nil(S("hello").index(?z, 3))
    assert_nil(S("hello").index(S("z"), 3))
    assert_nil(S("hello").index(/z./, 3))

    assert_nil(S("hello").index(?z))
    assert_nil(S("hello").index(S("z")))
    assert_nil(S("hello").index(/z./))

    assert_equal(0, S("").index(S("")))
    assert_equal(0, S("").index(//))
    assert_nil(S("").index(S("hello")))
    assert_nil(S("").index(/hello/))
    assert_equal(0, S("hello").index(S("")))
    assert_equal(0, S("hello").index(//))

    s = S("long") * 1000 << "x"
    assert_nil(s.index(S("y")))
    assert_equal(4 * 1000, s.index(S("x")))
    s << "yx"
    assert_equal(4 * 1000, s.index(S("x")))
    assert_equal(4 * 1000, s.index(S("xyx")))

    o = Object.new
    def o.to_str; "bar"; end
    assert_equal(3, "foobarbarbaz".index(o))
    assert_raise(TypeError) { "foo".index(Object.new) }

    assert_nil("foo".index(//, -100))
    assert_nil($~)

    assert_equal(2, S("abcdbce").index(/b\Kc/))
  end

  def test_insert
    assert_equal("Xabcd", S("abcd").insert(0, 'X'))
    assert_equal("abcXd", S("abcd").insert(3, 'X'))
    assert_equal("abcdX", S("abcd").insert(4, 'X'))
    assert_equal("abXcd", S("abcd").insert(-3, 'X'))
    assert_equal("abcdX", S("abcd").insert(-1, 'X'))
  end

  def test_intern
    assert_equal(:koala, S("koala").intern)
    assert_not_equal(:koala, S("Koala").intern)
  end

  def test_length
    assert_equal(0, S("").length)
    assert_equal(4, S("1234").length)
    assert_equal(6, S("1234\r\n").length)
    assert_equal(7, S("\0011234\r\n").length)
  end

  def test_ljust
    assert_equal(S("hello"),       S("hello").ljust(4))
    assert_equal(S("hello      "), S("hello").ljust(11))
    assert_equal(S("ababababab"), S("").ljust(10, "ab"), Bug2463)
    assert_equal(S("abababababa"), S("").ljust(11, "ab"), Bug2463)
  end

  def test_next
    assert_equal(S("abd"), S("abc").next)
    assert_equal(S("z"),   S("y").next)
    assert_equal(S("aaa"), S("zz").next)

    assert_equal(S("124"),  S("123").next)
    assert_equal(S("1000"), S("999").next)

    assert_equal(S("2000aaa"),  S("1999zzz").next)
    assert_equal(S("AAAAA000"), S("ZZZZ999").next)

    assert_equal(S("*+"), S("**").next)

    assert_equal(S("!"), S(" ").next)
    assert_equal(S(""), S("").next)
  end

  def test_next!
    a = S("abc")
    b = a.dup
    assert_equal(S("abd"), a.next!)
    assert_equal(S("abd"), a)
    assert_equal(S("abc"), b)

    a = S("y")
    assert_equal(S("z"), a.next!)
    assert_equal(S("z"), a)

    a = S("zz")
    assert_equal(S("aaa"), a.next!)
    assert_equal(S("aaa"), a)

    a = S("123")
    assert_equal(S("124"), a.next!)
    assert_equal(S("124"), a)

    a = S("999")
    assert_equal(S("1000"), a.next!)
    assert_equal(S("1000"), a)

    a = S("1999zzz")
    assert_equal(S("2000aaa"), a.next!)
    assert_equal(S("2000aaa"), a)

    a = S("ZZZZ999")
    assert_equal(S("AAAAA000"), a.next!)
    assert_equal(S("AAAAA000"), a)

    a = S("**")
    assert_equal(S("*+"), a.next!)
    assert_equal(S("*+"), a)

    a = S(" ")
    assert_equal(S("!"), a.next!)
    assert_equal(S("!"), a)
  end

  def test_oct
    assert_equal(255,  S("0377").oct)
    assert_equal(255,  S("377").oct)
    assert_equal(-255, S("-0377").oct)
    assert_equal(-255, S("-377").oct)
    assert_equal(0,    S("OO").oct)
    assert_equal(24,   S("030OO").oct)
  end

  def test_replace
    a = S("foo")
    assert_equal(S("f"), a.replace(S("f")))

    a = S("foo")
    assert_equal(S("foobar"), a.replace(S("foobar")))

    a = S("foo")
    b = a.replace(S("xyz"))
    assert_equal(S("xyz"), b)

    s = "foo" * 100
    s2 = ("bar" * 100).dup
    s.replace(s2)
    assert_equal(s2, s)

    s2 = ["foo"].pack("p")
    s.replace(s2)
    assert_equal(s2, s)

    fs = "".freeze
    assert_raise(FrozenError) { fs.replace("a") }
    assert_raise(FrozenError) { fs.replace(fs) }
    assert_raise(ArgumentError) { fs.replace() }
    assert_raise(FrozenError) { fs.replace(42) }
  end

  def test_reverse
    assert_equal(S("beta"), S("ateb").reverse)
    assert_equal(S("madamImadam"), S("madamImadam").reverse)

    a=S("beta")
    assert_equal(S("ateb"), a.reverse)
    assert_equal(S("beta"), a)
  end

  def test_reverse!
    a = S("beta")
    b = a.dup
    assert_equal(S("ateb"), a.reverse!)
    assert_equal(S("ateb"), a)
    assert_equal(S("beta"), b)

    assert_equal(S("madamImadam"), S("madamImadam").reverse!)

    a = S("madamImadam")
    assert_equal(S("madamImadam"), a.reverse!)  # ??
    assert_equal(S("madamImadam"), a)
  end

  def test_rindex
    assert_equal(3, S("hello").rindex(?l))
    assert_equal(6, S("ell, hello").rindex(S("ell")))
    assert_equal(7, S("ell, hello").rindex(/ll./))

    assert_equal(3, S("hello,lo").rindex(?l, 3))
    assert_equal(3, S("hello,lo").rindex(S("l"), 3))
    assert_equal(3, S("hello,lo").rindex(/l./, 3))

    assert_nil(S("hello").rindex(?z,     3))
    assert_nil(S("hello").rindex(S("z"), 3))
    assert_nil(S("hello").rindex(/z./,   3))

    assert_nil(S("hello").rindex(?z))
    assert_nil(S("hello").rindex(S("z")))
    assert_nil(S("hello").rindex(/z./))

    o = Object.new
    def o.to_str; "bar"; end
    assert_equal(6, "foobarbarbaz".rindex(o))
    assert_raise(TypeError) { "foo".rindex(Object.new) }

    assert_nil("foo".rindex(//, -100))
    assert_nil($~)

    assert_equal(3, "foo".rindex(//))
    assert_equal([3, 3], $~.offset(0))

    assert_equal(5, S("abcdbce").rindex(/b\Kc/))
  end

  def test_rjust
    assert_equal(S("hello"), S("hello").rjust(4))
    assert_equal(S("      hello"), S("hello").rjust(11))
    assert_equal(S("ababababab"), S("").rjust(10, "ab"), Bug2463)
    assert_equal(S("abababababa"), S("").rjust(11, "ab"), Bug2463)
  end

  def test_scan
    a = S("cruel world")
    assert_equal([S("cruel"), S("world")],a.scan(/\w+/))
    assert_equal([S("cru"), S("el "), S("wor")],a.scan(/.../))
    assert_equal([[S("cru")], [S("el ")], [S("wor")]],a.scan(/(...)/))

    res = []
    a.scan(/\w+/) { |w| res << w }
    assert_equal([S("cruel"), S("world") ],res)

    res = []
    a.scan(/.../) { |w| res << w }
    assert_equal([S("cru"), S("el "), S("wor")],res)

    res = []
    a.scan(/(...)/) { |w| res << w }
    assert_equal([[S("cru")], [S("el ")], [S("wor")]],res)

    /h/ =~ a
    a.scan(/x/)
    assert_nil($~)

    /h/ =~ a
    a.scan('x')
    assert_nil($~)

    assert_equal(%w[1 2 3], S("a1 a2 a3").scan(/a\K./))
  end

  def test_size
    assert_equal(0, S("").size)
    assert_equal(4, S("1234").size)
    assert_equal(6, S("1234\r\n").size)
    assert_equal(7, S("\0011234\r\n").size)
  end

  def test_slice
    assert_equal(?A, S("AooBar").slice(0))
    assert_equal(?B, S("FooBaB").slice(-1))
    assert_nil(S("FooBar").slice(6))
    assert_nil(S("FooBar").slice(-7))

    assert_equal(S("Foo"), S("FooBar").slice(0,3))
    assert_equal(S(S("Bar")), S("FooBar").slice(-3,3))
    assert_nil(S("FooBar").slice(7,2))     # Maybe should be six?
    assert_nil(S("FooBar").slice(-7,10))

    assert_equal(S("Foo"), S("FooBar").slice(0..2))
    assert_equal(S("Bar"), S("FooBar").slice(-3..-1))
    assert_equal(S(""), S("FooBar").slice(6..2))
    assert_nil(S("FooBar").slice(-10..-7))

    assert_equal(S("Foo"), S("FooBar").slice(/^F../))
    assert_equal(S("Bar"), S("FooBar").slice(/..r$/))
    assert_nil(S("FooBar").slice(/xyzzy/))
    assert_nil(S("FooBar").slice(/plugh/))

    assert_equal(S("Foo"), S("FooBar").slice(S("Foo")))
    assert_equal(S("Bar"), S("FooBar").slice(S("Bar")))
    assert_nil(S("FooBar").slice(S("xyzzy")))
    assert_nil(S("FooBar").slice(S("plugh")))

    bug9882 = '[ruby-core:62842] [Bug #9882]'
    substr = S("\u{30c6 30b9 30c8 2019}#{bug9882}").slice(4..-1)
    assert_equal(S(bug9882).hash, substr.hash, bug9882)
    assert_predicate(substr, :ascii_only?, bug9882)
  end

  def test_slice!
    a = S("AooBar")
    b = a.dup
    assert_equal(?A, a.slice!(0))
    assert_equal(S("ooBar"), a)
    assert_equal(S("AooBar"), b)

    a = S("FooBar")
    assert_equal(?r,a.slice!(-1))
    assert_equal(S("FooBa"), a)

    a = S("FooBar")
    if @aref_slicebang_silent
      assert_nil( a.slice!(6) )
      assert_nil( a.slice!(6r) )
    else
      assert_raise(IndexError) { a.slice!(6) }
      assert_raise(IndexError) { a.slice!(6r) }
    end
    assert_equal(S("FooBar"), a)

    if @aref_slicebang_silent
      assert_nil( a.slice!(-7) )
    else
      assert_raise(IndexError) { a.slice!(-7) }
    end
    assert_equal(S("FooBar"), a)

    a = S("FooBar")
    assert_equal(S("Foo"), a.slice!(0,3))
    assert_equal(S("Bar"), a)

    a = S("FooBar")
    assert_equal(S("Bar"), a.slice!(-3,3))
    assert_equal(S("Foo"), a)

    a=S("FooBar")
    if @aref_slicebang_silent
    assert_nil(a.slice!(7,2))      # Maybe should be six?
    else
    assert_raise(IndexError) {a.slice!(7,2)}     # Maybe should be six?
    end
    assert_equal(S("FooBar"), a)
    if @aref_slicebang_silent
    assert_nil(a.slice!(-7,10))
    else
    assert_raise(IndexError) {a.slice!(-7,10)}
    end
    assert_equal(S("FooBar"), a)

    a=S("FooBar")
    assert_equal(S("Foo"), a.slice!(0..2))
    assert_equal(S("Bar"), a)

    a=S("FooBar")
    assert_equal(S("Bar"), a.slice!(-3..-1))
    assert_equal(S("Foo"), a)

    a=S("FooBar")
    if @aref_slicebang_silent
    assert_equal(S(""), a.slice!(6..2))
    else
    assert_raise(RangeError) {a.slice!(6..2)}
    end
    assert_equal(S("FooBar"), a)
    if @aref_slicebang_silent
    assert_nil(a.slice!(-10..-7))
    else
    assert_raise(RangeError) {a.slice!(-10..-7)}
    end
    assert_equal(S("FooBar"), a)

    a=S("FooBar")
    assert_equal(S("Foo"), a.slice!(/^F../))
    assert_equal(S("Bar"), a)

    a=S("FooBar")
    assert_equal(S("Bar"), a.slice!(/..r$/))
    assert_equal(S("Foo"), a)

    a=S("FooBar")
    if @aref_slicebang_silent
      assert_nil(a.slice!(/xyzzy/))
    else
      assert_raise(IndexError) {a.slice!(/xyzzy/)}
    end
    assert_equal(S("FooBar"), a)
    if @aref_slicebang_silent
      assert_nil(a.slice!(/plugh/))
    else
      assert_raise(IndexError) {a.slice!(/plugh/)}
    end
    assert_equal(S("FooBar"), a)

    a=S("FooBar")
    assert_equal(S("Foo"), a.slice!(S("Foo")))
    assert_equal(S("Bar"), a)

    a=S("FooBar")
    assert_equal(S("Bar"), a.slice!(S("Bar")))
    assert_equal(S("Foo"), a)

    assert_raise(ArgumentError) { "foo".slice! }
  end

  def test_split
    fs, $; = $;, nil
    assert_equal([S("a"), S("b"), S("c")], S(" a   b\t c ").split)
    assert_equal([S("a"), S("b"), S("c")], S(" a   b\t c ").split(S(" ")))

    assert_equal([S(" a "), S(" b "), S(" c ")], S(" a | b | c ").split(S("|")))

    assert_equal([S("a"), S("b"), S("c")], S("aXXbXXcXX").split(/X./))

    assert_equal([S("a"), S("b"), S("c")], S("abc").split(//))

    assert_equal([S("a|b|c")], S("a|b|c").split(S('|'), 1))

    assert_equal([S("a"), S("b|c")], S("a|b|c").split(S('|'), 2))
    assert_equal([S("a"), S("b"), S("c")], S("a|b|c").split(S('|'), 3))

    assert_equal([S("a"), S("b"), S("c"), S("")], S("a|b|c|").split(S('|'), -1))
    assert_equal([S("a"), S("b"), S("c"), S(""), S("")], S("a|b|c||").split(S('|'), -1))

    assert_equal([S("a"), S(""), S("b"), S("c")], S("a||b|c|").split(S('|')))
    assert_equal([S("a"), S(""), S("b"), S("c"), S("")], S("a||b|c|").split(S('|'), -1))

    assert_equal([], "".split(//, 1))
  ensure
    EnvUtil.suppress_warning {$; = fs}
  end

  def test_split_with_block
    fs, $; = $;, nil
    result = []; S(" a   b\t c ").split {|s| result << s}
    assert_equal([S("a"), S("b"), S("c")], result)
    result = []; S(" a   b\t c ").split(S(" ")) {|s| result << s}
    assert_equal([S("a"), S("b"), S("c")], result)

    result = []; S(" a | b | c ").split(S("|")) {|s| result << s}
    assert_equal([S(" a "), S(" b "), S(" c ")], result)

    result = []; S("aXXbXXcXX").split(/X./) {|s| result << s}
    assert_equal([S("a"), S("b"), S("c")], result)

    result = []; S("abc").split(//) {|s| result << s}
    assert_equal([S("a"), S("b"), S("c")], result)

    result = []; S("a|b|c").split(S('|'), 1) {|s| result << s}
    assert_equal([S("a|b|c")], result)

    result = []; S("a|b|c").split(S('|'), 2) {|s| result << s}
    assert_equal([S("a"), S("b|c")], result)
    result = []; S("a|b|c").split(S('|'), 3) {|s| result << s}
    assert_equal([S("a"), S("b"), S("c")], result)

    result = []; S("a|b|c|").split(S('|'), -1) {|s| result << s}
    assert_equal([S("a"), S("b"), S("c"), S("")], result)
    result = []; S("a|b|c||").split(S('|'), -1) {|s| result << s}
    assert_equal([S("a"), S("b"), S("c"), S(""), S("")], result)

    result = []; S("a||b|c|").split(S('|')) {|s| result << s}
    assert_equal([S("a"), S(""), S("b"), S("c")], result)
    result = []; S("a||b|c|").split(S('|'), -1) {|s| result << s}
    assert_equal([S("a"), S(""), S("b"), S("c"), S("")], result)

    result = []; "".split(//, 1) {|s| result << s}
    assert_equal([], result)

    result = []; "aaa,bbb,ccc,ddd".split(/,/) {|s| result << s.gsub(/./, "A")}
    assert_equal(["AAA"]*4, result)
  ensure
    EnvUtil.suppress_warning {$; = fs}
  end

  def test_fs
    assert_raise_with_message(TypeError, /\$;/) {
      $; = []
    }

    assert_separately(%W[-W0], "#{<<~"begin;"}\n#{<<~'end;'}")
    bug = '[ruby-core:79582] $; must not be GCed'
    begin;
      $; = " "
      $a = nil
      alias $; $a
      alias $-F $a
      GC.start
      assert_equal([], "".split, bug)
    end;
  end

  def test_split_encoding
    bug6206 = '[ruby-dev:45441]'
    Encoding.list.each do |enc|
      next unless enc.ascii_compatible?
      s = S("a:".force_encoding(enc))
      assert_equal([enc]*2, s.split(":", 2).map(&:encoding), bug6206)
    end
  end

  def test_split_wchar
    bug8642 = '[ruby-core:56036] [Bug #8642]'
    WIDE_ENCODINGS.each do |enc|
      s = S("abc,def".encode(enc))
      assert_equal(["abc", "def"].map {|c| c.encode(enc)},
                   s.split(",".encode(enc)),
                   "#{bug8642} in #{enc.name}")
    end
  end

  def test_split_invalid_sequence
    bug10886 = '[ruby-core:68229] [Bug #10886]'
    broken = S("\xa1".force_encoding("utf-8"))
    assert_raise(ArgumentError, bug10886) {
      S("a,b").split(broken)
    }
  end

  def test_split_invalid_argument
    assert_raise(TypeError) {
      S("a,b").split(BasicObject.new)
    }
  end

  def test_split_dupped
    s = "abc"
    s.split("b", 1).map(&:upcase!)
    assert_equal("abc", s)
  end

  def test_split_lookbehind
    assert_equal([S("ab"), S("d")], S("abcd").split(/(?<=b)c/))
    assert_equal([S("ab"), S("d")], S("abcd").split(/b\Kc/))
  end

  def test_squeeze
    assert_equal(S("abc"), S("aaabbbbccc").squeeze)
    assert_equal(S("aa bb cc"), S("aa   bb      cc").squeeze(S(" ")))
    assert_equal(S("BxTyWz"), S("BxxxTyyyWzzzzz").squeeze(S("a-z")))
  end

  def test_squeeze!
    a = S("aaabbbbccc")
    b = a.dup
    assert_equal(S("abc"), a.squeeze!)
    assert_equal(S("abc"), a)
    assert_equal(S("aaabbbbccc"), b)

    a = S("aa   bb      cc")
    assert_equal(S("aa bb cc"), a.squeeze!(S(" ")))
    assert_equal(S("aa bb cc"), a)

    a = S("BxxxTyyyWzzzzz")
    assert_equal(S("BxTyWz"), a.squeeze!(S("a-z")))
    assert_equal(S("BxTyWz"), a)

    a=S("The quick brown fox")
    assert_nil(a.squeeze!)
  end

  def test_start_with?
    assert_send([S("hello"), :start_with?, S("hel")])
    assert_not_send([S("hello"), :start_with?, S("el")])
    assert_send([S("hello"), :start_with?, S("el"), S("he")])

    bug5536 = '[ruby-core:40623]'
    assert_raise(TypeError, bug5536) {S("str").start_with? :not_convertible_to_string}

    assert_equal(true, "hello".start_with?(/hel/))
    assert_equal("hel", $&)
    assert_equal(false, "hello".start_with?(/el/))
    assert_nil($&)
  end

  def test_strip
    assert_equal(S("x"), S("      x        ").strip)
    assert_equal(S("x"), S(" \n\r\t     x  \t\r\n\n      ").strip)
    assert_equal(S("x"), S("\x00x\x00").strip)

    assert_equal("0b0 ".force_encoding("UTF-16BE"),
                 "\x00 0b0 ".force_encoding("UTF-16BE").strip)
    assert_equal("0\x000b0 ".force_encoding("UTF-16BE"),
                 "0\x000b0 ".force_encoding("UTF-16BE").strip)
  end

  def test_strip!
    a = S("      x        ")
    b = a.dup
    assert_equal(S("x") ,a.strip!)
    assert_equal(S("x") ,a)
    assert_equal(S("      x        "), b)

    a = S(" \n\r\t     x  \t\r\n\n      ")
    assert_equal(S("x"), a.strip!)
    assert_equal(S("x"), a)

    a = S("\x00x\x00")
    assert_equal(S("x"), a.strip!)
    assert_equal(S("x"), a)

    a = S("x")
    assert_nil(a.strip!)
    assert_equal(S("x") ,a)
  end

  def test_sub
    assert_equal(S("h*llo"),    S("hello").sub(/[aeiou]/, S('*')))
    assert_equal(S("h<e>llo"),  S("hello").sub(/([aeiou])/, S('<\1>')))
    assert_equal(S("h ello"), S("hello").sub(/./) {
                   |s| s[0].to_s + S(' ')})
    assert_equal(S("HELL-o"),   S("hello").sub(/(hell)(.)/) {
                   |s| $1.upcase + S('-') + $2
                   })
    assert_equal(S("h<e>llo"),  S("hello").sub('e', S('<\0>')))

    assert_equal(S("a\\aba"), S("ababa").sub(/b/, '\\'))
    assert_equal(S("ab\\aba"), S("ababa").sub(/(b)/, '\1\\'))
    assert_equal(S("ababa"), S("ababa").sub(/(b)/, '\1'))
    assert_equal(S("ababa"), S("ababa").sub(/(b)/, '\\1'))
    assert_equal(S("a\\1aba"), S("ababa").sub(/(b)/, '\\\1'))
    assert_equal(S("a\\1aba"), S("ababa").sub(/(b)/, '\\\\1'))
    assert_equal(S("a\\baba"), S("ababa").sub(/(b)/, '\\\\\1'))

    assert_equal(S("a--ababababababababab"),
		 S("abababababababababab").sub(/(b)/, '-\9-'))
    assert_equal(S("1-b-0"),
		 S("1b2b3b4b5b6b7b8b9b0").
		 