# frozen_string_literal: false
require 'test/unit'
require 'tmpdir'
require 'timeout'

class TestThreadQueue < Test::Unit::TestCase
  Queue = Thread::Queue
  SizedQueue = Thread::SizedQueue

  def test_queue_initialized
    assert_raise(TypeError) {
      Queue.allocate.push(nil)
    }
  end

  def test_sized_queue_initialized
    assert_raise(TypeError) {
      SizedQueue.allocate.push(nil)
    }
  end

  def test_queue
    grind(5, 1000, 15, Queue)
  end

  def test_sized_queue
    grind(5, 1000, 15, SizedQueue, 1000)
  end

  def grind(num_threads, num_objects, num_iterations, klass, *args)
    from_workers = klass.new(*args)
    to_workers = klass.new(*args)

    workers = (1..num_threads).map {
      Thread.new {
        while object = to_workers.pop
          from_workers.push object
        end
      }
    }

    Thread.new {
      num_iterations.times {
        num_objects.times { to_workers.push 99 }
        num_objects.times { from_workers.pop }
      }
    }.join

    # close the queue the old way to test for backwards-compatibility
    num_threads.times { to_workers.push nil }
    workers.each { |t| t.join }

    assert_equal 0, from_workers.size
    assert_equal 0, to_workers.size
  end

  def test_queue_initialize
    e = Class.new do
      include Enumerable
      def initialize(list) @list = list end
      def each(&block) @list.each(&block) end
    end

    all_assertions_foreach(nil,
                           [Array, "Array"],
                           [e, "Enumerable"],
                           [Struct.new(:to_a), "Array-like"],
                           ) do |a, type|
      q = Thread::Queue.new(a.new([1,2,3]))
      assert_equal(3, q.size, type)
      assert_not_predicate(q, :empty?, type)
      assert_equal(1, q.pop, type)
      assert_equal(2, q.pop, type)
      assert_equal(3, q.pop, type)
      assert_predicate(q, :empty?, type)
    end
  end

  def test_sized_queue_initialize
    q = Thread::SizedQueue.new(1)
    assert_equal 1, q.max
    assert_raise(ArgumentError) { Thread::SizedQueue.new(0) }
    assert_raise(ArgumentError) { Thread::SizedQueue.new(-1) }
  end

  def test_sized_queue_assign_max
    q = Thread::SizedQueue.new(2)
    assert_equal(2, q.max)
    q.max = 1
    assert_equal(1, q.max)
    assert_raise(ArgumentError) { q.max = 0 }
    assert_equal(1, q.max)
    assert_raise(ArgumentError) { q.max = -1 }
    assert_equal(1, q.max)

    before = q.max
    q.max.times { q << 1 }
    t1 = Thread.new { q << 1 }
    sleep 0.01 until t1.stop?
    q.max = q.max + 1
    assert_equal before + 1, q.max
  ensure
    t1.join if t1
  end

  def test_queue_pop_interrupt
    q = Thread::Queue.new
    t1 = Thread.new { q.pop }
    sleep 0.01 until t1.stop?
    t1.kill.join
    assert_equal(0, q.num_waiting)
  end

  def test_queue_pop_non_block
    q = Thread::Queue.new
    assert_raise_with_message(ThreadError, /empty/) do
      q.pop(true)
    end
  end

  def test_sized_queue_pop_interrupt
    q = Thread::SizedQueue.new(1)
    t1 = Thread.new { q.pop }
    sleep 0.01 until t1.stop?
    t1.kill.join
    assert_equal(0, q.num_waiting)
  end

  def test_sized_queue_pop_non_block
    q = Thread::SizedQueue.new(1)
    assert_raise_with_message(ThreadError, /empty/) do
      q.pop(true)
    end
  end

  def test_sized_queue_push_interrupt
    q = Thread::SizedQueue.new(1)
    q.push(1)
    assert_raise_with_message(ThreadError, /full/) do
      q.push(2, true)
    end
  end

  def test_sized_queue_push_non_block
    q = Thread::SizedQueue.new(1)
    q.push(1)
    t1 = Thread.new { q.push(2) }
    sleep 0.01 until t1.stop?
    t1.kill.join
    assert_equal(0, q.num_waiting)
  end

  def test_thr_kill
    bug5343 = '[ruby-core:39634]'
    Dir.mktmpdir {|d|
      timeout = EnvUtil.apply_timeout_scale(60)
      total_count = 250
      begin
        assert_normal_exit(<<-"_eom", bug5343, **{:timeout => timeout, :chdir=>d})
          #{total_count}.times do |i|
            open("test_thr_kill_count", "w") {|f| f.puts i }
            queue = Thread::Queue.new
            r, w = IO.pipe
            th = Thread.start {
              queue.push(nil)
              r.read 1
            }
            queue.pop
            th.kill
            th.join
          end
        _eom
      rescue Timeout::Error
        count = File.read("#{d}/test_thr_kill_count").to_i
        flunk "only #{count}/#{total_count} done in #{timeout} seconds."
      end
    }
  end

  def test_queue_push_return_value
    q = Thread::Queue.new
    retval = q.push(1)
    assert_same q, retval
  end

  def test_queue_clear_return_value
    q = Thread::Queue.new
    retval = q.clear
    assert_same q, retval
  end

  def test_sized_queue_clear
    # Fill queue, then test that Thread::SizedQueue#clear wakes up all waiting threads
    sq = Thread::SizedQueue.new(2)
    2.times { sq << 1 }

    t1 = Thread.new do
      sq << 1
    end

    t2 = Thread.new do
      sq <