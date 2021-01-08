# frozen_string_literal: true
require_relative "user_interaction"

##
# This Gem::StreamUI subclass records input and output to StringIO for
# retrieval during tests.

class Gem::MockGemUi < Gem::StreamUI
  ##
  # Raised when you haven't provided enough input to your MockGemUi

  class InputEOFError < RuntimeError
    def initialize(question)
      super "Out of input for MockGemUi on #{question.inspect}"
    end
  end

  class TermError < RuntimeError
    attr_reader :exit_code

    def initialize(exit_code)
      super
      @exit_code = exit_code
    end
  end
  class SystemExitException < RuntimeError; end

  module TTY

    attr_accessor :tty

    def tty?()
      @tty = true unless defined?(@tty)
      @tty
    end

    def noecho
      yield self
    end
  end

  def initialize(input = "")
    re