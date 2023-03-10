
require_relative '../../spec_helper'

describe "Encoding.locale_charmap" do
  it "returns a String" do
    Encoding.locale_charmap.should be_an_instance_of(String)
  end

  # FIXME: Get this working on Windows
  platform_is :linux do
    platform_is_not :android do
      it "returns a value based on the LC_ALL environment variable" do
        old_lc_all = ENV['LC_ALL']
        ENV['LC_ALL'] = 'C'
        ruby_exe("print Encoding.locale_charmap").should == 'ANSI_X3.4-1968'
        ENV['LC_ALL'] = old_lc_all
      end
    end
  end

  platform_is :freebsd, :openbsd, :darwin do
    it "returns a value based on the LC_ALL environment variable" do
      old_lc_all = ENV['LC_ALL']
      ENV['LC_ALL'] = 'C'
      ruby_exe("print Encoding.locale_charmap").should == 'US-ASCII'
      ENV['LC_ALL'] = old_lc_all
    end
  end

  platform_is :netbsd do
    it "returns a value based on the LC_ALL environment variable" do
      old_lc_all = ENV['LC_ALL']
      ENV['LC_ALL'] = 'C'
      ruby_exe("print Encoding.locale_charmap").should == '646'
      ENV['LC_ALL'] = old_lc_all
    end
  end

  platform_is :android do
    it "always returns UTF-8" do
      old_lc_all = ENV['LC_ALL']
      ENV['LC_ALL'] = 'C'
      ruby_exe("print Encoding.locale_charmap").should == 'UTF-8'
      ENV['LC_ALL'] = old_lc_all
    end
  end

  platform_is :bsd, :darwin, :linux do
    it "is unaffected by assigning to ENV['LC_ALL'] in the same process" do
      old_charmap = Encoding.locale_charmap
      old_lc_all = ENV['LC_ALL']
      ENV['LC_ALL'] = 'C'
      Encoding.locale_charmap.should == old_charmap
      ENV['LC_ALL'] = old_lc_all
    end
  end
end