# frozen_string_literal: true
require_relative "helper"
require "rubygems/commands/sources_command"

class TestGemCommandsSourcesCommand < Gem::TestCase
  def setup
    super

    @cmd = Gem::Commands::SourcesCommand.new

    @new_repo = "http://beta-gems.example.com"

    @old_https_proxy_config = Gem.configuration[:http_proxy]
  end

  def teardown
    Gem.configuration[:http_proxy] = @old_https_proxy_config

    super
  end

  def test_initialize_proxy
    assert @cmd.handles?(["--http-proxy", "http://proxy.example.com"])
  end

  def test_execute
    @cmd.handle_options []

    use_ui @ui do
      @cmd.execute
    end

    expected = <<-EOF
*** CURRENT SOURCES ***

#{@gem_repo}
    EOF

    assert_equal expected, @ui.output
    assert_equal "", @ui.error
  end

  def test_execute_add
    spec_fetcher do |fetcher|
      fetcher.spec "a", 1
    end

    specs = Gem::Specification.map do |spec|
      [spec.name, spec.version, spec.original_platform]
    end

    specs_dump_gz = StringIO.new
    Zlib::GzipWriter.wrap specs_dump_gz do |io|
      Marshal.dump specs, io
    end

    @fetcher.data["#{@new_repo}/specs.#{@marshal_version}.gz"] =
      specs_dump_gz.string

    @cmd.handle_options %W[--add #{@new_repo}]

    use_ui @ui do
      @cmd.execute
    end

    assert_equal [@gem_repo, @new_repo], Gem.sources

    expected = <<-EOF
#{@new_repo} added to sources
    EOF

    assert_equal expected, @ui.output
    assert_equal "", @ui.error
  end

  def test_execute_add_allow_typo_squatting_source
    rubygems_org = "https://rubyems.org"

    spec_fetcher do |fetcher|
      fetcher.spec("a", 1)
    end

    specs = Gem::Specification.map do |spec|
      [spec.name, spec.version, spec.original_platform]
    end

    specs_dump_gz = StringIO.new
    Zlib::GzipWriter.wrap(specs_dump_gz) do |io|
      Marshal.dump(specs, io)
    end

    @fetcher.data["#{rubygems_org}/specs.#{@marshal_version}.gz"] =