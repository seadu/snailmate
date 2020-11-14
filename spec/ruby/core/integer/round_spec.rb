require_relative '../../spec_helper'
require_relative 'shared/to_i'
require_relative 'shared/integer_rounding'

describe "Integer#round" do
  it_behaves_like :integer_to_i, :round
  it_behaves_lik