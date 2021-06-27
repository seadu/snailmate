# frozen_string_literal: true
require 'reline'
require 'ripper'
require 'irb/ruby-lex'

module IRB # :nodoc:
  module Color
    CLEAR     = 0
    BOLD      = 1
    UNDERLINE = 4
    REVERSE   = 7
    RED       = 31
    GREEN     = 32
    YELLOW    = 33
    BLUE      = 34
    MAGENTA   = 35
    CYAN      = 36

    TOKEN_KEYWORDS = {
      on_kw: ['nil', 'self', 'true', 'false', '__FILE__', '__LINE__', '__ENCODING__'],
      on_const: ['ENV'],
    }
    private_constant :TOKEN_KEYWORDS

    # A constant of all-bit 1 to match any Ripper's state in #dispatch_seq
    ALL = -1
    private_constant :ALL

    begin
      # Following pry's colors where possible, but sometimes having a compromise like making
      # backtick and regexp as red (string's color, because they're sharing tokens).
      TOKEN_SEQ_EXPRS = {
        on_CHAR:            [[BLUE, BOLD],            ALL],
        on_backtick:        [[RED, BOLD],             ALL],
        on_comment:         [[BLUE, BOLD],            ALL],
      