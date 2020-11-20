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

    assert_raise_with_message(ArgumentError, /fo=fo/) {
      system({"fo=fo"=>"ha"}, *ENVCOMMAND)
    }
    assert_raise_with_message(ArgumentError, /\u{30c0}=\u{30e1}/) {
      system({"\u{30c0}=\u{30e1}"=>"ha"}, *ENVCOMMAND)
    }
  end

  def test_execopt_env_path
    bug8004 = '[ruby-core:53103] [Bug #8004]'
    Dir.mktmpdir do |d|
      open("#{d}/tmp_script.cmd", "w") {|f| f.puts ": ;"; f.chmod(0755)}
      assert_not_nil(pid = Process.spawn({"PATH" => d}, "tmp_script.cmd"), bug8004)
      wpid, st = Process.waitpid2(pid)
      assert_equal([pid, true], [wpid, st.success?], bug8004)
    end
  end

  def _test_execopts_env_popen(cmd)
    message = cmd.inspect
    IO.popen({"FOO"=>"BAR"}, cmd) {|io|
      assert_equal('FOO=BAR', io.read[/^FOO=.*/], message)
    }

    old = ENV["hmm"]
    begin
      ENV["hmm"] = "fufu"
      IO.popen(cmd) {|io| assert_match(/^hmm=fufu$/, io.read, message)}
      IO.popen({"hmm"=>""}, cmd) {|io| assert_match(/^hmm=$/, io.read, message)}
      IO.popen({"hmm"=>nil}, cmd) {|io| assert_not_match(/^hmm=/, io.read, message)}
      ENV["hmm"] = ""
      IO.popen(cmd) {|io| assert_match(/^hmm=$/, io.read, message)}
      IO.popen({"hmm"=>""}, cmd) {|io| assert_match(/^hmm=$/, io.read, message)}
      IO.popen({"hmm"=>nil}, cmd) {|io| assert_not_match(/^hmm=/, io.read, message)}
      ENV["hmm"] = nil
      IO.popen(cmd) {|io| assert_not_match(/^hmm=/, io.read, message)}
      IO.popen({"hmm"=>""}, cmd) {|io| assert_match(/^hmm=$/, io.read, message)}
      IO.popen({"hmm"=>nil}, cmd) {|io| assert_not_match(/^hmm=/, io.read, message)}
    ensure
      ENV["hmm"] = old
    end
  end

  def test_execopts_env_popen_vector
    _test_execopts_env_popen(ENVCOMMAND)
  end

  def test_execopts_env_popen_string
    with_tmpchdir do |d|
      open('test-script', 'w') do |f|
        ENVCOMMAND.each_with_index do |cmd, i|
          next if i.zero? or cmd == "-e"
          f.puts cmd
        end
      end
      _test_execopts_env_popen("#{RUBY} test-script")
    end
  end

  def test_execopts_preserve_env_on_exec_failure
    with_tmpchdir {|d|
      write_file 's', <<-"End"
        ENV["mgg"] = nil
        prog = "./nonexistent"
        begin
          Process.exec({"mgg" => "mggoo"}, [prog, prog])
        rescue Errno::ENOENT
        end
        open('out', 'w') {|f|
          f.print ENV["mgg"].inspect
        }
      End
      system(RUBY, 's')
      assert_equal(nil.inspect, File.read('out'),
        "[ruby-core:44093] [ruby-trunk - Bug #6249]")
    }
  end

  def test_execopts_env_single_word
    with_tmpchdir {|d|
      open("test_execopts_env_single_word.rb", "w") {|f|
        f.puts "print ENV['hgga']"
      }
      system({"hgga"=>"ugu"}, RUBY,
             :in => 'test_execopts_env_single_word.rb',
             :out => 'test_execopts_env_single_word.out')
      assert_equal('ugu', File.read('test_execopts_env_single_word.out'))
    }
  end

  def test_execopts_unsetenv_others
    h = {}
    MANDATORY_ENVS.each {|k| e = ENV[k] and h[k] = e}
    IO.popen([h, *ENVCOMMAND, :unsetenv_others=>true]) {|io|
      assert_equal("", io.read)
    }
    IO.popen([h.merge("A"=>"B"), *ENVCOMMAND, :unsetenv_others=>true]) {|io|
      assert_equal("A=B\n", io.read)
    }
  end

  PWD = [RUBY, '-e', 'puts Dir.pwd']

  def test_execopts_chdir
    with_tmpchdir {|d|
      IO.popen([*PWD, :chdir => d]) {|io|
        assert_equal(d, io.read.chomp)
      }
      assert_raise_with_message(Errno::ENOENT, %r"d/notexist") {
        Process.wait Process.spawn(*PWD, :chdir => "d/notexist")
      }
      n = "d/\u{1F37A}"
      assert_raise_with_message(Errno::ENOENT, /#{n}/) {
        Process.wait Process.spawn(*PWD, :chdir => n)
      }
    }
  end

  def test_execopts_open_chdir
    with_tmpchdir {|d|
      Dir.mkdir "foo"
      system(*PWD, :chdir => "foo", :out => "open_chdir_test")
      assert_file.exist?("open_chdir_test")
      assert_file.not_exist?("foo/open_chdir_test")
      assert_equal("#{d}/foo", File.read("open_chdir_test").chomp)
    }
  end

  def test_execopts_open_chdir_m17n_path
    with_tmpchdir {|d|
      Dir.mkdir "テスト"
      (pwd = PWD.dup).insert(1, '-EUTF-8:UTF-8')
      system(*pwd, :chdir => "テスト", :out => "open_chdir_テスト")
      assert_file.exist?("open_chdir_テスト")
      assert_file.not_exist?("テスト/open_chdir_テスト")
      assert_equal("#{d}/テスト", File.read("open_chdir_テスト", encoding: "UTF-8").chomp)
    }
  end if windows? || Encoding.find('locale') == Encoding::UTF_8

  def test_execopts_open_failure
    with_tmpchdir {|d|
      assert_raise_with_message(Errno::ENOENT, %r"d/notexist") {
        Process.wait Process.spawn(*PWD, :in => "d/notexist")
      }
      assert_raise_with_message(Errno::ENOENT, %r"d/notexist") {
        Process.wait Process.spawn(*PWD, :out => "d/notexist")
      }
      n = "d/\u{1F37A}"
      assert_raise_with_message(Errno::ENOENT, /#{n}/) {
        Process.wait Process.spawn(*PWD, :in => n)
      }
      assert_raise_with_message(Errno::ENOENT, /#{n}/) {
        Process.wait Process.spawn(*PWD, :out => n)
      }
    }
  end

  UMASK = [RUBY, '-e', 'printf "%04o\n", File.umask']

  def test_execopts_umask
    skip "umask is not supported" if windows?
    IO.popen([*UMASK, :umask => 0]) {|io|
      assert_equal("0000", io.read.chomp)
    }
    IO.popen([*UMASK, :umask => 0777]) {|io|
      assert_equal("0777", io.read.chomp)
    }
  end

  def with_pipe
    begin
      r, w = IO.pipe
      yield r, w
    ensure
      r.close unless r.closed?
      w.close unless w.closed?
    end
  end

  def with_pipes(n)
    ary = []
    begin
      n.times {
        ary << IO.pipe
      }
      yield ary
    ensure
      ary.each {|r, w|
        r.close unless r.closed?
        w.close unless w.closed?
      }
    end
  end

  ECHO = lambda {|arg| [RUBY, '-e', "puts #{arg.dump}; STDOUT.flush"] }
  SORT = [RUBY, '-e', "puts ARGF.readlines.sort"]
  CAT = [RUBY, '-e', "IO.copy_stream STDIN, STDOUT"]

  def test_execopts_redirect_fd
    with_tmpchdir {|d|
      Process.wait Process.spawn(*ECHO["a"], STDOUT=>["out", File::WRONLY|File::CREAT|File::TRUNC, 0644])
      assert_equal("a", File.read("out").chomp)
      if windows?
        # currently telling to child the file modes is not supported.
        open("out", "a") {|f| f.write "0\n"}
      else
        Process.wait Process.spawn(*ECHO["0"], STDOUT=>["out", File::WRONLY|File::CREAT|File::APPEND, 0644])
        assert_equal("a\n0\n", File.read("out"))
      end
      Process.wait Process.spawn(*SORT, STDIN=>["out", File::RDONLY, 0644],
                                         STDOUT=>["out2", File::WRONLY|File::CREAT|File::TRUNC, 0644])
      assert_equal("0\na\n", File.read("out2"))
      Process.wait Process.spawn(*ECHO["b"], [STDOUT, STDERR]=>["out", File::WRONLY|File::CREAT|File::TRUNC, 0644])
      assert_equal("b", File.read("out").chomp)
      # problem occur with valgrind
      #Process.wait Process.spawn(*ECHO["a"], STDOUT=>:close, STDERR=>["out", File::WRONLY|File::CREAT|File::TRUNC, 0644])
      #p File.read("out")
      #assert_not_empty(File.read("out")) # error message such as "-e:1:in `flush': Bad file descriptor (Errno::EBADF)"
      Process.wait Process.spawn(*ECHO["c"], STDERR=>STDOUT, STDOUT=>["out", File::WRONLY|File::CREAT|File::TRUNC, 0644])
      assert_equal("c", File.read("out").chomp)
      File.open("out", "w") {|f|
        Process.wait Process.spawn(*ECHO["d"], STDOUT=>f)
        assert_equal("d", File.read("out").chomp)
      }
      opts = {STDOUT=>["out", File::WRONLY|File::CREAT|File::TRUNC, 0644]}
      opts.merge(3=>STDOUT, 4=>STDOUT, 5=>STDOUT, 6=>STDOUT, 7=>STDOUT) unless windows?
      Process.wait Process.spawn(*ECHO["e"], opts)
      assert_equal("e", File.read("out").chomp)
      opts = {STDOUT=>["out", File::WRONLY|File::CREAT|File::TRUNC, 0644]}
      opts.merge(3=>0, 4=>:in, 5=>STDIN, 6=>1, 7=>:out, 8=>STDOUT, 9=>2, 10=>:err, 11=>STDERR) unless windows?
      Process.wait Process.spawn(*ECHO["ee"], opts)
      assert_equal("ee", File.read("out").chomp)
      unless windows?
        # passing non-stdio fds is not supported on Windows
        File.open("out", "w") {|f|
          h = {STDOUT=>f, f=>STDOUT}
          3.upto(30) {|i| h[i] = STDOUT if f.fileno != i }
          Process.wait Process.spawn(*ECHO["f"], h)
          assert_equal("f", File.read("out").chomp)
        }
      end
      assert_raise(ArgumentError) {
        Process.wait Process.spawn(*ECHO["f"], 1=>Process)
      }
      assert_raise(ArgumentError) {
        Process.wait Process.spawn(*ECHO["f"], [Process]=>1)
      }
      assert_raise(ArgumentError) {
        Process.wait Process.spawn(*ECHO["f"], [1, STDOUT]=>2)
      }
      assert_raise(ArgumentError) {
        Process.wait Process.spawn(*ECHO["f"], -1=>2)
      }
      Process.wait Process.spawn(*ECHO["hhh\nggg\n"], STDOUT=>"out")
      assert_equal("hhh\nggg\n", File.read("out"))
      Process.wait Process.spawn(*SORT, STDIN=>"out", STDOUT=>"out2")
      assert_equal("ggg\nhhh\n", File.read("out2"))

      unless windows?
        # passing non-stdio fds is not supported on Windows
        assert_raise(Errno::ENOENT) {
          Process.wait Process.spawn("non-existing-command", (3..60).to_a=>["err", File::WRONLY|File::CREAT])
        }
        assert_equal("", File.read("err"))
      end

      system(*ECHO["bb\naa\n"], STDOUT=>["out", "w"])
      assert_equal("bb\naa\n", File.read("out"))
      system(*SORT, STDIN=>["out"], STDOUT=>"out2")
      assert_equal("aa\nbb\n", File.read("out2"))
    }
  end

  def test_execopts_redirect_open_order_normal
    minfd = 3
    maxfd = 20
    with_tmpchdir {|d|
      opts = {}
      minfd.upto(maxfd) {|fd| opts[fd] = ["out#{fd}", "w"] }
      system RUBY, "-e", "#{minfd}.upto(#{maxfd}) {|fd| IO.new(fd).print fd.to_s }", opts
      minfd.upto(maxfd) {|fd| assert_equal(fd.to_s, File.read("out#{fd}")) }
    }
  end unless windows? # passing non-stdio fds is not supported on Windows

  def test_execopts_redirect_open_order_reverse
    minfd = 3
    maxfd = 20
    with_tmpchdir {|d|
      opts = {}
      maxfd.downto(minfd) {|fd| opts[fd] = ["out#{fd}", "w"] }
      system RUBY, "-e", "#{minfd}.upto(#{maxfd}) {|fd| IO.new(fd).print fd.to_s }", opts
      minfd.upto(maxfd) {|fd| assert_equal(fd.to_s, File.read("out#{fd}")) }
    }
  end unless windows? # passing non-stdio fds is not supported on Windows

  def test_execopts_redirect_open_fifo
    with_tmpchdir {|d|
      begin
        File.mkfifo("fifo")
      rescue NotImplementedError
        return
      end
      assert_file.pipe?("fifo")
      t1 = Thread.new {
        system(*ECHO["output to fifo"], :out=>"fifo")
      }
      t2 = Thread.new {
        IO.popen([*CAT, :in=>"fifo"]) {|f| f.read }
      }
      _, v2 = assert_join_threads([t1, t2])
      assert_equal("output to fifo\n", v2)
    }
  end unless windows? # does not support fifo

  def test_execopts_redirect_open_fifo_interrupt_raise
    with_tmpchdir {|d|
      begin
        File.mkfifo("fifo")
      rescue NotImplementedError
        return
      end
      IO.popen([RUBY, '-e', <<-'EOS']) {|io|
        class E < StandardError; end
        trap(:USR1) { raise E }
        begin
          puts "start"
          STDOUT.flush
          system("cat", :in => "fifo")
        rescue E
          puts "ok"
        end
      EOS
        assert_equal("start\n", io.gets)
        sleep 0.5
        Process.kill(:USR1, io.pid)
        assert_equal("ok\n", io.read)
      }
    }
  end unless windows? # does not support fifo

  def test_execopts_redirect_open_fifo_interrupt_print
    with_tmpchdir {|d|
      begin
        File.mkfifo("fifo")
      rescue NotImplementedError
        return
      end
      IO.popen([RUBY, '-e', <<-'EOS']) {|io|
        STDOUT.sync = true
        trap(:USR1) { print "trap\n" }
        puts "start"
        system("cat", :in => "fifo")
      EOS
        assert_equal("start\n", io.gets)
        sleep 0.2 # wait for the child to stop at opening "fifo"
        Process.kill(:USR1, io.pid)
        assert_equal("trap\n", io.readpartial(8))
        File.write("fifo", "ok\n")
        assert_equal("ok\n", io.read)
      }
    }
  end unless windows? # does not support fifo

  def test_execopts_redirect_pipe
    with_pipe {|r1, w1|
      with_pipe {|r2, w2|
        opts = {STDIN=>r1, STDOUT=>w2}
        opts.merge(w1=>:close, r2=>:close) unless windows?
        pid = spawn(*SORT, opts)
        r1.close
        w2.close
        w1.puts "c"
        w1.puts "a"
        w1.puts "b"
        w1.close
        assert_equal("a\nb\nc\n", r2.read)
        r2.close
        Process.wait(pid)
      }
    }

    unless windows?
      # passing non-stdio fds is not supported on Windows
      with_pipes(5) {|pipes|
        ios = pipes.flatten
        h = {}
        ios.length.times {|i| h[ios[i]] = ios[(i-1)%ios.length] }
        h2 = h.invert
        _rios = pipes.map {|r, w| r }
        wios  = pipes.map {|r, w| w }
        child_wfds = wios.map {|w| h2[w].fileno }
        pid = spawn(RUBY, "-e",
                    "[#{child_wfds.join(',')}].each {|fd| IO.new(fd, 'w').puts fd }", h)
        pipes.each {|r, w|
          assert_equal("#{h2[w].fileno}\n", r.gets)
        }
        Process.wait pid;
      }

      with_pipes(5) {|pipes|
        ios = pipes.flatten
        h = {}
        ios.length.times {|i| h[ios[i]] = ios[(i+1)%ios.length] }
        h2 = h.invert
        _rios = pipes.map {|r, w| r }
        wios  = pipes.map {|r, w| w }
        child_wfds = wios.map {|w| h2[w].fileno }
        pid = spawn(RUBY, "-e",
                    "[#{child_wfds.join(',')}].each {|fd| IO.new(fd, 'w').puts fd }", h)
        pipes.each {|r, w|
          assert_equal("#{h2[w].fileno}\n", r.gets)
        }
        Process.wait pid
      }

      closed_fd = nil
      with_pipes(5) {|pipes|
        io = pipes.last.last
        closed_fd = io.fileno
      }
      assert_raise(Errno::EBADF) { Process.wait spawn(*TRUECOMMAND, closed_fd=>closed_fd) }

      with_pipe {|r, w|
        if w.respond_to?(:"close_on_exec=")
          w.close_on_exec = true
          pid = spawn(RUBY, "-e", "IO.new(#{w.fileno}, 'w').print 'a'", w=>w)
          w.close
          assert_equal("a", r.read)
          Process.wait pid
        end
      }

      # ensure standard FDs we redirect to are blocking for compatibility
      with_pipes(3) do |pipes|
        src = 'p [STDIN,STDOUT,STDERR].map(&:nonblock?)'
        rdr = { 0 => pipes[0][0], 1 => pipes[1][1], 2 => pipes[2][1] }
        pid = spawn(RUBY, '-rio/nonblock', '-e', src, rdr)
        assert_equal("[false, false, false]\n", pipes[1][0].gets)
        Process.wait pid
      end
    end
  end

  def test_execopts_redirect_symbol
    with_tmpchdir {|d|
      system(*ECHO["funya"], :out=>"out")
      assert_equal("funya\n", File.read("out"))
      system(RUBY, '-e', 'STDOUT.reopen(STDERR); puts "henya"', :err=>"out")
      assert_equal("henya\n", File.read("out"))
      IO.popen([*CAT, :in=>"out"]) {|io|
        assert_equal("henya\n", io.read)
      }
    }
  end

  def test_execopts_redirect_nonascii_path
    bug9946 = '[ruby-core:63185] [Bug #9946]'
    with_tmpchdir {|d|
      path = "t-\u{30c6 30b9 30c8 f6}.txt"
      system(*ECHO["a"], out: path)
      assert_file.for(bug9946).exist?(path)
      assert_equal("a\n", File.read(path), bug9946)
    }
  end

  def test_execopts_redirect_to_out_and_err
    with_tmpchdir {|d|
      ret = system(RUBY, "-e", 'STDERR.print "e"; STDOUT.print "o"', [:out, :err] => "foo")
      assert_equal(true, ret)
      assert_equal("eo", File.read("foo"))
      ret = system(RUBY, "-e", 'STDERR.print "E"; STDOUT.print "O"', [:err, :out] => "bar")
      assert_equal(true, ret)
      assert_equal("EO", File.read("bar"))
    }
  end

  def test_execopts_redirect_dup2_child
    with_tmpchdir {|d|
      Process.wait spawn(RUBY, "-e", "STDERR.print 'err'; STDOUT.print 'out'",
                         STDOUT=>"out", STDERR=>[:child, STDOUT])
      assert_equal("errout", File.read("out"))

      Process.wait spawn(RUBY, "-e", "STDERR.print 'err'; STDOUT.print 'out'",
                         STDERR=>"out", STDOUT=>[:child, STDERR])
      assert_equal("errout", File.read("out"))

      skip "inheritance of fd other than stdin,stdout and stderr is not supported" if windows?
      Process.wait spawn(RUBY, "-e", "STDERR.print 'err'; STDOUT.print 'out'",
                         STDOUT=>"out",
                         STDERR=>[:child, 3],
                         3=>[:child, 4],
                         4=>[:child, STDOUT]
                        )
      assert_equal("errout", File.read("out"))

      IO.popen([RUBY, "-e", "STDERR.print 'err'; STDOUT.print 'out'", STDERR=>[:child, STDOUT]]) {|io|
        assert_equal("errout", io.read)
      }

      assert_raise(ArgumentError) { Process.wait spawn(*TRUECOMMAND, STDOUT=>[:child, STDOUT]) }
      assert_raise(ArgumentError) { Process.wait spawn(*TRUECOMMAND, 3=>[:child, 4], 4=>[:child, 3]) }
      assert_raise(ArgumentError) { Process.wait spawn(*TRUECOMMAND, 3=>[:child, 4], 4=>[:child, 5], 5=>[:child, 3]) }
      assert_raise(ArgumentError) { Process.wait spawn(*TRUECOMMAND, STDOUT=>[:child, 3]) }
    }
  end

  def test_execopts_exec
    with_tmpchdir {|d|
      write_file("s", 'exec "echo aaa", STDOUT=>"foo"')
      pid = spawn RUBY, 's'
      Process.wait pid
      assert_equal("aaa\n", File.read("foo"))
    }
  end

  def test_execopts_popen
    with_tmpchdir {|d|
      IO.popen("#{RUBY} -e 'puts :foo'") {|io| assert_equal("foo\n", io.read) }
      assert_raise(Errno::ENOENT) { IO.popen(["echo bar"]) {} } # assuming "echo bar" command not exist.
      IO.popen(ECHO["baz"]) {|io| assert_equal("baz\n", io.read) }
    }
  end

  def test_execopts_popen_stdio
    with_tmpchdir {|d|
      assert_raise(ArgumentError) {
        IO.popen([*ECHO["qux"], STDOUT=>STDOUT]) {|io| }
      }
      IO.popen([*ECHO["hoge"], STDERR=>STDOUT]) {|io|
        assert_equal("hoge\n", io.read)
      }
      assert_raise(ArgumentError) {
        IO.popen([*ECHO["fuga"], STDOUT=>"out"]) {|io| }
      }
    }
  end

  def test_execopts_popen_extra_fd
    skip "inheritance of fd other than stdin,stdout and stderr is not supported" if windows?
    with_tmpchdir {|d|
      with_pipe {|r, w|
        IO.popen([RUBY, '-e', 'IO.new(3, "w").puts("a"); puts "b"', 3=>w]) {|io|
          assert_equal("b\n", io.read)
        }
        w.close
        assert_equal("a\n", r.read)
      }
      IO.popen([RUBY, '-e', "IO.new(9, 'w').puts(:b)",
               9=>["out2", File::WRONLY|File::CREAT|File::TRUNC]]) {|io|
        assert_equal("", io.read)
      }
      assert_equal("b\n", File.read("out2"))
    }
  end

  def test_popen_fork
    IO.popen("-") {|io|
      if !io
        puts "fooo"
      else
        assert_equal("fooo\n", io.read)
      end
    }
  rescue NotImplementedError
  end

  def test_fd_inheritance
    skip "inheritance of fd other than stdin,stdout and stderr is not supported" if windows?
    with_pipe {|r, w|
      system(RUBY, '-e', 'IO.new(ARGV[0].to_i, "w").puts(:ba)', w.fileno.to_s, w=>w)
      w.close
      assert_equal("ba\n", r.read)
    }
    with_pipe {|r, w|
      Process.wait spawn(RUBY, '-e',
                         'IO.new(ARGV[0].to_i, "w").puts("bi") rescue nil',
                         w.fileno.to_s)
      w.close
      assert_equal("", r.read)
    }
    with_pipe {|r, w|
      with_tmpchdir {|d|
	write_file("s", <<-"End")
	  exec(#{RUBY.dump}, '-e',
	       'IO.new(ARGV[0].to_i, "w").puts("bu") rescue nil',
	       #{w.fileno.to_s.dump}, :close_others=>false)
	End
        w.close_on_exec = false
	Process.wait spawn(RUBY, "s", :close_others=>false)
	w.close
	assert_equal("bu\n", r.read)
      }
    }
    with_pipe {|r, w|
      io = IO.popen([RUBY, "-e", "STDERR.reopen(STDOUT); IO.new(#{w.fileno}, 'w').puts('me')"])
      begin
        w.close
        errmsg = io.read
        assert_equal("", r.read)
        assert_not_equal("", errmsg)
      ensure
        io.close
      end
    }
    with_pipe {|r, w|
      errmsg = `#{RUBY} -e "STDERR.reopen(STDOUT); IO.new(#{w.fileno}, 'w').puts(123)"`
      w.close
      assert_equal("", r.read)
      assert_not_equal("", errmsg)
    }
  end

  def test_execopts_close_others
    skip "inheritance of fd other than stdin,stdout and stderr is not supported" if windows?
    with_tmpchdir {|d|
      with_pipe {|r, w|
        system(RUBY, '-e', 'STDERR.reopen("err", "w"); IO.new(ARGV[0].to_i, "w").puts("ma")', w.fileno.to_s, :close_others=>true)
        w.close
        assert_equal("", r.read)
        assert_not_equal("", File.read("err"))
        File.unlink("err")
      }
      with_pipe {|r, w|
        Process.wait spawn(RUBY, '-e', 'STDERR.reopen("err", "w"); IO.new(ARGV[0].to_i, "w").puts("mi")', w.fileno.to_s, :close_others=>true)
        w.close
        assert_equal("", r.read)
        assert_not_equal("", File.read("err"))
        File.unlink("err")
      }
      with_pipe {|r, w|
        w.close_on_exec = false
        Process.wait spawn(RUBY, '-e', 'IO.new(ARGV[0].to_i, "w").puts("bi")', w.fileno.to_s, :close_others=>false)
        w.close
        assert_equal("bi\n", r.read)
      }
      with_pipe {|r, w|
	write_file("s", <<-"End")
	  exec(#{RUBY.dump}, '-e',
	       'STDERR.reopen("err", "w"); IO.new(ARGV[0].to_i, "w").puts("mu")',
	       #{w.fileno.to_s.dump},
	       :close_others=>true)
	End
        Process.wait spawn(RUBY, "s", :close_others=>false)
        w.close
        assert_equal("", r.read)
        assert_not_equal("", File.read("err"))
        File.unlink("err")
      }
      with_pipe {|r, w|
        io = IO.popen([RUBY, "-e", "STDERR.reopen(STDOUT); IO.new(#{w.fileno}, 'w').puts('me')", :close_others=>true])
        begin
          w.close
          errmsg = io.read
          assert_equal("", r.read)
          assert_not_equal("", errmsg)
        ensure
          io.close
        end
      }
      with_pipe {|r, w|
        w.close_on_exec = false
        io = IO.popen([RUBY, "-e", "STDERR.reopen(STDOUT); IO.new(#{w.fileno}, 'w').puts('mo')", :close_others=>false])
        begin
          w.close
          errmsg = io.read
          assert_equal("mo\n", r.read)
          assert_equal("", errmsg)
        ensure
          io.close
        end
      }
      with_pipe {|r, w|
        w.close_on_exec = false
        io = IO.popen([RUBY, "-e", "STDERR.reopen(STDOUT); IO.new(#{w.fileno}, 'w').puts('mo')", :close_others=>nil])
        begin
          w.close
          errmsg = io.read
          assert_equal("mo\n", r.read)
          assert_equal("", errmsg)
        ensure
          io.close
        end
      }

    }
  end

  def test_close_others_default_false
    IO.pipe do |r,w|
      w.close_on_exec = false
      src = "IO.new(#{w.fileno}).puts(:hi)"
      assert_equal true, system(*%W(#{RUBY} --disable=gems -e #{src}))
      assert_equal "hi\n", r.gets
    end
  end unless windows? # passing non-stdio fds is not supported on Windows

  def test_execopts_redirect_self
    begin
      with_pipe {|r, w|
        w << "haha\n"
        w.close
        r.close_on_exec = true
        IO.popen([RUBY, "-e", "print IO.new(#{r.fileno}, 'r').read", r.fileno=>r.fileno, :close_others=>false]) {|io|
          assert_equal("haha\n", io.read)
        }
      }
    rescue NotImplementedError
      skip "IO#close_on_exec= is not supported"
    end
  end unless windows? # passing non-stdio fds is not supported on Windows

  def test_execopts_redirect_tempfile
    bug6269 = '[ruby-core:44181]'
    Tempfile.create("execopts") do |tmp|
      pid = assert_nothing_raised(ArgumentError, bug6269) do
        break spawn(RUBY, "-e", "print $$", out: tmp)
      end
      Process.wait(pid)
      tmp.rewind
      assert_equal(pid.to_s, tmp.read)
    end
  end

  def test_execopts_duplex_io
    IO.popen("#{RUBY} -e ''", "r+") {|duplex|
      assert_raise(ArgumentError) { system("#{RUBY} -e ''", duplex=>STDOUT) }
      assert_raise(ArgumentError) { system("#{RUBY} -e ''", STDOUT=>duplex) }
    }
  end

  def test_execopts_modification
    h = {}
    Process.wait spawn(*TRUECOMMAND, h)
    assert_equal({}, h)

    h = {}
    system(*TRUECOMMAND, h)
    assert_equal({}, h)

    h = {}
    io = IO.popen([*TRUECOMMAND, h])
    io.close
    assert_equal({}, h)
  end

  def test_system_noshell
    str = "echo non existing command name which contains spaces"
    assert_nil(system([str, str]))
  end

  def test_spawn_noshell
    str = "echo non existing command name which contains spaces"
    assert_raise(Errno::ENOENT) { spawn([str, str]) }
  end

  def test_popen_noshell
    str = "echo non existing command name which contains spaces"
    assert_raise(Errno::ENOENT) { IO.popen([str, str]) }
  end

  def test_exec_noshell
    with_tmpchdir {|d|
      write_file("s", <<-"End")
	  str = "echo non existing command name which contains spaces"
	  STDERR.reopen(STDOUT)
	  begin
	    exec [str, str]
	  rescue Errno::ENOENT
	    print "Errno::ENOENT success"
	  end
	End
      r = IO.popen([RUBY, "s", :close_others=>false], "r") {|f| f.read}
      assert_equal("Errno::ENOENT success", r)
    }
  end

  def test_system_wordsplit
    with_tmpchdir {|d|
      write_file("script", <<-'End')
        File.open("result", "w") {|t| t << "haha pid=#{$$} ppid=#{Process.ppid}" }
        exit 5
      End
      str = "#{RUBY} script"
      ret = system(str)
      status = $?
      assert_equal(false, ret)
      assert_predicate(status, :exited?)
      assert_equal(5, status.exitstatus)
      assert_equal("haha pid=#{status.pid} ppid=#{$$}", File.read("result"))
    }
  end

  def test_spawn_wordsplit
    with_tmpchdir {|d|
      write_file("script", <<-'End')
        File.open("result", "w") {|t| t << "hihi pid=#{$$} ppid=#{Process.ppid}" }
        exit 6
      End
      str = "#{RUBY} script"
      pid = spawn(str)
      Process.wait pid
      status = $?
      assert_equal(pid, status.pid)
      assert_predicate(status, :exited?)
      assert_equal(6, status.exitstatus)
      assert_equal("hihi pid=#{status.pid} ppid=#{$$}", File.read("result"))
    }
  end

  def test_popen_wordsplit
    with_tmpchdir {|d|
      write_file("script", <<-'End')
        print "fufu pid=#{$$} ppid=#{Process.ppid}"
        exit 7
      End
      str = "#{RUBY} script"
      io = IO.popen(str)
      pid = io.pid
      result = io.read
      io.close
      status = $?
      assert_equal(pid, status.pid)
      assert_predicate(status, :exited?)
      assert_equal(7, status.exitstatus)
      assert_equal("fufu pid=#{status.pid} ppid=#{$$}", result)
    }
  end

  def test_popen_wordsplit_beginning_and_trailing_spaces
    with_tmpchdir {|d|
      write_file("script", <<-'End')
        print "fufumm pid=#{$$} ppid=#{Process.ppid}"
        exit 7
      End
      str = " #{RUBY} script "
      io = IO.popen(str)
      pid = io.pid
      result = io.read
      io.close
      status = $?
      assert_equal(pid, status.pid)
      assert_predicate(status, :exited?)
      assert_equal(7, status.exitstatus)
      assert_equal("fufumm pid=#{status.pid} ppid=#{$$}", result)
    }
  end

  def test_exec_wordsplit
    with_tmpchdir {|d|
      write_file("script", <<-'End')
        File.open("result", "w") {|t|
          if /mswin|bccwin|mingw/ =~ RUBY_PLATFORM
            t << "hehe ppid=#{Process.ppid}"
          else
            t << "hehe pid=#{$$} ppid=#{Process.ppid}"
          end
        }
        exit 6
      End
      write_file("s", <<-"End")
	ruby = #{RUBY.dump}
	exec "\#{ruby} script"
      End
      pid = spawn(RUBY, "s")
      Process.wait pid
      status = $?
      assert_equal(pid, status.pid)
      assert_predicate(status, :exited?)
      assert_equal(6, status.exitstatus)
      if windows?
        expected = "hehe ppid=#{status.pid}"
      else
        expected = "hehe pid=#{status.pid} ppid=#{$$}"
      end
      assert_equal(expected, File.read("result"))
    }
  end

  def test_system_shell
    with_tmpchdir {|d|
      write_file("script1", <<-'End')
        File.open("result1", "w") {|t| t << "taka pid=#{$$} ppid=#{Process.ppid}" }
        exit 7
      End
      write_file("script2", <<-'End')
        File.open("result2", "w") {|t| t << "taki pid=#{$$} ppid=#{Process.ppid}" }
        exit 8
      End
      ret = system("#{RUBY} script1 || #{RUBY} script2")
      status = $?
      assert_equal(false, ret)
      assert_predicate(status, :exited?)
      result1 = File.read("result1")
      result2 = File.read("result2")
      assert_match(/\Ataka pid=\d+ ppid=\d+\z/, result1)
      assert_match(/\Ataki pid=\d+ ppid=\d+\z/, result2)
      assert_not_equal(result1[/\d+/].to_i, status.pid)

      if windows?
        Dir.mkdir(path = "path with space")
        write_file(bat = path + "/bat test.bat", "@echo %1>out")
        system(bat, "foo 'bar'")
        assert_equal(%["foo 'bar'"\n], File.read("out"), '[ruby-core:22960]')
        system(%[#{bat.dump} "foo 'bar'"])
        assert_equal(%["foo 'bar'"\n], File.read("out"), '[ruby-core:22960]')
      end
    }
  end

  def test_spawn_shell
    with_tmpchdir {|d|
      write_file("script1", <<-'End')
        File.open("result1", "w") {|t| t << "taku pid=#{$$} ppid=#{Process.ppid}" }
        exit 7
      End
      write_file("script2", <<-'End')
        File.open("result2", "w") {|t| t << "take pid=#{$$} ppid=#{Process.ppid}" }
        exit 8
      End
      pid = spawn("#{RUBY} script1 || #{RUBY} script2")
      Process.wait pid
      status = $?
      assert_predicate(status, :exited?)
      assert_not_predicate(status, :success?)
      result1 = File.read("result1")
      result2 = File.read("result2")
      assert_match(/\Ataku pid=\d+ ppid=\d+\z/, result1)
      assert_match(/\Atake pid=\d+ ppid=\d+\z/, result2)
      assert_not_equal(result1[/\d+/].to_i, status.pid)

      if windows?
        Dir.mkdir(path = "path with space")
        write_file(bat = path + "/bat test.bat", "@echo %1>out")
        pid = spawn(bat, "foo 'bar'")
        Process.wait pid
        status = $?
        assert_predicate(status, :exited?)
        assert_predicate(status, :success?)
        assert_equal(%["foo 'bar'"\n], File.read("out"), '[ruby-core:22960]')
        pid = spawn(%[#{bat.dump} "foo 'bar'"])
        Process.wait pid
        status = $?
        assert_predicate(status, :exited?)
        assert_predicate(status, :success?)
        assert_equal(%["foo 'bar'"\n], File.read("out"), '[ruby-core:22960]')
      end
    }
  end

  def test_popen_shell
    with_tmpchdir {|d|
      write_file("script1", <<-'End')
        puts "tako pid=#{$$} ppid=#{Process.ppid}"
        exit 7
      End
      write_file("script2", <<-'End')
        puts "tika pid=#{$$} ppid=#{Process.ppid}"
        exit 8
      End
      io = IO.popen("#{RUBY} script1 || #{RUBY} script2")
      result = io.read
      io.close
      status = $?
      assert_predicate(status, :exited?)
      assert_not_predicate(status, :success?)
      assert_match(/\Atako pid=\d+ ppid=\d+\ntika pid=\d+ ppid=\d+\n\z/, result)
      assert_not_equal(result[/\d+/].to_i, status.pid)

      if windows?
        Dir.mkdir(path = "path with space")
        write_file(bat = path + "/bat test.bat", "@echo %1")
        r = IO.popen([bat, "foo 'bar'"]) {|f| f.read}
        assert_equal(%["foo 'bar'"\n], r, '[ruby-core:22960]')
        r = IO.popen(%[#{bat.dump} "foo 'bar'"]) {|f| f.read}
        assert_equal(%["foo 'bar'"\n], r, '[ruby-core:22960]')
      end
    }
  end

  def test_exec_shell
    with_tmpchdir {|d|
      write_file("script1", <<-'End')
        File.open("result1", "w") {|t| t << "tiki pid=#{$$} ppid=#{Process.ppid}" }
        exit 7
      End
      write_file("script2", <<-'End')
        File.open("result2", "w") {|t| t << "tiku pid=#{$$} ppid=#{Process.ppid}" }
        exit 8
      End
      write_file("s", <<-"End")
	ruby = #{RUBY.dump}
	exec("\#{ruby} script1 || \#{ruby} script2")
      End
      pid = spawn RUBY, "s"
      Process.wait pid
      status = $?
      assert_predicate(status, :exited?)
      assert_not_predicate(status, :success?)
      result1 = File.read("result1")
      result2 = File.read("result2")
      assert_match(/\Atiki pid=\d+ ppid=\d+\z/, result1)
      assert_match(/\Atiku pid=\d+ ppid=\d+\z/, result2)
      assert_not_equal(result1[/\d+/].to_i, status.pid)
    }
  end

  def test_argv0
    with_tmpchdir {|d|
      assert_equal(false, system([RUBY, "asdfg"], "-e", "exit false"))
      assert_equal(true, system([RUBY, "zxcvb"], "-e", "exit true"))

      Process.wait spawn([RUBY, "poiu"], "-e", "exit 4")
      assert_equal(4, $?.exitstatus)

      assert_equal("1", IO.popen([[RUBY, "qwerty"], "-e", "print 1"]) {|f| f.read })

      write_file("s", <<-"End")
        exec([#{RUBY.dump}, "lkjh"], "-e", "exit 5")
      End
      pid = spawn RUBY, "s"
      Process.wait pid
      assert_equal(5, $?.exitstatus)
    }
  end

  def with_stdin(filename)
    open(filename) {|f|
      begin
        old = STDIN.dup
        begin
          STDIN.reopen(filename)
          yield
        ensure
          STDIN.reopen(old)
        end
      ensure
        old.close
      end
    }
  end

  def test_argv0_noarg
    with_tmpchdir {|d|
      open("t", "w") {|f| f.print "exit true" }
      open("f", "w") {|f| f.print "exit false" }

      with_stdin("t") { assert_equal(true, system([RUBY, "qaz"])) }
      with_stdin("f") { assert_equal(false, system([RUBY, "wsx"])) }

      with_stdin("t") { Process.wait spawn([RUBY, "edc"]) }
      assert_predicate($?, :success?)
      with_stdin("f") { Process.wait spawn([RUBY, "rfv"]) }
      assert_not_predicate($?, :success?)

      with_stdin("t") { IO.popen([[RUBY, "tgb"]]) {|io| assert_equal("", io.read) } }
      assert_predicate($?, :success?)
      with_stdin("f") { IO.popen([[RUBY, "yhn"]]) {|io| assert_equal("", io.read) } }
      assert_not_predicate($?, :success?)

      status = run_in_child "STDIN.reopen('t'); exec([#{RUBY.dump}, 'ujm'])"
      assert_predicate(status, :success?)
      status = run_in_child "STDIN.reopen('f'); exec([#{RUBY.dump}, 'ik,'])"
      assert_not_predicate(status, :success?)
    }
  end

  def test_argv0_keep_alive
    assert_in_out_err([], <<~REPRO, ['-'], [], "[Bug #15887]")
      $0 = "diverge"
      4.times { GC.start }
      puts Process.argv0
    REPRO
  end

  def test_status
    with_tmpchdir do
      s = run_in_child("exit 1")
      assert_equal("#<Process::Status: pid #{ s.pid } exit #{ s.exitstatus }>", s.inspect)

      assert_equal(s, s)
      assert_equal(s, s.to_i)

      assert_equal(s.to_i & 0x55555555, s & 0x55555555)
      assert_equal(s.to_i >> 1, s >> 1)
      assert_equal(false, s.stopped?)
      assert_equal(nil, s.stopsig)

      assert_equal(s, Marshal.load(Marshal.dump(s)))
    end
  end

  def test_status_kill
    return unless Process.respond_to?(:kill)
    return unless Signal.list.include?("KILL")

    # assume the system supports signal if SIGQUIT is available
    expected = Signal.list.include?("QUIT") ? [false, true, false, nil] : [true, false, false, true]

    with_tmpchdir do
      write_file("foo", "Process.kill(:KILL, $$); exit(42)")
      system(RUBY, "foo")
      s = $?
      assert_equal(expected,
                   [s.exited?, s.signaled?, s.stopped?, s.success?],
                   "[s.exited?, s.signaled?, s.stopped?, s.success?]")

      assert_equal(s, Marshal.load(Marshal.dump(s)))
    end
  end

  def test_status_quit
    return unless Process.respond_to?(:kill)
    return unless Signal.list.include?("QUIT")

    with_tmpchdir do
      s = assert_in_out_err([], "Signal.trap(:QUIT,'DEFAULT'); Process.kill(:SIGQUIT, $$);sleep 30", //, //, rlimit_core: 0)
      assert_equal([false, true, false, nil],
                   [s.exited?, s.signaled?, s.stopped?, s.success?],
                   "[s.exited?, s.signaled?, s.stopped?, s.success?]")
      assert_equal("#<Process::Status: pid #{ s.pid } SIGQUIT (signal #{ s.termsig })>",
                   s.inspect.sub(/ \(core dumped\)(?=>\z)/, ''))

      assert_equal(s, Marshal.load(Marshal.dump(s)))
    end
  end

  def test_status_fail
    ret = Process::Status.wait($$)
    assert_instance_of(Process::Status, ret)
    assert_equal(-1, ret.pid)
  end


  def test_status_wait
    IO.popen([RUBY, "-e", "gets"], "w") do |io|
      pid = io.pid
      assert_nil(Process::Status.wait(pid, Process::WNOHANG))
      io.puts
      ret = Process::Status.wait(pid)
      assert_instance_of(Process::Status, ret)
      assert_equal(pid, ret.pid)
      assert_predicate(ret, :exited?)
    end
  end

  def test_wait_without_arg
    with_tmpchdir do
      write_file("foo", "sleep 0.1")
      pid = spawn(RUBY, "foo")
      assert_equal(pid, Process.wait)
    end
  end

  def test_wait2
    with_tmpchdir do
      write_file("foo", "sleep 0.1")
      pid = spawn(RUBY, "foo")
      assert_equal([pid, 0], Process.wait2)
    end
  end

  def test_waitall
    with_tmpchdir do
      write_file("foo", "sleep 0.1")
      ps = (0...3).map { spawn(RUBY, "foo") }.sort
      ss = Process.waitall.sort
      ps.zip(ss) do |p1, (p2, s)|
        assert_equal(p1, p2)
        assert_equal(p1, s.pid)
      end
    end
  end

  def test_wait_exception
    bug11340 = '[ruby-dev:49176] [Bug #11340]'
    t0 = t1 = nil
    sec = 3
    code = "puts;STDOUT.flush;Thread.start{gets;exit};sleep(#{sec})"
    IO.popen([RUBY, '-e', code], 'r+') do |f|
      pid = f.pid
      f.gets
      t0 = Time.now
      th = Thread.start(Thread.current) do |main|
        Thread.pass until main.stop?
        main.raise Interrupt
      end
      begin
        assert_raise(Interrupt) {Process.wait(pid)}
      ensure
        th.kill.join
      end
      t1 = Time.now
      diff = t1 - t0
      assert_operator(diff, :<, sec,
                  ->{"#{bug11340}: #{diff} seconds to interrupt Process.wait"})
      f.puts
    end
  end

  def test_abort
    with_tmpchdir do
      s = run_in_child("abort")
      assert_not_predicate(s, :success?)
      write_file("test-script", "#{<<~"begin;"}\n#{<<~'end;'}")
      begin;
        STDERR.reopen(STDOUT)
        begin
          raise "[Bug #16424]"
        rescue
          abort
        end
      end;
      assert_include(IO.popen([RUBY, "test-script"], &:read), "[Bug #16424]")
    end
  end

  def test_sleep
    assert_raise(ArgumentError) { sleep(1, 1) }
    [-1, -1.0, -1r].each do |sec|
      assert_raise_with_message(ArgumentError, /not.*negative/) { sleep(sec) }
    end
  end

  def test_getpgid
    assert_kind_of(Integer, Process.getpgid(Process.ppid))
  rescue NotImplementedError
  end

  def test_getpriority
    assert_kind_of(Integer, Process.getpriority(Process::PRIO_PROCESS, $$))
  rescue NameError, NotImplementedError
  end

  def test_setpriority
    if defined? Process::PRIO_USER
      assert_nothing_raised do
        pr = Process.getpriority(Process::PRIO_PROCESS, $$)
        Process.setpriority(Process::PRIO_PROCESS, $$, pr)
      end
    end
  end

  def test_getuid
    assert_kind_of(Integer, Process.uid)
  end

  def test_groups
    gs = Process.groups
    assert_instance_of(Array, gs)
    gs.each {|g| assert_kind_of(Integer, g) }
  rescue NotImplementedError
  end

  def test_maxgroups
    max = Process.maxgroups
  rescue NotImplementedError
  else
    assert_kind_of(Integer, max)
    assert_predicate(max, :positive?)
    skip "not limited to NGROUPS_MAX" if /darwin/ =~ RUBY_PLATFORM
    gs = Process.groups
    assert_operator(gs.size, :<=, max)
    gs[0] ||= 0
    assert_raise(ArgumentError) {Process.groups = gs * (max / gs.size + 1)}
  end

  def test_geteuid
    assert_kind_of(Integer, Process.euid)
  end

  def test_seteuid
    assert_nothing_raised(TypeError) {Process.euid += 0}
  rescue NotImplementedError
  end

  def test_seteuid_name
    user = (Etc.getpwuid(Process.euid).name rescue ENV["USER"]) or return
    assert_nothing_raised(TypeError) {Process.