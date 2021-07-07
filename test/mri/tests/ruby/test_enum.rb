# frozen_string_literal: false
require 'test/unit'
EnvUtil.suppress_warning {require 'continuation'}
require 'stringio'
require 'delegate'

class TestEnumerable < Test::Unit::TestCase
  def setup
    @obj = Object.new
    class << @obj
      include Enumerable
      def each
        yield 1
        yield 2
        yield 3
        yield 1
        yield 2
        self
      end
    end
    @empty = Object.new
    class << @empty
      attr_reader :block
      include Enumerable
      def each(&block)
        @block = block
        self
      end
    end
    @verbose = $VERBOSE
  end

  def teardown
    $VERBOSE = @verbose
  end

  def test_grep_v
    assert_equal([3], @obj.grep_v(1..2))
    a = []
    @obj.grep_v(2) {|x| a << x }
    assert_equal([1, 3, 1], a)

    a = []
    lambda = ->(x, i) {a << [x, i]}
    @obj.each_with_index.grep_v(proc{|x,i|x!=2}, &lambda)
    assert_equal([[2, 1], [2, 4]], a)
  end

  def test_grep
    assert_equal([1, 2, 1, 2], @obj.grep(1..2))
    a = []
    @obj.grep(2) {|x| a << x }
    assert_equal([2, 2], a)

    bug5801 = '[ruby-dev:45041]'
    @empty.grep(//)
    block = @empty.block
    assert_nothing_raised(bug5801) {100.times {block.call}}

    a = []
    lambda = ->(x, i) {a << [x, i]}
    @obj.each_with_index.grep(proc{|x,i|x==2}, &lambda)
    assert_equal([[2, 1], [2, 4]], a)
  end

  def test_grep_optimization
    bug17030 = '[ruby-core:99156]'
    'set last match' =~ /set last (.*)/
    assert_equal([:a, 'b', :c], [:a, 'b', 'z', :c, 42, nil].grep(/[a-d]/), bug17030)
    assert_equal(['z', 42, nil], [:a, 'b', 'z', :c, 42, nil].grep_v(/[a-d]/), bug17030)
    assert_equal('match', $1, bug17030)

    regexp = Regexp.new('x')
    assert_equal([], @obj.grep(regexp), bug17030) # sanity check
    def regexp.===(other)
      true
    end
    assert_equal([1, 2, 3, 1, 2], @obj.grep(regexp), bug17030)

    o = Object.new
    def o.to_str
      'hello'
    end
    assert_same(o, [o].grep(/ll/).first, bug17030)
  end

  def test_count
    assert_equal(5, @obj.count)
    assert_equal(2, @obj.count(1))
    assert_equal(3, @obj.count {|x| x % 2 == 1 })
    assert_equal(2, assert_warning(/given block not used/) {@obj.count(1) {|x| x % 2 == 1 }})
    assert_raise(ArgumentError) { @obj.count(0, 1) }

    if RUBY_ENGINE == "ruby"
      en = Class.new {
        include Enumerable
        alias :size :count
        def each
          yield 1
        end
      }
      assert_equal(1, en.new.count, '[ruby-core:24794]')
    end
  end

  def test_find
    assert_equal(2, @obj.find {|x| x % 2 == 0 })
    assert_equal(nil, @obj.find {|x| false })
    assert_equal(:foo, @obj.find(proc { :foo }) {|x| false })
    cond = ->(x, i) { x % 2 == 0 }
    assert_equal([2, 1], @obj.each_with_index.find(&cond))
  end

  def test_find_index
    assert_equal(1, @obj.find_index(2))
    assert_equal(1, @obj.find_index {|x| x % 2 == 0 })
    assert_equal(nil, @obj.find_index {|x| false })
    assert_raise(ArgumentError) { @obj.find_index(0, 1) }
    assert_equal(1, assert_warning(/given block not used/) {@obj.find_index(2) {|x| x == 1 }})
  end

  def test_find_all
    assert_equal([1, 3, 1], @obj.find_all {|x| x % 2 == 1 })
    cond = ->(x, i) { x % 2 == 1 }
    assert_equal([[1, 0], [3, 2], [1, 3]], @obj.each_with_index.find_all(&cond))
  end

  def test_reject
    assert_equal([2, 3, 2], @obj.reject {|x| x < 2 })
    cond = ->(x, i) {x < 2}
    assert_equal([[2, 1], [3, 2], [2, 4]], @obj.each_with_index.reject(&cond))
  end

  def test_to_a
    assert_equal([1, 2, 3, 1, 2], @obj.to_a)
  end

  def test_to_a_keywords
    @obj.singleton_class.remove_method(:each)
    def @obj.each(foo:) yield foo end
    assert_equal([1], @obj.to_a(foo: 1))
  end

  def test_to_a_size_symbol
    sym = Object.new
    class << sym
      include Enumerable
      def each
        self
      end

      def size
        :size
      end
    end
    assert_equal([], sym.to_a)
  end

  def test_to_a_size_infinity
    inf = Object.new
    class << inf
      include Enumerable
      def each
        self
      end

      def size
        Float::INFINITY
      end
    end
    assert_equal([], inf.to_a)
  end

  StubToH = Object.new.tap do |obj|
    def obj.each(*args)
      yield(*args)
      yield [:key, :value]
      yield :other_key, :other_value
      kvp = Object.new
      def kvp.to_ary
        [:obtained, :via_to_ary]
      end
      yield kvp
    end
    obj.extend Enumerable
    obj.freeze
  end

  def test_to_h
    obj = StubToH

    assert_equal({
      :hello => :world,
      :key => :value,
      :other_key => :other_value,
      :obtained => :via_to_ary,
    }, obj.to_h(:hello, :world))

    e = assert_raise(TypeError) {
      obj.to_h(:not_an_array)
    }
    assert_equal "wrong element type Symbol (expected array)", e.message

    e = assert_raise(ArgumentError) {
      obj.to_h([1])
    }
    assert_equal "element has wrong array length (expected 2, was 1)", e.message
  end

  def test_to_h_block
    obj = StubToH

    assert_equal({
      "hello" => "world",
      "key" => "value",
      "other_key" => "other_value",
      "obtained" => "via_to_ary",
    }, obj.to_h(:hello, :world) {|k, v| [k.to_s, v.to_s]})

    e = assert_raise(TypeError) {
      obj.to_h {:not_an_array}
    }
    assert_equal "wrong element type Symbol (expected array)", e.message

    e = assert_raise(ArgumentError) {
      obj.to_h {[1]}
    }
    assert_equal "element has wrong array length (expected 2, was 1)", e.message
  end

  def test_inject
    assert_equal(12, @obj.inject {|z, x| z * x })
    assert_equal(48, @obj.inject {|z, x| z * 2 + x })
    assert_equal(12, @obj.inject(:*))
    assert_equal(24, @obj.inject(2) {|z, x| z * x })
    assert_equal(24, assert_warning(/given block not used/) {@obj.inject(2, :*) {|z, x| z * x }})
    assert_equal(nil, @empty.inject() {9})
  end

  FIXNUM_MIN = RbConfig::LIMITS['FIXNUM_MIN']
  FIXNUM_MAX = RbConfig::LIMITS['FIXNUM_MAX']

  def test_inject_array_mul
    assert_equal(nil, [].inject(:*))
    assert_equal(5, [5].inject(:*))
    assert_equal(35, [5, 7].inject(:*))
    assert_equal(3, [].inject(3, :*))
    assert_equal(15, [5].inject(3, :*))
    assert_equal(105, [5, 7].inject(3, :*))
  end

  def test_inject_array_plus
    assert_equal(3, [3].inject(:+))
    assert_equal(8, [3, 5].inject(:+))
    assert_equal(15, [3, 5, 7].inject(:+))
    assert_float_equal(15.0, [3, 5, 7.0].inject(:+))
    assert_equal(2*FIXNUM_MAX, Array.new(2, FIXNUM_MAX).inject(:+))
    assert_equal(3*FIXNUM_MAX, Array.new(3, FIXNUM_MAX).inject(:+))
    assert_equal(2*(FIXNUM_MAX+1), Array.new(2, FIXNUM_MAX+1).inject(:+))
    assert_equal(10*FIXNUM_MAX, Array.new(10, FIXNUM_MAX).inject(:+))
    assert_equal(0, ([FIXNUM_MAX, 1, -FIXNUM_MAX, -1]*10).inject(:+))
