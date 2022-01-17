require_relative '../../spec_helper'
require_relative 'fixtures/common'

describe "Proc.new with an associated block" do
  it "returns a proc that represents the block" do
    Proc.new { }.call.should == nil
    Proc.new { "hello" }.call.should == "hello"
  end

  describe "called on a subclass of Proc" do
    before :each do
      @subclass = Class.new(Proc) do
        attr_reader :ok
        def initialize
          @ok = true
          super
        end
      end
    end

    it "returns an instance of the subclass" do
      proc = @subclass.new {"hello"}

      proc.class.should == @subclass
      proc.call.should == "hello"
      proc.ok.should == true
    end

    # JRUBY-5026
    describe "using a reified block parameter" do
      it "returns an instance of the subclass" do
        cls = Class.new do
          def self.subclass=(subclass)
            @subclass = subclass
          end
          def self.foo(&block)
            @subclass.new(&block)
          end
        end
        cls.subclass = @subclass
        proc = cls.foo {"hello"}

        proc.class.should == @subclass
        proc.call.should == "hello"
        proc.ok.should == true
      end
    end
  end

  # JRUBY-5261; Proc sets up the block during .new, not in #initialize
  describe "called on a subclass of Proc that does not 'super' in 'initialize'" do
    before :each do
      @subclass = Class.new(Proc) do
        attr_reader :ok
        def initialize
          @ok = true
        end
      end
    end

    it "still constructs a functional proc" do
      proc = @subclass.new {'ok'}
      proc.call.should == 'ok'
      proc.ok.should == true
    end
  end

  it "raise