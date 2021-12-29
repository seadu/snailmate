# frozen_string_literal: true
require "rake"
require "rake/tasklib"

module Rake

  # Create a task that runs a set of tests.
  #
  # Example:
  #   require "rake/testtask"
  #
  #   Rake::TestTask.new do |t|
  #     t.libs << "test"
  #     t.test_files = FileList['test/test*.rb']
  #     t.verbose = true
  #   end
  #
  # If rake is invoked with a "TEST=filename" command line option,
  # then the list of test files will be overridden to include only the
  # filename specified on the command line.  This provides an easy way
  # to run just one test.
  #
  # If rake is invoked with a "TESTOPTS=options" command line option,
  # then the given options are passed to the test process after a
  # '--'.  This allows Test::Unit options to be passed to the test
  # suite.
  #
  # Examples:
  #
  #   rake test                           # run tests normally
  #   rake test TEST=just_one_file.rb     # run just one test file.
  #   rake test TESTOPTS="-v"             # run in verbose mode
  #   ra