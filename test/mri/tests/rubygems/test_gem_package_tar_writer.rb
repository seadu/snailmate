# frozen_string_literal: true
require_relative "package/tar_test_case"
require "rubygems/package/tar_writer"

class TestGemPackageTarWriter < Gem::Package::TarTestCase
  def setup
    super

    # Setting `@default_source_date_epoch` to `nil` effectively resets the
    # value used for `Gem.source_date_epoch` whenever `$SOURCE_DATE_EPOCH`
    # is not set.
    Gem.instance_variable_set(:'@default_source_date_epoch', nil)

    @data = "abcde12345"
    @io = TempIO.new
    @tar_writer = Gem::Package::TarWriter.new @io
    @epoch = ENV["SOURCE_DATE_EPOCH"]
    ENV["SOURCE_DATE_EPOCH"] = nil
  end

  def teardown
    ENV["SOURCE_DATE_EPOCH"] = @epoch
    @tar_writer.close unless @tar_writer.closed?
    @io.close!

    super
  end

  def test_add_file
    Time.stub :now, Time.at(1458518157) do
      @tar_writer.add_file "x", 0644 do |f|
        f.write "a" * 10
      end

      assert_headers_equal(tar_file_header("x", "", 0644, 10, Time.now),
                         @io.string[0, 512])
    end
    assert_equal "aaaaaaaaaa#{"\0" * 502}", @io.string[512, 512]
    assert_equal 1024, @io.pos
  end

  def test_add_file_source_date_epoch
    ENV["SOURCE_DATE_EPOCH"] = "123456789"
    Time.stub :now, Time.at(1458518157) do
      @tar_writer.mkdir "foo", 0644

      assert_headers_equal tar_dir_header("foo", "", 0644, Time.at(ENV["SOURCE_DATE_EPOCH"].to_i).utc),
                           @io.string[0, 512]
    end
  end

  def test_add_symlink
    Time.stub :now, Time.at(1458518157) do
      @tar_writer.add_symlink "x", "y", 0644

      assert_headers_equal(tar_symlink_header("x", "", 0644, Time.now, "y"),
                         @io.string[0, 512])
    end
    assert_equal 512, @io.pos
  end

  def test_add_symlink_source_date_epoch
    ENV["SOURCE_DATE_EPOCH"] = "123456789"
    Time.stub :now, Time.at(1458518157) do
      @tar_writer.add_symlink "x", "y", 0644

      assert_headers_equal(tar_symlink_header("x", "", 0644, Time.at(ENV["SOURCE_DATE_EPOCH"].to_i).utc, "y"),
                         @io.string[0, 512])
    end
  end

  def test_add_file_digest
    digest_algorithms = Digest::SHA1.new, Digest::SHA512.new

    Time.stub :now, Time.at(1458518157) do
      digests = @tar_writer.add_file_digest "x", 0644, digest_algorithms do |io|
        io.write "a" * 10
      end

      assert_equal "3495ff69d34671d1e15b33a63c1379fdedd3a32a",
                   digests["SHA1"].hexdigest
      assert_equal "4714870aff6c97ca09d135834fdb58a6389a50c1" \
                   "1fef8ec4afef466fb60a23ac6b7a9c92658f14df" \
                   "4993d6b40a4e4d8424196afc347e97640d68de61" \
                   "e1cf14b0",
                   digests["SHA512"].hexdigest

      assert_headers_equal(tar_file_header("x", "", 0644, 10, Time.now),
                         @io.string[0, 512])
    end
    assert_equal "aaaaaaaaaa#{"\0" * 502}", @io.string[512, 512]
    assert_equal 1024,