# frozen_string_literal: true

module Bundler
  class CLI::Config < Thor
    class_option :parseable, :type => :boolean, :banner => "Use minimal formatting for more parseable output"

    def self.scope_options
      method_option :global, :type => :boolean, :banner => "Only change the global config"
      method_option :local, :type => :boolean, :banner => "Only change the local config"
    end
    private_class_method :scope_options

    desc "base NAME [VALUE]", "The Bundler 1 config interface", :hide => true
    scope_options
    method_option :delete, :type => :boolean, :banner => "delete"
    def base(name = nil, *value)
      new_args =
        if ARGV.size == 1
          ["config", "list"]
        elsif ARGV.include?("--delete")
          ARGV.map {|arg| arg == "--delete" ? "unset" : arg }
        elsif ARGV.include?("--global") || ARGV.include?("--local") || ARGV.size == 3
          ["config", "set", *ARGV[1..-1]]
        else
          ["config", "get", ARGV[1]]
        end

      SharedHelpers.major_deprecation 3,
        "Using the `config` command without a subcommand [list, get, set, unset] is deprecated and will be removed in the future. Use `bundle #{new_args.join(" ")}` instead."

      Base.new(options, name, value, self).run
    end

    desc "list", "List out all configured settings"
    def list
      Base.new(options, nil, nil, self).run
    end

    desc "get NAME", "Returns the value for the given key"
    def get(name)
      Base.new(options, name, nil, self).run
    end

    desc "set NAME VALUE", "Sets the given value for the given key"
    scope_options
    def set(name, value, *value_)
      Base.new(options, name, va