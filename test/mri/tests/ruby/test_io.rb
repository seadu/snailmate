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
        IO.copy_stream(src, "nodir/bar")
      }
    }
  end

  def test_copy_stream_pipe
    with_srccontent {|src, content|
      pipe(proc do |w|
        ret = IO.copy_stream(src, w)
        assert_equal(content.bytesize, ret)
        w.close
      end, proc do |r|
        assert_equal(content, r.read)
      end)
    }
  end

  def test_copy_stream_write_pipe
    with_srccontent {|src, content|
      with_pipe {|r, w|
        w.close
        assert_raise(IOError) { IO.copy_stream(src, w) }
      }
    }
  end

  def with_pipecontent
    mkcdtmpdir {
      yield "abc"
    }
  end

  def test_copy_stream_pipe_to_file
    with_pipecontent {|pipe_content|
      dst = "dst"
      with_read_pipe(pipe_content) {|r|
        ret = IO.copy_stream(r, dst)
        assert_equal(pipe_content.bytesize, ret)
        assert_equal(pipe_content, File.read(dst))
      }
    }
  end

  def test_copy_stream_read_pipe
    with_pipecontent {|pipe_content|
      with_read_pipe(pipe_content) {|r1|
        assert_equal("a", r1.getc)
        pipe(proc do |w2|
          w2.sync = false
          w2 << "def"
          ret = IO.copy_stream(r1, w2)
          assert_equal(2, ret)
          w2.close
        end, proc do |r2|
          assert_equal("defbc", r2.read)
        end)
      }

      with_read_pipe(pipe_content) {|r1|
        assert_equal("a", r1.getc)
        pipe(proc do |w2|
          w2.sync = false
          w2 << "def"
          ret = IO.copy_stream(r1, w2, 1)
          assert_equal(1, ret)
          w2.close
        end, proc do |r2|
          assert_equal("defb", r2.read)
        end)
      }

      with_read_pipe(pipe_content) {|r1|
        assert_equal("a", r1.getc)
        pipe(proc do |w2|
          ret = IO.copy_stream(r1, w2)
          assert_equal(2, ret)
          w2.close
        end, proc do |r2|
          assert_equal("bc", r2.read)
        end)
      }

      with_read_pipe(pipe_content) {|r1|
        assert_equal("a", r1.getc)
        pipe(proc do |w2|
          ret = IO.copy_stream(r1, w2, 1)
          assert_equal(1, ret)
          w2.close
        end, proc do |r2|
          assert_equal("b", r2.read)
        end)
      }

      with_read_pipe(pipe_content) {|r1|
        assert_equal("a", r1.getc)
        pipe(proc do |w2|
          ret = IO.copy_stream(r1, w2, 0)
          assert_equal(0, ret)
          w2.close
        end, proc do |r2|
          assert_equal("", r2.read)
        end)
      }

      pipe(proc do |w1|
        w1 << "abc"
        w1 << "def"
        w1.close
      end, proc do |r1|
        assert_equal("a", r1.getc)
        pipe(proc do |w2|
          ret = IO.copy_stream(r1, w2)
          assert_equal(5, ret)
          w2.close
        end, proc do |r2|
          assert_equal("bcdef", r2.read)
        end)
      end)
    }
  end

  def test_copy_stream_file_to_pipe
    with_srccontent {|src, content|
      pipe(proc do |w|
        ret = IO.copy_stream(src, w, 1, 1)
        assert_equal(1, ret)
        w.close
      end, proc do |r|
        assert_equal(content[1,1], r.read)
      end)
    }
  end

  if have_nonblock?
    def test_copy_stream_no_busy_wait
      skip "MJIT has busy wait on GC. This sometimes fails with --jit." if defined?(RubyVM::MJIT) && RubyVM::MJIT.enabled?
      skip "multiple threads already active" if Thread.list.size > 1

      msg = 'r58534 [ruby-core:80969] [Backport #13533]'
      IO.pipe do |r,w|
        r.nonblock = true
        assert_cpu_usage_low(msg, stop: ->{w.close}) do
          IO.copy_stream(r, IO::NULL)
        end
      end
    end

    def test_copy_stream_pipe_nonblock
      mkcdtmpdir {
        with_read_pipe("abc") {|r1|
          assert_equal("a", r1.getc)
          with_pipe {|r2, w2|
            begin
              w2.nonblock = true
            rescue Errno::EBADF
              skip "nonblocking IO for pipe is not implemented"
            end
            s = w2.syswrite("a" * 100000)
            t = Thread.new { sleep 0.1; r2.read }
            ret = IO.copy_stream(r1, w2)
            w2.close
            assert_equal(2, ret)
            assert_equal("a" * s + "bc", t.value)
          }
        }
      }
    end
  end

  def with_bigcontent
    yield "abc" * 123456
  end

  def with_bigsrc
    mkcdtmpdir {
      with_bigcontent {|bigcontent|
        bigsrc = "bigsrc"
        File.open("bigsrc", "w") {|f| f << bigcontent }
        yield bigsrc, bigcontent
      }
    }
  end

  def test_copy_stream_bigcontent
    with_bigsrc {|bigsrc, bigcontent|
      ret = IO.copy_stream(bigsrc, "bigdst")
      assert_equal(bigcontent.bytesize, ret)
      assert_equal(bigcontent, File.read("bigdst"))
    }
  end

  def test_copy_stream_bigcontent_chop
    with_bigsrc {|bigsrc, bigcontent|
      ret = IO.copy_stream(bigsrc, "bigdst", nil, 100)
      assert_equal(bigcontent.bytesize-100, ret)
      assert_equal(bigcontent[100..-1], File.read("bigdst"))
    }
  end

  def test_copy_stream_bigcontent_mid
    with_bigsrc {|bigsrc, bigcontent|
      ret = IO.copy_stream(bigsrc, "bigdst", 30000, 100)
      assert_equal(30000, ret)
      assert_equal(bigcontent[100, 30000], File.read("bigdst"))
    }
  end

  def test_copy_stream_bigcontent_fpos
    with_bigsrc {|bigsrc, bigcontent|
      File.open(bigsrc) {|f|
        begin
          assert_equal(0, f.pos)
          ret = IO.copy_stream(f, "bigdst", nil, 10)
          assert_equal(bigcontent.bytesize-10, ret)
          assert_equal(bigcontent[10..-1], File.read("bigdst"))
          assert_equal(0, f.pos)
          ret = IO.copy_stream(f, "bigdst", 40, 30)
          assert_equal(40, ret)
          assert_equal(bigcontent[30, 40], File.read("bigdst"))
          assert_equal(0, f.pos)
        rescue NotImplementedError
          #skip "pread(2) is not implemented."
        end
      }
    }
  end

  def test_copy_stream_closed_pipe
    with_srccontent {|src,|
      with_pipe {|r, w|
        w.close
        assert_raise(IOError) { IO.copy_stream(src, w) }
      }
    }
  end

  def with_megacontent
    yield "abc" * 1234567
  end

  def with_megasrc
    mkcdtmpdir {
      with_megacontent {|megacontent|
        megasrc = "megasrc"
        File.open(megasrc, "w") {|f| f << megacontent }
        yield megasrc, megacontent
      }
    }
  end

  if have_nonblock?
    def test_copy_stream_megacontent_nonblock
      with_megacontent {|megacontent|
        with_pipe {|r1, w1|
          with_pipe {|r2, w2|
            begin
              r1.nonblock = true
              w2.nonblock = true
            rescue Errno::EBADF
              skip "nonblocking IO for pipe is not implemented"
            end
            t1 = Thread.new { w1 << megacontent; w1.close }
            t2 = Thread.new { r2.read }
            t3 = Thread.new {
              ret = IO.copy_stream(r1, w2)
              assert_equal(megacontent.bytesize, ret)
              w2.close
            }
            _, t2_value, _ = assert_join_threads([t1, t2, t3])
            assert_equal(megacontent, t2_value)
          }
        }
      }
    end
  end

  def test_copy_stream_megacontent_pipe_to_file
    with_megasrc {|megasrc, megacontent|
      with_pipe {|r1, w1|
        with_pipe {|r2, w2|
          t1 = Thread.new { w1 << megacontent; w1.close }
          t2 = Thread.new { r2.read }
          t3 = Thread.new {
            ret = IO.copy_stream(r1, w2)
            assert_equal(megacontent.bytesize, ret)
            w2.close
          }
          _, t2_value, _ = assert_join_threads([t1, t2, t3])
          assert_equal(megacontent, t2_value)
        }
      }
    }
  end

  def test_copy_stream_megacontent_file_to_pipe
    with_megasrc {|megasrc, megacontent|
      with_pipe {|r, w|
        t1 = Thread.new { r.read }
        t2 = Thread.new {
          ret = IO.copy_stream(megasrc, w)
          assert_equal(megacontent.bytesize, ret)
          w.close
        }
        t1_value, _ = assert_join_threads([t1, t2])
        assert_equal(megacontent, t1_value)
      }
    }
  end

  def test_copy_stream_rbuf
    mkcdtmpdir {
      begin
        pipe(proc do |w|
          File.open("foo", "w") {|f| f << "abcd" }
          File.open("foo") {|f|
            f.read(1)
            assert_equal(3, IO.copy_stream(f, w, 10, 1))
          }
          w.close
        end, proc do |r|
          assert_equal("bcd", r.read)
        end)
      rescue NotImplementedError
        skip "pread(2) is not implemtented."
      end
    }
  end

  def with_socketpair
    s1, s2 = UNIXSocket.pair
    begin
      yield s1, s2
    ensure
      s1.close unless s1.closed?
      s2.close unless s2.closed?
    end
  end

  def test_copy_stream_socket1
    with_srccontent("foobar") {|src, content|
      with_socketpair {|s1, s2|
        ret = IO.copy_stream(src, s1)
        assert_equal(content.bytesize, ret)
        s1.close
        assert_equal(content, s2.read)
      }
    }
  end if defined? UNIXSocket

  def test_copy_stream_socket2
    with_bigsrc {|bigsrc, bigcontent|
      with_socketpair {|s1, s2|
        t1 = Thread.new { s2.read }
        t2 = Thread.new {
          ret = IO.copy_stream(bigsrc, s1)
          assert_equal(bigcontent.bytesize, ret)
          s1.close
        }
        result, _ = assert_join_threads([t1, t2])
        assert_equal(bigcontent, result)
      }
    }
  end if defined? UNIXSocket

  def test_copy_stream_socket3
    with_bigsrc {|bigsrc, bigcontent|
      with_socketpair {|s1, s2|
        t1 = Thread.new { s2.read }
        t2 = Thread.new {
          ret = IO.copy_stream(bigsrc, s1, 10000)
          assert_equal(10000, ret)
          s1.close
        }
        result, _ = assert_join_threads([t1, t2])
        assert_equal(bigcontent[0,10000], result)
      }
    }
  end if defined? UNIXSocket

  def test_copy_stream_socket4
    with_bigsrc {|bigsrc, bigcontent|
      File.open(bigsrc) {|f|
        assert_equal(0, f.pos)
        with_socketpair {|s1, s2|
          t1 = Thread.new { s2.read }
          t2 = Thread.new {
            ret = IO.copy_stream(f, s1, nil, 100)
            assert_equal(bigcontent.bytesize-100, ret)
            assert_equal(0, f.pos)
            s1.close
          }
          result, _ = assert_join_threads([t1, t2])
          assert_equal(bigcontent[100..-1], result)
        }
      }
    }
  end if defined? UNIXSocket

  def test_copy_stream_socket5
    with_bigsrc {|bigsrc, bigcontent|
      File.open(bigsrc) {|f|
        assert_equal(bigcontent[0,100], f.read(100))
        assert_equal(100, f.pos)
        with_socketpair {|s1, s2|
          t1 = Thread.new { s2.read }
          t2 = Thread.new {
            ret = IO.copy_stream(f, s1)
            assert_equal(bigcontent.bytesize-100, ret)
            assert_equal(bigcontent.length, f.pos)
            s1.close
          }
          result, _ = assert_join_threads([t1, t2])
          assert_equal(bigcontent[100..-1], result)
        }
      }
    }
  end if defined? UNIXSocket

  def test_copy_stream_socket6
    mkcdtmpdir {
      megacontent = "abc" * 1234567
      File.open("megasrc", "w") {|f| f << megacontent }

      with_socketpair {|s1, s2|
        begin
          s1.nonblock = true
        rescue Errno::EBADF
          skip "nonblocking IO for pipe is not implemented"
        end
        t1 = Thread.new { s2.read }
        t2 = Thread.new {
          ret = IO.copy_stream("megasrc", s1)
          assert_equal(megacontent.bytesize, ret)
          s1.close
        }
        result, _ = assert_join_threads([t1, t2])
        assert_equal(megacontent, result)
      }
    }
  end if defined? UNIXSocket

  def test_copy_stream_socket7
    GC.start
    mkcdtmpdir {
      megacontent = "abc" * 1234567
      File.open("megasrc", "w") {|f| f << megacontent }

      with_socketpair {|s1, s2|
        begin
          s1.nonblock = true
        rescue Errno::EBADF
          skip "nonblocking IO for pipe is not implemented"
        end
        trapping_usr2 do |rd|
          nr = 30
          begin
            pid = fork do
              s1.close
              IO.select([s2])
              Process.kill(:USR2, Process.ppid)
              buf = String.new(capacity: 16384)
              nil while s2.read(16384, buf)
            end
            s2.close
            nr.times do
              assert_equal megacontent.bytesize, IO.copy_stream("megasrc", s1)
            end
            assert_equal(1, rd.read(4).unpack1('L'))
          ensure
            s1.close
            _, status = Process.waitpid2(pid) if pid
          end
          assert_predicate(status, :success?)
        end
      }
    }
  end if defined? UNIXSocket and IO.method_defined?("nonblock=")

  def test_copy_stream_strio
    src = StringIO.new("abcd")
    dst = StringIO.new
    ret = IO.copy_stream(src, dst)
    assert_equal(4, ret)
    assert_equal("abcd", dst.string)
    assert_equal(4, src.pos)
  end

  def test_copy_stream_strio_len
    src = StringIO.new("abcd")
    dst = StringIO.new
    ret = IO.copy_stream(src, dst, 3)
    assert_equal(3, ret)
    assert_equal("abc", dst.string)
    assert_equal(3, src.pos)
  end

  def test_copy_stream_strio_off
    src = StringIO.new("abcd")
    with_pipe {|r, w|
      assert_raise(ArgumentError) {
        IO.copy_stream(src, w, 3, 1)
      }
    }
  end

  def test_copy_stream_fname_to_strio
    mkcdtmpdir {
      File.open("foo", "w") {|f| f << "abcd" }
      src = "foo"
      dst = StringIO.new
      ret = IO.copy_stream(src, dst, 3)
      assert_equal(3, ret)
      assert_equal("abc", dst.string)
    }
  end
