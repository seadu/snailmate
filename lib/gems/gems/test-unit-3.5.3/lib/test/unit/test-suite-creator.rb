#--
#
# Author:: Kouhei Sutou
# Copyright::
#   * Copyright (c) 2011 Kouhei Sutou <tt><kou@clear-code.com></tt>
# License:: Ruby license.

require "test/unit/data-sets"

module Test
  module Unit
    class TestSuiteCreator # :nodoc:
      class << self
        def test_method?(test_case, method_name)
          /\Atest./ =~ method_name.to_s or
            test_case.find_attribute(method_name, :test)
        end
      end

      def initialize(test_case)
        @test_case = test_case
      end

      def create
        suite = TestSuite.new(@test_case.name, @test_case)
        collect_test_names.each do |test_name|
          data_sets = extract_data_sets(test_name)
          if data_sets
            data_sets.each do |label, data|
              append_test(suite, test_name) do |test|
                test.assign_test_data(label, data)
              end
            end
          else
            append_test(suite, test_name)
          end
        end
        append_test(suite, "default_test") if suite.empty?
        suite
      end

      private
      def extract_data_sets(test_name)
        data_sets = @test_case.find_attribute(test_name,
                                              :data,
                                              :recursive => false)
        data_method_name = "data_#{test_name}"
        test = @test_case.new(test_name)
        if test.respond_to?(data_method_name)
          