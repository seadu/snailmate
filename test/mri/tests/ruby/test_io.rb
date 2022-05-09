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
      assert_nil r.gets
      r.close
    end)
  end

  def test_gets_rs_nil
    pipe(proc do |w|
      w.print "a\n\nb\n\n"
      w.close
    end, proc do |r|
      assert_equal "a\n\nb\n\n", r.gets(nil)
      assert_nil r.gets("")
      r.close
    end)
  end

  def test_gets_rs_377
    pipe(proc do |w|
      w.print "\377xyz"
      w.close
    end, proc do |r|
      r.binmode
      assert_equal("\377", r.gets("\377"), "[ruby-dev:24460]")
      r.close
    end)
  end

  def test_gets_paragraph
    pipe(proc do |w|
      w.print "a\n\nb\n\n"
      w.close
    end, proc do |r|
      assert_equal "a\n\n", r.gets(""), "[ruby-core:03771]"
      assert_equal "b\n\n", r.gets("")
      assert_nil r.gets("")
      r.close
    end)
  end

  def test_gets_chomp_rs
    rs = ":"
    pipe(proc do |w|
      w.print "aaa:bbb"
      w.close
    end, proc do |r|
      assert_equal "aaa", r.gets(rs, chomp: true)
      assert_equal "bbb", r.gets(rs, chomp: true)
      assert_nil r.gets(rs, chomp: true)
      r.close
    end)
  end

  def test_gets_chomp_default_rs
    pipe(proc do |w|
      w.print "aaa\r\nbbb\nccc"
      w.close
    end, proc do |r|
      assert_equal "aaa", r.gets(chomp: true)
      assert_equal "bbb", r.gets(chomp: true)
      assert_equal "ccc", r.gets(chomp: true)
      assert_nil r.gets
      r.close
    end)

    (0..3).each do |i|
      pipe(proc do |w|
        w.write("a" * ((4096 << i) - 4), "\r\n" "a\r\n")
        w.close
      end,
      proc do |r|
        r.gets
        assert_equal "a", r.gets(chomp: true)
        assert_nil r.gets
        r.close
      end)
    end
  end

  def test_gets_chomp_rs_nil
    pipe(proc do |w|
      w.print "a\n\nb\n\n"
      w.close
    end, proc do |r|
      assert_equal "a\n\nb\n", r.gets(nil, chomp: true)
      assert_nil r.gets("")
      r.close
    end)
  end

  def test_gets_chomp_paragraph
    pipe(proc do |w|
      w.print "a\n\nb\n\n"
      w.close
    end, proc do |r|
      assert_equal "a", r.gets("", chomp: true)
      assert_equal "b", r.gets("", chomp: true)
      assert_nil r.gets("", chomp: true)
      r.close
    end)
  end

  def test_gets_limit_extra_arg
    pipe(proc do |w|
      w << "0123456789\n0123456789"
      w.close
    end, proc do |r|
      assert_equal("0123456789\n0", r.gets(nil, 12))
      assert_raise(TypeError) { r.gets(3,nil) }
    end)
  end

  # This test cause SEGV.
  def test_ungetc
    pipe(proc do |w|
      w.close
    end, proc do |r|
      s = "a" * 1000
      assert_raise(IOError, "[ruby-dev:31650]") { 200.times { r.ungetc s } }
    end)
  end

  def test_ungetbyte
    make_tempfile {|t|
      t.open
      t.binmode
      t.ungetbyte(0x41)
      assert_equal(-1, t.pos)
      assert_equal(0x41, t.getbyte)
      t.rewind
      assert_equal(0, t.pos)
      t.ungetbyte("qux")
      assert_equal(-3, t.pos)
      assert_equal("quxfoo\n", t.gets)
      assert_equal(4, t.pos)
      t.set_encoding("utf-8")
      t.ungetbyte(0x89)
      t.ungetbyte(0x8e)
      t.ungetbyte("\xe7")
      t.ungetbyte("\xe7\xb4\x85")
      assert_equal(-2, t.pos)
      assert_equal("\u7d05\u7389bar\n", t.gets)
    }
  end

  def test_each_byte
    pipe(proc do |w|
      w << "abc def"
      w.close
    end, proc do |r|
      r.each_byte {|byte| break if byte == 32 }
      assert_equal("def", r.read, "[ruby-dev:31659]")
    end)
  end

  def test_each_byte_with_seek
    make_tempfile {|t|
      bug5119 = '[ruby-core:38609]'
      i = 0
      open(t.path) do |f|
        f.each_byte {i = f.pos}
      end
      assert_equal(12, i, bug5119)
    }
  end

  def test_each_byte_closed
    pipe(proc do |w|
      w << "abc def"
      w.close
    end, proc do |r|
      assert_raise(IOError) do
        r.each_byte {|byte| r.close if byte == 32 }
      end
    end)
    make_tempfile {|t|
      File.open(t, 'rt') {|f|
        assert_raise(IOError) do
          f.each_byte {|c| f.close if c == 10}
        end
      }
    }
  end

  def test_each_codepoint
    make_tempfile {|t|
      bug2959 = '[ruby-core:28650]'
      a = ""
      File.open(t, 'rt') {|f|
        f.each_codepoint {|c| a << c}
      }
      assert_equal("foo\nbar\nbaz\n", a, bug2959)
    }
  end

  def test_each_codepoint_closed
    pipe(proc do |w|
      w.print("abc def")
      w.close
    end, proc do |r|
      assert_raise(IOError) do
        r.each_codepoint {|c| r.close if c == 32}
      end
    end)
    make_tempfile {|t|
      File.open(t, 'rt') {|f|
        assert_raise(IOError) do
          f.each_codepoint {|c| f.close if c == 10}
        end
      }
    }
  end

  def test_rubydev33072
    t = make_tempfile
    path = t.path
    t.close!
    assert_raise(Errno::ENOENT, "[ruby-dev:33072]") do
      File.read(path, nil, nil, **{})
    end
  end

  def with_srccontent(content = "baz")
    src = "src"
    mkcdtmpdir {
      File.open(src, "w") {|f| f << content }
      yield src, content
    }
  end

  def test_copy_stream_small
    with_srccontent("foobar") {|src, content|
      ret = IO.copy_stream(src, "dst")
      assert_equal(content.bytesize, ret)
      assert_equal(content, File.read("dst"))
    }
  end

  def test_copy_stream_append
    with_srccontent("foobar") {|src, content|
      File.open('dst', 'ab') do |dst|
        ret = IO.copy_stream(src, dst)
        assert_equal(content.bytesize, ret)
        assert_equal(content, File.read("dst"))
      end
    }
  end

  def test_copy_stream_append_to_nonempty
    with_srccontent("foobar") {|src, content|
      preface = 'preface'
      File.write('dst', preface)
      File.open('dst', 'ab') do |dst|
        ret = IO.copy_stream(src, dst)
        assert_equal(content.bytesize, ret)
        assert_equal(preface + content, File.read("dst"))
      end
    }
  end

  def test_copy_stream_smaller
    with_srccontent {|src, content|

      # overwrite by smaller file.
      dst = "dst"
      File.open(dst, "w") {|f| f << "foobar"}

      ret = IO.copy_stream(src, dst)
      assert_equal(content.bytesize, ret)
      assert_equal(content, File.read(dst))

      ret = IO.copy_stream(src, dst, 2)
      assert_equal(2, ret)
      assert_equal(content[0,2], File.read(dst))

      ret = IO.copy_stream(src, dst, 0)
      assert_equal(0, ret)
      assert_equal("", File.read(dst))

      ret = IO.copy_stream(src, dst, nil, 1)
      assert_equal(content.bytesize-1, ret)
      assert_equal(content[1..-1], File.read(dst))
    }
  end

  def test_copy_stream_noent
    with_srccontent {|src, content|
      assert_raise(Errno::ENOENT) {
        IO.copy_stream("nodir/foo", "dst")
      }

      assert_raise(Errno::ENOENT) {
        IO.