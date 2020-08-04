# frozen_string_literal: true

module Bundler
  class Source
    class Metadata < Source
      def specs
        @specs ||= Index.build do |idx|
          idx << Gem::Specification.new("Ruby\0", Gem.ruby_version)
          idx << Gem::Specification.new("RubyGems\0", Gem::VERSION) do |s|
            s.required_rubygems_version = Gem::Requirement.default
          end

          idx << Gem::Specification.new do |s|
            s.name     = "bundler"
            s.version  = VERSION
            s.license  = "MIT"
            s.platform = Gem::Platform::RUBY
            s.source   = self
            s.authors  = ["bundler team"]
            s.bindir   = "exe"
            s.homepage = "https://bundler.io"
            s.summary  = "The best way to manage your application's dependencies"
            s.executables = %w[bundle]
            # can't point to the actual gemspec or else the require paths will be wrong
            s.loaded_from = __dir__
          end

          if local_spec = Bundler.rubygems.find_bundler(VERSION)
            idx << local_spec
          end

