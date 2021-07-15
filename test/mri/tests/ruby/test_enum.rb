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
    assert_equal(FIXNUM_MAX*10, ([FIXNUM_MAX+1, -1]*10).inject(:+))
    assert_equal(2*FIXNUM_MIN, Array.new(2, FIXNUM_MIN).inject(:+))
    assert_equal(3*FIXNUM_MIN, Array.new(3, FIXNUM_MIN).inject(:+))
    assert_equal((FIXNUM_MAX+1).to_f, [FIXNUM_MAX, 1, 0.0].inject(:+))
    assert_float_equal(10.0, [3.0, 5].inject(2.0, :+))
    assert_float_equal((FIXNUM_MAX+1).to_f, [0.0, FIXNUM_MAX+1].inject(:+))
    assert_equal(2.0+3.0i, [2.0, 3.0i].inject(:+))
  end

  def test_inject_op_redefined
    assert_separately([], "#{<<~"end;"}\n""end")
    k = Class.new do
      include Enumerable
      def each
        yield 1
        yield 2
        yield 3
      end
    end
    all_assertions_foreach("", *%i[+ * / - %]) do |op|
      bug = '[ruby-dev:49510] [Bug#12178] should respect redefinition'
      begin
        Integer.class_eval do
          alias_method :orig, op
          define_method(op) do |x|
            0
          end
        end
        assert_equal(0, k.new.inject(op), bug)
      ensure
        Integer.class_eval do
          undef_method op
          alias_method op, :orig
        end
      end
    end;
  end

  def test_inject_op_private
    assert_separately([], "#{<<~"end;"}\n""end")
    k = Class.new do
      include Enumerable
      def each
        yield 1
        yield 2
        yield 3
      end
    end
    all_assertions_foreach("", *%i[+ * / - %]) do |op|
      bug = '[ruby-core:81349] [Bug #13592] should respect visibility'
      assert_raise_with_message(NoMethodError, /private method/, bug) do
        begin
          Integer.class_eval do
            private op
          end
          k.new.inject(op)
        ensure
          Integer.class_eval do
            public op
          end
        end
      end
    end;
  end

  def test_inject_array_op_redefined
    assert_separately([], "#{<<~"end;"}\n""end")
    all_assertions_foreach("", *%i[+ * / - %]) do |op|
      bug = '[ruby-dev:49510] [Bug#12178] should respect redefinition'
      begin
        Integer.class_eval do
          alias_method :orig, op
          define_method(op) do |x|
            0
          end
        end
        assert_equal(0, [1,2,3].inject(op), bug)
      ensure
        Integer.class_eval do
          undef_method op
          alias_method op, :orig
        end
      end
    end;
  end

  def test_inject_array_op_private
    assert_separately([], "#{<<~"end;"}\n""end")
    all_assertions_foreach("", *%i[+ * / - %]) do |op|
      bug = '[ruby-core:81349] [Bug #13592] should respect visibility'
      assert_raise_with_message(NoMethodError, /private method/, bug) do
        begin
          Integer.class_eval do
            private op
          end
          [1,2,3].inject(op)
        ensure
          Integer.class_eval do
            public op
          end
        end
      end
    end;
  end

  def test_refine_Enumerable_then_include
    assert_separately([], "#{<<~"end;"}\n")
      module RefinementBug
        refine Enumerable do
          def refined_method
            :rm
          end
        end
      end
      using RefinementBug

      class A
        include Enumerable
      end

      assert_equal(:rm, [].refined_method)
    end;
  end

  def test_partition
    assert_equal([[1, 3, 1], [2, 2]], @obj.partition {|x| x % 2 == 1 })
    cond = ->(x, i) { x % 2 == 1 }
    assert_equal([[[1, 0], [3, 2], [1, 3]], [[2, 1], [2, 4]]], @obj.each_with_index.partition(&cond))
  end

  def test_group_by
    h = { 1 => [1, 1], 2 => [2, 2], 3 => [3] }
    assert_equal(h, @obj.group_by {|x| x })

    h = {1=>[[1, 0], [1, 3]], 2=>[[2, 1], [2, 4]], 3=>[[3, 2]]}
    cond = ->(x, i) { x }
    assert_equal(h, @obj.each_with_index.group_by(&cond))
  end

  def test_tally
    h = {1 => 2, 2 => 2, 3 => 1}
    assert_equal(h, @obj.tally)

    h = {1 => 5, 2 => 2, 3 => 1, 4 => "x"}
    assert_equal(h, @obj.tally({1 => 3, 4 => "x"}))

    assert_raise(TypeError) do
      @obj.tally({1 => ""})
    end

    h = {1 => 2, 2 => 2, 3 => 1}
    assert_same(h, @obj.tally(h))

    h = {1 => 2, 2 => 2, 3 => 1}.freeze
    assert_raise(FrozenError) do
      @obj.tally(h)
    end
    assert_equal({1 => 2, 2 => 2, 3 => 1}, h)

    hash_convertible = Object.new
    def hash_convertible.to_hash
      {1 => 3, 4 => "x"}
    end
    assert_equal({1 => 5, 2 => 2, 3 => 1, 4 => "x"}, @obj.tally(hash_convertible))

    hash_convertible = Object.new
    def hash_convertible.to_hash
      {1 => 3, 4 => "x"}.freeze
    end
    assert_raise(FrozenError) do
      @obj.tally(hash_convertible)
    end
    assert_equal({1 => 3, 4 => "x"}, hash_convertible.to_hash)

    assert_raise(TypeError) do
      @obj.tally(BasicObject.new)
    end

    h = {1 => 2, 2 => 2, 3 => 1}
    assert_equal(h, @obj.tally(Hash.new(100)))
    assert_equal(h, @obj.tally(Hash.new {100}))
  end

  def test_first
    assert_equal(1, @obj.first)
    assert_equal([1, 2, 3], @obj.first(3))
    assert_nil(@empty.first)
    assert_equal([], @empty.first(10))

    bug5801 = '[ruby-dev:45041]'
    assert_in_out_err([], <<-'end;', [], /unexpected break/, bug5801)
      empty = Object.new
      class << empty
        attr_reader :block
        include Enumerable
        def each(&block)
          @block = block
          self
        end
      end
      empty.first
      empty.block.call
    end;

    bug18475 = '[ruby-dev:107059]'
    assert_in_out_err([], <<-'end;', [], /unexpected break/, bug18475)
      e = Enumerator.new do |g|
        Thread.new do
          g << 1
        end.join
      end

      e.first
    end;
  end

  def test_sort
    assert_equal([1, 1, 2, 2, 3], @obj.sort)
    assert_equal([3, 2, 2, 1, 1], @obj.sort {|x, y| y <=> x })
  end

  def test_sort_by
    assert_equal([3, 2, 2, 1, 1], @obj.sort_by {|x| -x })
    assert_equal((1..300).to_a.reverse, (1..300).sort_by {|x| -x })

    cond = ->(x, i) { [-x, i] }
    assert_equal([[3, 2], [2, 1], [2, 4], [1, 0], [1, 3]], @obj.each_with_index.sort_by(&cond))
  end

  def test_all
    assert_equal(true, @obj.all? {|x| x <= 3 })
    assert_equal(false, @obj.all? {|x| x < 3 })
    assert_equal(true, @obj.all?)
    assert_equal(false, [true, true, false].all?)
    assert_equal(true, [].all?)
    assert_equal(true, @empty.all?)
    assert_equal(true, @obj.all?(Integer))
    assert_equal(false, @obj.all?(1..2))
  end

  def test_all_with_unused_block
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      [1, 2].all?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      (1..2).all?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      3.times.all?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      {a: 1, b: 2}.all?([:b, 2]) {|x| x == 4 }
    EOS
  end

  def test_any
    assert_equal(true, @obj.any? {|x| x >= 3 })
    assert_equal(false, @obj.any? {|x| x > 3 })
    assert_equal(true, @obj.any?)
    assert_equal(false, [false, false, false].any?)
    assert_equal(false, [].any?)
    assert_equal(false, @empty.any?)
    assert_equal(true, @obj.any?(1..2))
    assert_equal(false, @obj.any?(Float))
    assert_equal(false, [1, 42].any?(Float))
    assert_equal(true, [1, 4.2].any?(Float))
    assert_equal(false, {a: 1, b: 2}.any?(->(kv) { kv == [:foo, 42] }))
    assert_equal(true, {a: 1, b: 2}.any?(->(kv) { kv == [:b, 2] }))
  end

  def test_any_with_unused_block
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      [1, 23].any?(1) {|x| x == 1 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      (1..2).any?(34) {|x| x == 2 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      3.times.any?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      {a: 1, b: 2}.any?([:b, 2]) {|x| x == 4 }
    EOS
  end

  def test_one
    assert(@obj.one? {|x| x == 3 })
    assert(!(@obj.one? {|x| x == 1 }))
    assert(!(@obj.one? {|x| x == 4 }))
    assert(@obj.one?(3..4))
    assert(!(@obj.one?(1..2)))
    assert(!(@obj.one?(4..5)))
    assert(%w{ant bear cat}.one? {|word| word.length == 4})
    assert(!(%w{ant bear cat}.one? {|word| word.length > 4}))
    assert(!(%w{ant bear cat}.one? {|word| word.length < 4}))
    assert(%w{ant bear cat}.one?(/b/))
    assert(!(%w{ant bear cat}.one?(/t/)))
    assert(!([ nil, true, 99 ].one?))
    assert([ nil, true, false ].one?)
    assert(![].one?)
    assert(!@empty.one?)
    assert([ nil, true, 99 ].one?(Integer))
  end

  def test_one_with_unused_block
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      [1, 2].one?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      (1..2).one?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      3.times.one?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      {a: 1, b: 2}.one?([:b, 2]) {|x| x == 4 }
    EOS
  end

  def test_none
    assert(@obj.none? {|x| x == 4 })
    assert(!(@obj.none? {|x| x == 1 }))
    assert(!(@obj.none? {|x| x == 3 }))
    assert(@obj.none?(4..5))
    assert(!(@obj.none?(1..3)))
    assert(%w{ant bear cat}.none? {|word| word.length == 5})
    assert(!(%w{ant bear cat}.none? {|word| word.length >= 4}))
    assert(%w{ant bear cat}.none?(/d/))
    assert(!(%w{ant bear cat}.none?(/b/)))
    assert([].none?)
    assert([nil].none?)
    assert([nil,false].none?)
    assert(![nil,false,true].none?)
    assert(@empty.none?)
  end

  def test_none_with_unused_block
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      [1, 2].none?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      (1..2).none?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      3.times.none?(1) {|x| x == 3 }
    EOS
    assert_in_out_err [], <<-EOS, [], ["-:1: warning: given block not used"]
      {a: 1, b: 2}.none?([:b, 2]) {|x| x == 4 }
    EOS
  end

  def test_min
    assert_equal(1, @obj.min)
    assert_equal(3, @obj.min {|a,b| b <=> a })
    cond = ->((a, ia), (b, ib)) { (b <=> a).nonzero? or ia <=> ib }
    assert_equal([3, 2], @obj.each_with_index.min(&cond))
    enum = %w(albatross dog horse).to_enum
    assert_equal("albatross", enum.min)
    assert_equal("dog", enum.min {|a,b| a.length <=> b.length })
    assert_equal(1, [3,2,1].to_enum.min)
    assert_equal(%w[albatross dog], enum.min(2))
    assert_equal(%w[dog horse],
                 enum.min(2) {|a,b| a.length <=> b.length })
    assert_equal([13, 14], [20, 32, 32, 21, 30, 25, 29, 13, 14].to_enum.min(2))
    assert_equal([2, 4, 6, 7], [2, 4, 8, 6, 7].to_enum.min(4))
  end

  def test_max
    assert_equal(3, @obj.max)
    assert_equal(1, @obj.max {|a,b| b <=> a })
    cond = ->((a, ia), (b, ib)) { (b <=> a).nonzero? or ia <=> ib }
    assert_equal([1, 3], @obj.each_with_index.max(&cond))
    enum = %w(albatross dog horse).to_enum
    assert_equal("horse", enum.max)
    assert_equal("albatross", enum.max {|a,b| a.length <=> b.length })
    assert_equal(1, [3,2,1].to_enum.max{|a,b| b <=> a })
    assert_equal(%w[horse dog], enum.max(2))
    assert_equal(%w[albatross horse],
                 enum.max(2) {|a,b| a.length <=> b.length })
    assert_equal([3, 2], [0, 0, 0, 0, 0, 0, 1, 3, 2].to_enum.max(2))
  end

  def test_minmax
    assert_equal([1, 3], @obj.minmax)
    assert_equal([3, 1], @obj.minmax {|a,b| b <=> a })
    ary = %w(albatross dog horse)
    assert_equal(["albatross", "horse"], ary.minmax)
    assert_equal(["dog", "albatross"], ary.minmax {|a,b| a.length <=> b.length })
    assert_equal([1, 3], [2,3,1].minmax)
    assert_equal([3, 1], [2,3,1].minmax {|a,b| b <=> a })
    assert_equal([1, 3], [2,2,3,3,1,1].minmax)
    assert_equal([nil, nil], [].minmax)
  end

  def test_min_by
    assert_equal(3, @obj.min_by {|x| -x })
    cond = ->(x, i) { -x }
    assert_equal([3, 2], @obj.each_with_index.min_by(&cond))
    a = %w(albatross dog horse)
    assert_equal("dog", a.min_by {|x| x.length })
    assert_equal(3, [2,3,1].min_by {|x| -x })
    assert_equal(%w[dog horse], a.min_by(2) {|x| x.length })
    assert_equal([13, 14], [20, 32, 32, 21, 30, 25, 29, 13, 14].min_by(2) {|x| x})
  end

  def test_max_by
    assert_equal(1, @obj.max_by {|x| -x })
    cond = ->(x, i) { -x }
    assert_equal([1, 0], @obj.each_with_index.max_by(&cond))
    a = %w(albatross dog horse)
    assert_equal("albatross", a.max_by {|x| x.length })
    assert_equal(1, [2,3,1].max_by {|x| -x })
    assert_equal(%w[albatross horse], a.max_by(2) {|x| x.length })
    assert_equal([3, 2], [0, 0, 0, 0, 0, 0, 1, 3, 2].max_by(2) {|x| x})
  end

  def test_minmax_by
    assert_equal([3, 1], @obj.minmax_by {|x| -x })
    cond = ->(x, i) { -x }
    assert_equal([[3, 2], [1, 0]], @obj.each_with_index.minmax_by(&cond))
    a = %w(albatross dog horse)
    assert_equal(["dog", "albatross"], a.minmax_by {|x| x.length })
    assert_equal([3, 1], [2,3,1].minmax_by {|x| -x })
  end

  def test_member
    assert(@obj.member?(1))
    assert(!(@obj.member?(4)))
    assert([1,2,3].member?(1))
    assert(!(