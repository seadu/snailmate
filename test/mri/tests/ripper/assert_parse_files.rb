
# frozen_string_literal: true
require 'test/unit'

module TestRipper; end
class TestRipper::Generic < Test::Unit::TestCase
  # Changed from MRI to reflect the different relative path of the test suite.
  SRCDIR = File.expand_path("../../../../..", __FILE__)

  def assert_parse_files(dir, pattern = "**/*.rb")
    assert_separately(%W[--disable-gem -rripper - #{SRCDIR}/#{dir} #{pattern}],
                      __FILE__, __LINE__, "#{<<-"begin;"}\n#{<<-'end;'}", timeout: Float::INFINITY)
    pattern = "#{pattern}"
    begin;
      TEST_RATIO = ENV["TEST_RIPPER_RATIO"]&.tap {|s|break s.to_f} || 0.05 # testing all files needs too long time...
      class Parser < Ripper
        PARSER_EVENTS.each {|n| eval "def on_#{n}(*args) r = [:#{n}, *args]; r.inspect; Object.new end" }
        SCANNER_EVENTS.each {|n| eval "def on_#{n}(*args) r = [:#{n}, *args]; r.inspect; Object.new end" }
      end
      dir = ARGV.shift
      scripts = Dir.chdir(dir) {Dir[pattern]}
      if (1...scripts.size).include?(num = scripts.size * TEST_RATIO)
        scripts = scripts.sample(num)
      end
      scripts.sort!
      for script in scripts
        assert_nothing_raised {
          parser = Parser.new(File.read("#{dir}/#{script}"), script)
          parser.instance_eval "parse", "<#{script}>"
        }
      end
    end;
  end
end