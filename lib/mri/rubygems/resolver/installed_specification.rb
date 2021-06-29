# frozen_string_literal: true
##
# An InstalledSpecification represents a gem that is already installed
# locally.

class Gem::Resolver::InstalledSpecification < Gem::Resolver::SpecSpecification
  def ==(other) # :nodoc:
    self.class === other &&
      @set  == other.set &&
      @spec == other.spec
  end

  ##
  # This is a null install as this specification 