# frozen_string_literal: true

module Bundler
  # Manages which plugins are installed and their sources. This also is supposed to map
  # which plugin does what (currently the features are not implemented so this class is
  # now a stub class).
  module Plugin
    class Index
      class CommandConflict < PluginError
        def initialize(plugin, commands)
          msg = "Command(s) `#{commands.join("`, `")}` declared by #{plugin} are already registered."
          super msg
        end
      end

      class SourceConflict < PluginError
        def initialize(plugin, sources)
          msg = "Source(s) `#{sources.join("`, `")}` declared by #{plugin} are already registered."
          super msg
        end
      end

      attr_reader :commands

      def initialize
        @plugin_paths = {}
        @commands = {}
        @sources = {}
        @hooks = {}
        @load_paths = {}

        begin
          load_index(global_index_file, true)
        rescue GenericSystemCallError
          # no need to fail when on a read-only FS, for example
          nil
        end
        load_index(local_index_file) if SharedHelpers.in_bundle?
      end

      # This function is to be called when a new plugin is installed. This
      # function shall add the functions of the plugin to existing maps and also
      # the name to source location.
      #
      # @param [String] name of the plugin to be registered
      # @param [String] path where the plugin is installed
      # @param [Array<String>] load_paths for the plugin
      # @param [Array<String>] commands that are handled by the plugin
      # @param [Array<String>] sources that are handled by the plugin
      def register_plugin(name, path, load_paths, commands, sources, hooks)
        old_commands = @commands.dup

        common = commands & @commands.keys
        raise CommandConflict.new(name, common) unless common.empty?
        commands.each {|c| @commands[c] = name }

        common = sources & @sources.keys
        raise SourceConflict.new(name, common) unless common.empty?
        sources.each {|k| @sources[k] = name }

        hooks.each do |event|
          event_hooks = (@hooks[event] ||= []) << name
          event_hooks.uniq!
        end

        @plugin_paths[name] = path
        @load_paths[name] = load_paths
        save_index
      rescue StandardError
        @commands = old_commands
        raise
     