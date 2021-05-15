# frozen_string_literal: false
require 'test/unit'
require 'tempfile'
require_relative 'marshaltestlib'

class TestMarshal < Test::Unit::TestCase
  include MarshalTestLib

  def setup
    @verbose = $VERBOSE
  end

  def teardown
    $VERBOSE = @verbose
  end

  def encode(o)
    Marshal.dump(o)
  end

  def decode(s)
    Marshal.load(s)
  end

  def fact(n)
    return 1 if n == 0
    f = 1
    while n>0
      f *= n
      n -= 1
    end
    return f
  end

  def test_marshal
    a = [1, 2, 3, [4,5,"foo"], {1=>"bar"}, 2.5, fact(30)]
    assert_equal a, Marshal.load(Marshal.dump(a))

    [[1,2,3,4], [81, 2, 118, 3146]].each { |w,x,y,z|
      obj = (x.to_f + y.to_f / z.to_f) * Math.exp(w.to_f / (x.to_f + y.to_f / z.to_f))
      assert_equal obj, Marshal.load(Marshal.dump(obj))
    }

    bug3659 = '[ruby-dev:41936]'
    [1.0, 10.0, 100.0, 110.0].each {|x|
      assert_equal(x, Marshal.load(Marshal.dump(x)), bug3659)
    }
  end

  StrClone = String.clone
  def test_marshal_cloned_class
    assert_instance_of(StrClone, Marshal.load(Marshal.dump(StrClone.new("abc"))))
  end

  def test_inconsistent_struct
    TestMarshal.const_set :StructOrNot, Struct.new(:a)
    s = Marshal.dump(StructOrNot.new(1))
    TestMarshal.instance_eval { remove_const :StructOrNot }
    TestMarshal.const_set :StructOrNot, Class.new
    assert_raise(TypeError, "[ruby-dev:31709]") { Marshal.load(s) }
  ensure
    TestMarshal.instance_eval { remove_const :StructOrNot }
  end

  def test_struct_invalid_members
    TestMarshal.const_set :StructInvalidMembers, Struct.new(:a)
    assert_raise(TypeError, "[ruby-dev:31759]") {
      Marshal.load("\004\bIc&TestMarshal::StructInvalidMembers\006:\020__members__\"\bfoo")
      TestMarshal::StructInvalidMembers.members
    }
  ensure
    TestMarshal.instance_eval { remove_const :StructInvalidMembers }
  end

  class C
    def initialize(str)
      @str = str
    end
    attr_reader :str
    def _dump(limit)
      @str
    end
    def self._load(s)
      new(s)
    end
  end

  def test_too_long_string
    data = Marshal.dump(C.new("a".force_encoding("ascii-8bit")))
    data[-2, 1] = "\003\377\377\377"
    assert_raise_with_message(ArgumentError, "marshal data too short", "[ruby-dev:32054]") {
      Marshal.load(data)
    }
  end


  def test_userdef_encoding
    s1 = "\xa4\xa4".force_encoding("euc-jp")
    o1 = C.new(s1)
    m = Marshal.dump(o1)
    o2 = Marshal.load(m)
    s2 = o2.str
    assert_equal(s1, s2)
  end

  def test_pipe
    o1 = C.new("a" * 10000)

    IO.pipe do |r, w|
      th = Thread.new {Marshal.dump(o1, w)}
      o2 = Marshal.load(r)
      th.join
      assert_equal(o1.str, o2.str)
    end

    IO.pipe do |r, w|
      th = Thread.new {Marshal.dump(o1, w, 2)}
      o2 = Marshal.load(r)
      th.join
      assert_equal(o1.str, o2.str)
    end

    assert_raise(TypeError) { Marshal.dump("foo", Object.new) }
    assert_raise(TypeError) { Marshal.load(Object.new) }
  end

  def test_limit
    assert_equal([[[]]], Marshal.load(Marshal.dump([[[]]], 3)))
    assert_raise(ArgumentError) { Marshal.dump([[[]]], 2) }
    assert_nothing_raised(ArgumentError, '[ruby-core:24100]') { Marshal.dump("\u3042", 1) }
  end

  def test_userdef_invalid
    o = C.new(nil)
    assert_raise(TypeError) { Marshal.dump(o) }
  end

  def test_class
    o = class << Object.new; self; end
    assert_raise(TypeError) { Marshal.dump(o) }
    assert_equal(Object, Marshal.load(Marshal.dump(Object)))
    assert_equal(Enumerable, Marshal.load(Marshal.dump(Enumerable)))
  end

  class C2
    def initialize(ary)
      @ary = ary
    end
    def _dump(s)
      @ary.clear
      "foo"
    end
  end

  def test_modify_array_during_dump
    a = []
    o = C2.new(a)
    a << o << nil
    assert_raise(RuntimeError) { Marshal.dump(a) }
  end

  def test_change_class_name
    self.class.__send__(:remove_const, :C3) if self.class.const_defined?(:C3)
    eval("class C3; def _dump(s); 'foo'; end; end")
    m = Marshal.dump(C3.new)
    assert_raise(TypeError) { Marshal.load(m) }
    self.class.__send__(:remove_const, :C3)
    eval("C3 = nil")
    assert_raise(TypeError) { Marshal.load(m) }
  ensure
    self.class.__send__(:remove_const, :C3) if self.class.const_defined?(:C3)
  end

  def test_change_struct
    self.class.__send__(:remove_const, :C3) if self.class.const_defined?(:C3)
    eval("C3 = Struct.new(:foo, :bar)")
    m = Marshal.dump(C3.new("FOO", "BAR"))
    self.class.__send__(:remove_const, :C3)
    eval("C3 = Struct.new(:foo)")
    assert_raise(TypeError) { Marshal.load(m) }
    self.class.__send__(:remove_const, :C3)
    eval("C3 = Struct.new(:foo, :baz)")
    assert_raise(TypeError) { Marshal.load(m) }
  ensure
    self.class.__send__(:remove_const, :C3) if self.class.const_defined?(:C3)
  end

  class C4
    def initialize(gc)
      @gc = gc
    end
    def _dump(s)
      GC.start if @gc
      "foo"
    end
  end

  def test_gc
    assert_nothing_raised do
      Marshal.dump((0..1000).map {|x| C4.new(x % 50 == 25) })
    end
  end

  def test_symbol2
    [:ruby, :"\u{7d05}\u{7389}"].each do |sym|
      assert_equal(sym, Marshal.load(Marshal.dump(sym)), '[ruby-core:24788]')
    end
    bug2548 = '[ruby-core:27375]'
    ary = [:$1, nil]
    assert_equal(ary, Marshal.load(Marshal.dump(ary)), bug2548)
  end

  def test_symlink
    assert_include(Marshal.dump([:a, :a]), ';')
  end

  def test_symlink_in_ivar
    bug10991 = '[ruby-core:68587] [Bug #10991]'
    sym = Marshal.load("\x04\x08" +
                       "I" ":\x0bKernel" +
                       ("\x06" +
                        ("I" ":\x07@a" +
                         ("\x06" ":\x07@b" "e;\x0""o:\x0bObject""\x0")) +
                        "0"))
    assert_equal(:Kernel, sym, bug10991)
  end

  ClassUTF8 = eval("class R\u{e9}sum\u{e9}; self; end")

  iso_8859_1 = Encoding::ISO_8859_1

  structISO8859_1 = Struct.new("r\xe9sum\xe9".force_encoding(iso_8859_1).intern)
  const_set("R\xe9sum\xe9".force_encoding(iso_8859_1), structISO8859_1)
  structISO8859_1.name
  StructISO8859_1 = structISO8859_1
  classISO8859_1 = Class.new do
    attr_accessor "r\xe9sum\xe9".force_encoding(iso_8859_1)
    eval("def initialize(x) @r\xe9sum\xe9 = x; end".force_encoding(iso_8859_1))
  end
  const_set("R\xe9sum\xe92".force_encoding(iso_8859_1), classISO8859_1)
  classISO8859_1.name
  ClassISO8859_1 = classISO8859_1

  def test_class_nonascii
    a = ClassUTF8.new
    assert_instance_of(ClassUTF8, Marshal.load(Marshal.dump(a)), '[ruby-core:24790]')

    bug1932 = '[ruby-core:24882]'

    a = StructISO8859_1.new(10)
    assert_nothing_raised(bug1932) do
      assert_equal(a, Marshal.load(Marshal.dump(a)), bug1932)
    end
    a.__send__("#{StructISO8859_1.members[0]}=", a)
    assert_nothing_raised(bug1932) do
      assert_equal(a, Marshal.load(Marshal.dump(a)), bug1932)
    end

    a = ClassISO8859_1.new(10)
    assert_nothing_raised(bug1932) do
      b = Marshal.load(Marshal.dump(a))
      assert_equal(ClassISO8859_1, b.class, bug1932)
      assert_equal(a.instance_variables, b.instance_variables, bug1932)
      a.instance_variables.each do |i|
        assert_equal(a.instance_variable_get(i), b.instance_variable_get(i), bug1932)
      end
    end
    a.__send__(a.methods(true).grep(/=\z/)[0], a)
    assert_nothing_raised(bug1932) do
      b = Marshal.load(Marshal.dump(a))
      assert_equal(ClassISO8859_1, b.class, bug1932)
      assert_equal(a.instance_variables, b.instance_variables, bug1932)
      assert_equal(b, b.instance_variable_get(a.instance_variables[0]), bug1932)
    end
  end

  def test_regexp2
    assert_equal(/\\u/, Marshal.load("\004\b/\b\\\\u\000"))
    assert_equal(/u/, Marshal.load("\004\b/\a\\u\000"))
    assert_equal(/u/, Marshal.load("\004\bI/\a\\u\000\006:\016@encoding\"\vEUC-JP"))

    bug2109 = '[ruby-core:25625]'
    a = "\x82\xa0".force_encoding(Encoding::Windows_31J)
    b = "\x82\xa2".force_encoding(Encoding::Windows_31J)
    c = [/#{a}/, /#{b}/]
    assert_equal(c, Marshal.load(Marshal.dump(c)), bug2109)

    assert_nothing_raised(ArgumentError, '[ruby-dev:40386]') do
      re = Tempfile.create("marshal_regexp") do |f|
        f.binmode.write("\x04\bI/\x00\x00\x06:\rencoding\"\rUS-ASCII")
        f.rewind
        re2 = Marshal.load(f)
        re2
      end
      assert_equal(//, re)
    end
  end

  class DumpTest
    def marshal_dump
      @@block.call(:marshal_dump)
    end

    def dump_each(&block)
      