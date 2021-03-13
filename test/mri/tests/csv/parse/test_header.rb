# -*- coding: utf-8 -*-
# frozen_string_literal: false

require_relative "../helper"

class TestCSVHeaders < Test::Unit::TestCase
  extend DifferentOFS

  def setup
    super
    @data = <<-CSV
first,second,third
A,B,C
1,2,3
    CSV
  end

  def test_first_row
    [:first_row, true].each do |setting|  # two names for the same setting
      # activate headers
      csv = nil
      assert_nothing_raised(Exception) do
        csv = CSV.parse(@data, headers: setting)
      end

      # first data row - skipping headers
      row = csv[0]
      assert_not_nil(row)
      assert_instance_of(CSV::Row, row)
      assert_equal([%w{first A}, %w{second B}, %w{third C}], row.to_a)

      # second data row
      row = csv[1]
      assert_not_nil(row)
      assert_instance_of(CSV::Row, row)
      assert_equal([%w{first 1}, %w{second 2}, %w{third 3}], row.to_a)

      # empty
      assert_nil(csv[2])
    end
  end

  def test_array_of_headers
    # activate headers
    csv = nil
    assert_nothing_raised(Exception) do
      csv = CSV.parse(@data, headers: [:my, :new, :headers])
    end

    # first data row - skipping headers
    row = csv[0]
    assert_not_nil(row)
    assert_instance_of(CSV::Row, row)
    assert_equal( [[:my, "first"], [:new, "second"], [:headers, "third"]],
                  row.to_a )

    # second data row
    row = csv[1]
    assert_not_nil(row)
    assert_instance_of(CSV::Row, row)
    assert_equal([[:my, "A"], [:new, "B"], [:headers, "C"]], row.to_a)

    # third data row
    row = csv[2]
    assert_not_nil(row)
    assert_instance_of(CSV::Row, row)
    assert_equal([[:my, "1"], [:new, "2"], [:headers, "3"]], row.to_a)

    # empty
    assert_nil(csv[3])

    # with return and convert
    assert_nothing_raised(Exception) do
      csv = CSV.parse( @data, headers:           [:my, :new, :headers],
                              return_headers:    true,
                              header_converters: lambda { |h| h.to_s } )
    end
    row = csv[0]
    assert_not_nil(row)
    assert_instance_of(CSV::Row, row)
    assert_equal([["my", :my], ["new", :new], ["headers", :headers]], row.to_a)
    assert_predicate(row, :header_row?)
    assert_not_predicate(row, :field_row?)
  end

  def test_csv_header_string
    # activate headers
    csv = nil
    assert_nothing_raised(Exception) do
      csv = CSV.parse(@data, headers: "my,new,headers")
    end

    # first data row - skipping headers
    row = csv[0]
    assert_not_nil(row)
    assert_instance_of(CSV::Row, row)
    assert_equal([%w{my first}, %w{new second}, %w{headers third}], row.to_a)

    # second data row
    row = csv[1]
    assert_not_nil(row)
    assert_instance_of(CSV::Row, row)
    assert_equal([%w{my A}, %w{new B}, %w{headers C}], row.to_a)

    # third data row
    row = csv[2]
    assert_not_nil(row)
    assert_instance_of(CSV::Row, row)