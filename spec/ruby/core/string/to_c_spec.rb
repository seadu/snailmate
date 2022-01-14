require_relative '../../spec_helper'
require_relative '../../shared/kernel/complex'
require_relative 'fixtures/to_c'

describe "String#to_c" do
  it_behaves_like :kernel_complex, :to_c_method, StringSpecs
end

describe "String#to_c" do
  it "returns a complex number with 0 as the real part, 0 as the imaginary part for unrecognised Strings" do
    'ruby'.to_c.should == Complex(0, 0)
  end

  it "ignores trailing garbage" do
    '79+4iruby'.to_c.should == Complex(79, 4)
    ruby_bug "[Bug #19087]", ""..."3.2" do
      '7__9+4__0i'.to_c.should == Complex(7, 0)
    end
  end

  it "understands Float::INFINITY" do