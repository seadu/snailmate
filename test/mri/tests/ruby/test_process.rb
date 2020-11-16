# coding: utf-8
# frozen_string_literal: false
require 'test/unit'
require 'tempfile'
require 'timeout'
require 'io/wait'
require 'rbconfig'

class TestProcess < Test::Unit::TestCase
  RUBY = EnvUtil.rubybin

  def setup
    Process.waitall
  end

  def teardown
    Process.waitall
  end

  def windows?
    self.class.windows?
  end
  def self.windows?
    return /mswin|mingw|bccwin/ =~ RUBY_PLATFORM
  end

  def write_file(filename, content)
    File.open(filename, "w") {|f|
      f << content
    }
  end

  def with_tmpchdir
    Dir.mktmpdir {|d|
      d = File.realpath(d)
      Dir.chdir(d) {
        yield d
      }
    }
  end

  def run_in_child(str) # should be called in a temporary directory
    write_file("test-script", str)
    Process.wait spawn(RUBY, "test-script")
    $?
  end

  def test_rlimit_availability
    begin
      Process.getrlimit(nil)
    rescue NotImplementedError
      assert_raise(NotImplementedError) { Process.setrlimit }
    rescue TypeError
      assert_raise(ArgumentError) { Process.setrlimit }
    end
  end

  def rlimit_exist?
    Process.getrlimit(nil)
  rescue NotImplementedError
    return false
  rescue TypeError
    return true
  end

  def test_rlimit_nofile
    return unless rlimit_exist?
    with_tmpchdir {
      write_file 's', <<-"End"
        # Too small RLIMIT_NOFILE, such as zero, causes problems.
        # [OpenBSD] Setting to zero freezes this test.
        # [GNU/Linux] EINVAL on poll().  EINVAL on ruby's internal poll() ruby with "[ASYNC BUG] thread_timer: select".
        pipes = IO.pipe
	limit = pipes.map {|io| io.fileno }.min
	result = 1
	begin
	  Process.setrlimit(Process::RLIMIT_NOFILE, limit)
	rescue Errno::EINVAL
	  result = 0
	end
	if result == 1
	  begin
	    IO.pipe
	  rescue Errno::EMFILE
	   result = 0
	  end
	end
	exit result
      End
      pid = spawn RUBY, "s"
      Process.wait pid
      assert_equal(0, $?.to_i, "#{$?}")
    }
  end

  def test_rlimit_name
    return unless rlimit_exist?
    [
      :AS, "AS",
      :CORE, "CORE",
      :CPU, "CPU",
      :DATA, "DATA",
      :FSIZE, "FSIZE",
      :MEMLOCK, "MEMLOCK",
      :MSGQUEUE, "MSGQUEUE",
      :NICE, "NICE",
      :NOFILE, "NOFILE",
      :NPROC, "NPROC",
      :RSS, "RSS",
      :RTPRIO, "RTPRIO",
      :RTTIME, "RTTIME",
      :SBSIZE, "SBSIZE",
      :SIGPENDING, "SIGPENDING",
      :STACK, "STACK",
    ].each {|name|
      if Process.const_defined? "RLIMIT_#{name}"
        assert_nothing_raised { Process.getrlimit(name) }
      else
        assert_raise(ArgumentError) { Process.getrlimit(name) }
      end
    }
    assert_raise(ArgumentError) { Process.getrlimit(:FOO) }
    assert_raise(ArgumentError) { Process.getrlimit("FOO") }
    assert_raise_with_message(ArgumentError, /\u{30eb 30d3 30fc}/) { Process.getrlimit("\u{30eb 30d3 30fc}") }
  end

  def test_rlimit_value
    return unless rlimit_exist?
    assert_raise(ArgumentError) { Process.setrlimit(:FOO, 0) }
    assert_raise(ArgumentError) { Process.setrlimit(:CORE, :FOO) }
    assert_raise_with_message(ArgumentError, /\u{30eb 30d3 30fc}/) { Process.setrlimit("\u{30eb 30d3 30fc}", 0) }
    assert_raise_with_message(ArgumentError, /\u{30eb 30d3 30fc}/) { Process.setrlimit(:CORE, "\u{30eb 30d3 30fc}") }
    with_tmpchdir do
      s = run_in_child(<<-'End')
        cur, max = Process.getrlimit(:NOFILE)
        Process.setrlimit(:NOFILE, [max-10, cur].min)
        begin
          Process.setrlimit(:NOFILE, :INFINITY)
        rescue Errno::EPERM
          exit false
        end
      End
      assert_not_predicate(s, :success?)
      s = run_in_child(<<-'End')
        cur, max = Process.getrlimit(:NOFILE)
        Process.setrlimit(:NOFILE, [max-10, cur].min)
        begin
          Process.setrlimit(:NOFILE, "INFINITY")
        rescue Errno::EPERM
          exit false
        end
      End
      assert_not_predicate(s, :success?)
    end
  end

  TRUECOMMAND = [RUBY, '-e', '']

  def test_execopts_opts
    assert_nothing_raised {
      Process.wait Process.spawn(*TRUECOMMAND, {})
    }
    assert_raise(ArgumentError) {
      Process.wait Process.spawn(*TRUECOMMAND, :foo => 100)
    }
    assert_raise(ArgumentError) {
      Process.wait Process.spawn(*TRUECOMMAND, Process => 100)
    }
  end

  def test_execopts_pgroup
    skip "system(:pgroup) is not supported" if windows?
    assert_nothing_raised { system(*TRUECOMMAND, :pgroup=>false) }

    io = IO.popen([RUBY, "-e", "print Process.getpgrp"])
    assert_equal(Process.getpgrp.to_s, io.read)
    io.close

    io = IO.popen([RUBY, "-e", "print Process.getpgrp", :pgroup=>true])
    assert_equal(io.pid.to_s, io.read)
    io.close

    assert_raise(ArgumentError) { system(*TRUECOMMAND, :pgroup=>-1) }
    IO.popen([RUBY, '-egets'], 'w') do |f|
      assert_raise(Errno::EPERM) {
        Process.wait spawn(*TRUECOMMAND, :pgroup=>f.pid)
      }
    end

    io1 = IO.popen([RUBY, "-e", "print Process.getpgrp", :pgroup=>true])
    io2 = IO.popen([RUBY, "-e", "print Process.getpgrp", :pgroup=>io1.pid])
    assert_equal(io1.pid.to_s, io1.read)
    assert_equal(io1.pid.to_s, io2.read)
    Process.wait io1.pid
    Process.wait io2.pid
    io1.close
    io2.close
  end

  def test_execopts_rlimit
    return unless rlimit_exist?
    assert_raise(ArgumentError) { system(*TRUECOMMAND, :rlimit_foo=>0) }
    assert_raise(ArgumentError) { system(*TRUECOMMAND, :rlimit_NOFILE=>0) }
    assert_raise(ArgumentError) { system(*TRUECOMMAND, :rlimit_nofile=>[]) }
    assert_raise(ArgumentError) { system(*TRUECOMMAND, :rlimit_nofile=>[1,2,3]) }

    max = Process.getrlimit(:CORE).last

    n = max
    IO.popen([RUBY, "-e",
             "puts Process.getrlimit(:CORE)", :rlimit_core=>n]) {|io|
      assert_equal("#{n}\n#{n}\n", io.read)
    }

    n = 0
    IO.popen([RUBY, "-e",
             "puts Process.getrlimit(:CORE)", :rlimit_core=>n]) {|io|
      assert_equal("#{n}\n#{n}\n", io.read)
    }

    n = max
    IO.popen([RUBY, "-e",
             "puts Process.getrlimit(:CORE)", :rlimit_core=>[n]]) {|io|
      assert_equal("#{n}\n#{n}\n", io.read)
    }

    m, n = 0, max
    IO.popen([RUBY, "-e",
             "puts Process.getrlimit(:CORE)", :rlimit_core=>[m,n]]) {|io|
      assert_equal("#{m}\n#{n}\n", io.read)
    }

    m, n = 0, 0
    IO.popen([RUBY, "-e",
             "puts Process.getrlimit(:CORE)", :rlimit_core=>[m,n]]) {|io|
      assert_equal("#{m}\n#{n}\n", io.read)
    }

    n = max
    IO.popen([RUBY, "-e",
      "puts Process.getrlimit(:CORE), Process.getrlimit(:CPU)",
      :rlimit_core=>n, :rlimit_cpu=>3600]) {|io|
      assert_equal("#{n}\n#{n}\n""3600\n3600\n", io.read)
    }

    assert_raise(ArgumentError) do
      system(RUBY, '-e', 'exit',  'rlimit_bogus'.to_sym => 123)
    end
    assert_separately([],"#{<<~"begin;"}\n#{<<~'end;'}", 'rlimit_cpu'.to_sym => 3600)
    BUG = "[ruby-core:82033] [Bug #13744]"
    begin;
      assert_equal([3600,3600], Process.getrlimit(:CPU), BUG)
    end;

    assert_raise_with_message(ArgumentError, /bogus/) do
      system(RUBY, '-e', 'exit', :rlimit_bogus => 123)
    end

    assert_raise_with_message(ArgumentError, /rlimit_cpu/) {
      system(RUBY, '-e', 'exit', "rlimit_cpu\0".to_sym => 3600)
    }
  end

  def test_overwrite_ENV
    assert_separately([],"#{<<~"begin;"}\n#{<<~"end;"}")
    BUG = "[ruby-core:105223] [Bug #18164]"
    begin;
      $VERBOSE = nil
      ENV = {}
      pid = spawn({}, *#{TRUECOMMAND.inspect})
      ENV.replace({})
      assert_kind_of(Integer, pid, BUG)
    end;
  end

  MANDATORY_ENVS = %w[RUBYLIB MJIT_SEARCH_BUILD_DIR]
  case RbConfig::CONFIG['target_os']
  when /linux/
    MANDATORY_ENVS << 'LD_PRELOAD'
  when /mswin|mingw/
    MANDATORY_ENVS.concat(%w[HOME USER TMPDIR])
  when /darwin/
    MANDATORY_ENVS.concat(ENV.keys.grep(/\A__CF_/))
  end
  if e = RbConfig::CONFIG['LIBPATHENV']
    MANDATORY_ENVS << e
  end
  if e = RbConfig::CONFIG['PRELOADENV'] and !e.empty?
    MANDATORY_ENVS << e
  end
  PREENVARG = ['-e', "%w[#{MANDATORY_ENVS.join(' ')}].each{|e|ENV.delete(e)}"]
  ENVARG = ['-e', 'ENV.each {|k,v| puts "#{k}=#{v}" }']
  ENVCOMMAND = [RUBY].concat(PREENVARG).concat(ENVARG)

  def test_execopts_env
    assert_raise(ArgumentError) {
      system({"F=O"=>"BAR"}, *TRUECOMMAND)
    }

    with_tmpchdir {|d|
      prog = "#{d}/notexist"
      e = assert_raise(Errno::ENOENT) {
        Process.wait Process.spawn({"FOO"=>"BAR"}, prog)
      }
      assert_equal(prog, e.message.sub(/.* - /, ''))
      e = assert_raise(Errno::ENOENT) {
        Process.wait Process.spawn({"FOO"=>"BAR"}, [prog, "blar"])
      }
      assert_equal(prog, e.message.sub(/.* - /, ''))
    }
    h = {}
    cmd = [h, RUBY]
    (ENV.keys + MANDATORY_ENVS).each do |k|
      case k
      when /\APATH\z/i
      when *MANDATORY_ENVS
        cmd << '-e' << "ENV.delete('#{k}')"
      else
        h[k] = nil
      end
    end
    cmd << '-e' << 'puts ENV.keys.map{|e|e.upcase}'
    IO.popen(cmd) {|io|
      assert_equal("PATH\n", io.read)
    }

    IO.popen([{"FOO"=>"BAR"}, *ENVCOMMAND]) {|io|
      assert_match(/^FOO=BAR$/, io.read)
    }

    with_tmpchdir {|d|
      system({"fofo"=>"haha"}, *ENVCOMMAND, STDOUT=>"out")
      assert_match(/^fofo=haha$/, File.read("out").chomp)
    }

    old = ENV["hmm"]
    begin
      ENV["hmm"] = "fufu"
      IO.popen(ENVCOMMAND) {|io| assert_match(/^hmm=fufu$/, io.read) }
      IO.popen([{"hmm"=>""}, *ENVCOMMAND]) {|io| assert_match(/^hmm=$/, io.read) }
      IO.popen([{"hmm"=>nil}, *ENVCOMMAND]) {|io| assert_not_match(/^hmm=/, io.read) }
      ENV["hmm"] = ""
      IO.popen(ENVCOMMAND) {|io| assert_match(/^hmm=$/, io.read) }
      IO.popen([{"hmm"=>""}, *ENVCOMMAND]) {|io| assert_match(/^hmm=$/, io.read) }
      IO.popen([{"hmm"=>nil}, *ENVCOMMAND]) {|io| assert_not_match(/^hmm=/, io.read) }
      ENV["hmm"] = nil
      IO.popen(ENVCOMMAND) {|io| assert_not_match(/^hmm=/, io.read) }
      IO.popen([{"hmm"=>""}, *ENVCOMMAND]) {|io| assert_match(/^hmm=$/, io.read) }
      IO.popen([{"hmm"=>nil}, *ENVCOMMAND]) {|io| assert_not_match(/^hmm=/, io.read) }
    ensure
      ENV["hmm"] = old
    end

    assert_raise_