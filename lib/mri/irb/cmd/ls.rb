# frozen_string_literal: true

require "reline"
require_relative "nop"
require_relative "../color"

# :stopdoc:
module IRB
  module ExtendCommand
    class Ls < Nop
      def execute(*arg, grep: nil)
        o = Output.new(grep: grep)

        obj    = arg.empty? ? irb_context.workspace.main : arg.first
        locals = arg.empty? ? irb_context.workspace.binding.local_variables : []
        klass  = (obj.class == Class || obj.class == Module ? obj : obj.class)

        o.dump("constants", obj.constants) if obj.respond_to?(:constants)
        dump_methods(o, klass, obj)
        o.dump("instance variables", obj.instance_variables)
        o.dump("class variables", klass.class_variables)
        o.dump("locals", locals)
      end

      def dump_methods(o, klass, obj)
        singleton_class = begin obj.singleton_class; rescue TypeError; nil end
        maps = class_method_map((singleton_class || klass).ancestors)
        maps.each do |mod, methods|
          name = mod == singleton_class ? "#{klass}.methods" : "#{mod}#methods"
          o.dump(name, methods)
        end
      end

      def class_method_map(classes)
        dumped = Array.