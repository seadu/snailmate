# frozen_string_literal: true
require 'test/unit'
require 'tempfile'

class TestTempfile < Test::Unit::TestCase
  def initialize(*)
    super
    @tempfile = nil
  end

  def tempfile(*args, **kw, &block)
    t = Tempfile.new(*args, **kw, &block)
    @tempfile = (t unless block)
  end

  def teardown
    if @tempfile
      @tempfile.close!
    end
  end

  def test_leackchecker
    assert_instance_of(Tempfile, Tempfile.allocate)
  end

  def test_basic
    t = tempfile("foo")
    path = t.path
    t.write("hello world")
    t.close
    assert_equal "hello world", File.read(path)
  end

  def test_saves_in_given_directory
    subdir = File.join(Dir.tmpdir, "tempfile-test-#{rand}")
    Dir.mkdir(subdir)
    begin
      tempfile = Tempfile.new("foo", subdir)
      tempfile.close
      begin
        assert_equal subdir, File.dirname(tempfile.path)
      ensure
        tempfile.unlink
      end
    ensure
      Dir.rmdir(subdir)
    end
  end

  def test_basename
    t = tempfile("foo")
    assert_match(/^foo/, File.basename(t.path))
  end

  def test_default_basename
    t = tempfile
    assert_file.exist?(t.path)
  end

  def test_basename_with_suffix
    t = tempfile(["foo", ".txt"])
    assert_match(/^foo/, File.basename(t.path))
    assert_match(/\.txt$/, File.basename(t.path))
  end

  def test_unlink
    t = tempfile("foo")
    path = t.path

    t.close
    assert_file.exist?(path)

    t.unlink
    assert_file.not_exist?(path)

    assert_nil t.path
  end

  def test_unlink_silently_fails_on_windows
    tempfile = tempfile("foo")
    path = tempfile.path
    begin
      assert_nothing_raised do
        tempfile.unlink
      end
    ensure
      tempfile.close
      File.unlink(path) if File.exist?(path)
    end
  end

  def test_unlink_before_close_works_on_posix_systems
    tempfile = tempfile("foo")
    begin
      path = tempfile.path
      tempfile.unlink
      assert_file.not_exist?(path)
      tempfile.write("hello ")
      tempfile.write("world\n")
      tempfile.rewind
      assert_equal "hello world\n", tempfile.read
    ensure
      tempfile.close
      tempfile.unlink
    end
  end unless /mswin|mingw/ =~ RUBY_PLATFORM

  def test_close_and_close_p
    t = tempfile("foo")
    assert_not_predicate(t, :closed?)
    t.close
    assert_predicate(t, :closed?)
  end

  def test_close_with_unlink_now_true_works
    t = tempfile("foo")
    path = t.path
    t.close(true)
    assert_predicate(t, :closed?)
    assert_nil t.path
    assert_file.not_exist?(path)
  end

  def test_close_with_unlink_now_true_does_not_unlink_if_already_unlinked
    t = tempfile("foo")
    path = t.path
    t.unlink
    File.open(path, "w").close
    begin
      t.close(true)
      assert_file.exist?(path)
    ensure
      File.unlink(path) rescue nil
    end
  end unless /mswin|mingw/ =~ RUBY_PL