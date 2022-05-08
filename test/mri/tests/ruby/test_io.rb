# coding: US-ASCII
# frozen_string_literal: false
require 'test/unit'
require 'tmpdir'
require "fcntl"
require 'io/nonblock'
require 'pathname'
require 'socket'
require 'stringio'
require 'timeout'
require 'tempfile'
require 'weakref'

class TestIO < Test::Unit::TestCase
  module Feature
    def have_close_on_exec?
      $stdin.close_on_exec?
      true
    rescue NotImplementedError
      false
    end

    def have_nonblock?
      IO.method_defined?("nonblock=")
    end
  end

  include Feature
  extend Feature

  def pipe(wp, rp)
    re, we = nil, nil
    r, w = IO.pipe
    rt = Thread.new do
      begin
        rp.call(r)
      rescue Exception
        r.close
        re = $!
      end
    end
    wt = Thread.new do
      begin
        wp.call(w)
      rescue Exception
        w.close
        we = $!
      end
    end
    flunk("timeout") unless wt.join(10) && rt.join(10)
  ensure
    w&.close
    r&.close
    (wt.kill; wt.join) if wt
    (rt.kill; rt.join) if rt
    raise we if we
    raise re if re
  end

  def with_pipe
    r, w = IO.pipe
    begin
      yield r, w
    ensure
      r.close
      w.close
    end
  end

  def with_read_pipe(content)
    pipe(proc do |w|
      w << content
      w.close
    end, proc do |r|
      yield r
    end)
  end

  def mkcdtmpdir
    Dir.mktmpdir {|d|
      Dir.chdir(d) {
        yield
      }
    }
  end

  def trapping_usr2
    @usr2_rcvd  = 0
    r, w = IO.pipe
    trap(:USR2) do
      w.write([@usr2_rcvd += 1].pack('L'))
    end
    yield r
  ensure
    trap(:USR2, "DEFAULT")
    w&.close
    r&.close
  end

  def test_pipe
    r, w = IO.pipe
    assert_instance_of(IO, r)
    assert_instance_of(IO, w)
    [
      Thread.start{
        w.print "abc"
        w.close
      },
      Thread.start{
        assert_equal("abc", r.read)
        r.close
      }
    ].each{|thr| thr.join}
  end

  def test_binmode_pipe
    EnvUtil.with_default_internal(Encoding::UTF_8) do
      EnvUtil.with_default_external(Encoding::UTF_8) do
        begin
          reader0, writer0 = IO.pipe
          reader0.binmode
          writer0.binmode

          reader1, writer1 = IO.pipe

          reader2, writer2 = IO.pipe(binmode: true)
          assert_predicate writer0, :binmode?
          assert_predicate writer2, :binmode?
          assert_equal writer0.binmode?, writer2.binmode?
          assert_equal writer0.external_encoding, writer2.external_encoding
          assert_equal writer0.internal_encoding, writer2.internal_encoding
          assert_predicate reader0, :binmode?
          assert_predicate reader2, :binmode?
          assert_equal reader0.binmode?, reader2.binmode?
          assert_equal reader0.external_encoding, reader2.external_encoding
          assert_equal reader0.internal_encoding, reader2.internal_encoding

          reader3, writer3 = IO.pipe("UTF-8:UTF-8", binmode: true)
          assert_predicate writer3, :binmode?
          assert_equal writer1.external_encoding, writer3.external_encoding
          assert_equal writer1.internal_encoding, writer3.internal_encoding
          assert_predicate reader3, :binmode?
          assert_equal reader1.external_encoding, reader3.external_encoding
          assert_equal reader1.internal_encoding, reader3.internal_encoding

          reader4, writer4 = IO.pipe("UTF-8:UTF-8", binmode: true)
          assert_predicate writer4, :binmode?
          assert_equal writer1.external_encoding, writer4.external_encoding
          assert_equal writer1.internal_encoding, writer4.internal_encoding
          assert_predicate reader4, :binmode?
          assert_equal reader1.external_encoding, reader4.external_encoding
          assert_equal reader1.internal_encoding, reader4.internal_encoding

          reader5, writer5 = IO.pipe("UTF-8", "UTF-8", binmode: true)
          assert_predicate writer5, :binmode?
          assert_equal writer1.external_encoding, writer5.external_encoding
          assert_equal writer1.internal_encoding, writer5.internal_encoding
          assert_predicate reader5, :binmode?
          assert_equal reader1.external_encoding, reader5.external_encoding
          assert_equal reader1.internal_encoding, reader5.internal_encoding
        ensure
          [
            reader0, writer0,
            reader1, writer1,
            reader2, writer2,
            reader3, writer3,
            reader4, writer4,
            reader5, writer5,
          ].compact.map(&:close)
        end
      end
    end
  end

  def test_pipe_block
    x = nil
    ret = IO.pipe {|r, w|
      x = [r,w]
      assert_instance_of(IO, r)
      assert_instance_of(IO, w)
      [
        Thread.start do
          w.print "abc"
          w.close
        end,
        Thread.start do
          assert_equal("abc", r.read)
        end
      ].each{|thr| thr.join}
      assert_not_predicate(r, :closed?)
      assert_predicate(w, :closed?)
      :foooo
    }
    assert_equal(:foooo, ret)
    assert_predicate(x[0], :closed?)
    assert_predicate(x[1], :closed?)
  end

  def test_pipe_block_close
    4.times {|i|
      x = nil
      IO.pipe {|r, w|
        x = [r,w]
        r.close if (i&1) == 0
        w.close if (i&2) == 0
      }
      assert_predicate(x[0], :closed?)
      assert_predicate(x[1], :closed?)
    }
  end

  def test_gets_rs
    rs = ":"
    pipe(proc do |w|
      w.print "aaa:bbb"
      w.close
    end, proc do |r|
      assert_equal "aaa:", r.gets(rs)
      assert_equal "bbb", r.gets(rs)
      assert_nil r.gets(rs)
      r.close
    end)
  end

  def test_gets_default_rs
    pipe(proc do |w|
      w.print "aaa\nbbb\n"
      w.close
    end, proc do |r|
      assert_equal "aaa\n", r.gets
      assert_equal "bbb\n", r.gets
      assert_n