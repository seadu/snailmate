# frozen_string_literal: true
##
# Represents a possible Specification object returned from IndexSet.  Used to
# delay needed to download full Specification objects when only the +name+
# and +version+ are needed.

class Gem::Resolver::IndexSpecification < Gem::Resolver::Specification
  ##
  # An IndexSpecification is created from the index format described in `gem
  # help generate_index`.
  #
  # The +set+ contains other speci