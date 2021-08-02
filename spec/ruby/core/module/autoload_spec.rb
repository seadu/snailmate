require_relative '../../spec_helper'
require_relative '../../fixtures/code_loading'
require_relative 'fixtures/classes'
require 'thread'

describe "Module#autoload?" do
  it "returns the name of the file that will be autoloaded" do
    ModuleSpecs::Autoload.autoload :Autoload, "autoload.rb"
    ModuleSpecs::Autoload.autoload?(:Autoload).should == "autoload.rb"
  end

  it "returns nil if no file has been registered for a constant" do
    ModuleSpecs::Autoload.autoload?(:Manualload).should be_nil
  end

  it "returns the name of the file that will be autoloaded if an ancestor defined that autoload" do
    ModuleSpecs::Autoload::Parent.autoload :AnotherAutoload, "another_autoload.rb"
    ModuleSpecs::Autoload::Child.autoload?(:AnotherAutoload).should == "another_autoload.rb"
  end

  it "returns nil if an ancestor defined that autoload but recursion is disabled" do
    ModuleSpecs::Autoload::Parent.autoload :InheritedAutoload, "inherited_autoload.rb"
    ModuleSpecs::Autoload::Child.autoload?(:InheritedAutoload, false).should be_nil
  end

  it "returns the name of the file that will be loaded if recursion is disabled but the autoload is defined on the class itself" do
    ModuleSpecs::Autoload::Child.autoload :ChildAutoload, "child_autoload.rb"
    ModuleSpecs::Autoload::Child.autoload?(:ChildAutoload, false).should == "child_autoload.rb"
  end
end

describe "Module#autoload" do
  before :all do
    @non_existent = fixture __FILE__, "no_autoload.rb"
    CodeLoadingSpecs.preload_rubygems
  end

  before :each do
    @loaded_features = $".dup

    ScratchPad.clear
    @remove = []
  end

  after :each do
    $".replace @loaded_features
    @remove.each { |const|
      ModuleSpecs::Autoload.send :remove_const, const
    }
  end

  it "registers a file to load the first time the named constant is accessed" do
    ModuleSpecs::Autoload.autoload :A, @non_existent
    ModuleSpecs::Autoload.autoload?(:A).should == @non_existent
  end

  it "sets the autoload constant in the constants table" do
    ModuleSpecs::Autoload.autoload :B, @non_existent
    ModuleSpecs::Autoload.should have_constant(:B)
  end

  it "can be overridden with a second autoload on the same constant" do
    ModuleSpecs::Autoload.autoload :Overridden, @non_existent
    @remove << :Overridden
    ModuleSpecs::Autoload.autoload?(:Overridden).should == @non_existent

    path = fixture(__FILE__, "autoload_overridden.rb")
    ModuleSpecs::Autoload.autoload :Overridden, path
    ModuleSpecs::Autoload.autoload?(:Overridden).should == path

    ModuleSpecs::Autoload::Overridden.should == :overridden
  end

  it "loads the registered constant when it is accessed" do
    ModuleSpecs::Autoload.should_not have_constant(:X)
    ModuleSpecs::Autoload.autoload :X, fixture(__FILE__, "autoload_x.rb")
    @remove << :X
    ModuleSpecs::Autoload::X.should == :x
  end

  it "loads the registered constant into a dynamically created class" do
    cls = Class.new { autoload :C, fixture(__FILE__, "autoload_c.rb") }
    ModuleSpecs::Autoload::DynClass = cls
    @remove << :DynClass

    ScratchPad.recorded.should be_nil
    ModuleSpecs::Autoload::DynClass::C.new.loaded.should == :dynclass_c
    ScratchPad.recorded.should == :loaded
  end

  it "loads the registered constant into a dynamically created module" do
    mod = Module.new { autoload :D, fixture(__FILE__, "autoload_d.rb") }
    ModuleSpecs::Autoload::DynModule = mod
    @remove << :DynModule

    ScratchPad.recorded.should be_nil
    ModuleSpecs::Autoload::DynModule::D.new.loaded.should == :dynmodule_d
    ScratchPad.recorded.should == :loaded
  end

  it "loads the registered constant when it is opened as a class" do
    ModuleSpecs::Autoload.autoload :E, fixture(__FILE__, "autoload_e.rb")
    class ModuleSpecs::Autoload::E
    end
    ModuleSpecs::Autoload::E.new.loaded.should == :autoload_e
  end

  it "loads the registered constant when it is opened as a module" do
    ModuleSpecs::Autoload.autoload :F, fixture(__FILE__, "autoload_f.rb")
    module ModuleSpecs::Autoload::F
    end
    ModuleSpecs::Autoload::F.loaded.should == :autoload_f
  end

  it "loads the registered constant when it is inherited from" do
    ModuleSpecs::Autoload.autoload :G, fixture(__FILE__, "autoload_g.rb")
    class ModuleSpecs::Autoload::Gsub < ModuleSpecs::Autoload::G
    end
    ModuleSpecs::Autoload::Gsub.new.loaded.should == :autoload_g
  end

  it "loads the registered constant when it is included" do
    ModuleSpecs::Autoload.autoload :H, fixture(__FILE__, "autoload_h.rb")
    class ModuleSpecs::Autoload::HClass
      include ModuleSpecs::Autoload::H
    end
    ModuleSpecs::Autoload::HClass.new.loaded.should == :autoload_h
  end

  it "does not load the file when the constant is already set" do
    ModuleSpecs::Autoload.autoload :I, fixture(__FILE__, "autoload_i.rb")
    @remove << :I
    ModuleSpecs::Autoload.const_set :I, 3
    ModuleSpecs::Autoload::I.should == 3
    ScratchPad.recorded.should be_nil
  end

  it "loads a file with .rb extension when passed the name without the extension" do
    ModuleSpecs::Autoload.autoload :J, fixture(__FILE__, "autoload_j")
    ModuleSpecs::Autoload::J.should == :autoload_j
  end

  it "calls main.require(path) to load the file" do
    ModuleSpecs::Autoload.autoload :ModuleAutoloadCallsRequire, "module_autoload_not_exist.rb"
    main = TOPLEVEL_BINDING.eval("self")
    main.should_receive(:require).with("module_autoload_not_exist.rb")
    # The constant won't be defined since require is mocked to do nothing
    -> { ModuleSpecs::Autoload::ModuleAutoloadCallsRequire }.should raise_error(NameError)
  end

  it "does not load the file if the file is manually required" do
    filename = fixture(__FILE__, "autoload_k.rb")
    ModuleSpecs::Autoload.autoload :KHash, filename
    @remove << :KHash

    require filename
    ScratchPad.recorded.should == :loaded
    ScratchPad.clear

    ModuleSpecs::Autoload::KHash.should be_kind_of(Class)
    ModuleSpecs::Autoload::KH