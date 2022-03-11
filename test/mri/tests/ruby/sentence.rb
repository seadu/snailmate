
# frozen_string_literal: false
# == sentence library
#
# = Features
#
# * syntax based sentences generation
# * sentence operations such as substitution.
#
# = Example
#
# Some arithmetic expressions using "+", "-", "*" and "/" are generated as follows.
#
#  require 'sentence'
#  Sentence.each({
#    :exp => [["num"],
#             [:exp, "+", :exp],
#             [:exp, "-", :exp],
#             [:exp, "*", :exp],
#             [:exp, "/", :exp]]
#  }, :exp, 2) {|sent| p sent }
#  #=>
#  #<Sentence: "num">
#  #<Sentence: ("num") "+" ("num")>
#  #<Sentence: ("num") "+" (("num") "+" ("num"))>
#  #<Sentence: ("num") "+" (("num") "-" ("num"))>
#  #<Sentence: ("num") "+" (("num") "*" ("num"))>
#  #<Sentence: ("num") "+" (("num") "/" ("num"))>
#  #<Sentence: (("num") "+" ("num")) "+" ("num")>
#  ...
#
# Sentence.each takes 3 arguments.
# The first argument is the syntax for the expressions.
# The second argument, :exp, is a generating nonterminal.
# The third argument, 2, limits derivation to restrict results finitely.
#
# Some arithmetic expressions including parenthesis can be generated as follows.
#
#  syntax = {
#    :factor => [["n"],
#                ["(", :exp, ")"]],
#    :term => [[:factor],
#              [:term, "*", :factor],
#              [:term, "/", :factor]],
#    :exp => [[:term],
#             [:exp, "+", :term],
#             [:exp, "-", :term]]
#  }
#  Sentence.each(syntax, :exp, 2) {|sent| p sent }
#  #=>
#  #<Sentence: (("n"))>
#  #<Sentence: (("(" ((("n"))) ")"))>
#  #<Sentence: (("(" ((("(" ((("n"))) ")"))) ")"))>
#  #<Sentence: (("(" (((("n")) "*" ("n"))) ")"))>
#  #<Sentence: (("(" (((("n")) "/" ("n"))) ")"))>
#  #<Sentence: (("(" (((("n"))) "+" (("n"))) ")"))>
#  #<Sentence: (("(" (((("n"))) "-" (("n"))) ")"))>
#  #<Sentence: ((("n")) "*" ("n"))>
#  #<Sentence: ((("n")) "*" ("(" ((("n"))) ")"))>
#  ...
#
#  Sentence#to_s can be used to concatenate strings
#  in a sentence:
#
#  Sentence.each(syntax, :exp, 2) {|sent| p sent.to_s }
#  #=>
#  "n"
#  "(n)"
#  "((n))"
#  "(n*n)"
#  "(n/n)"
#  "(n+n)"
#  "(n-n)"
#  "n*n"
#  "n*(n)"
#  ...
#

# Sentence() instantiates a sentence object.
#
#  Sentence("foo", "bar")
#  #=> #<Sentence: "foo" "bar">
#
#  Sentence("foo", ["bar", "baz"])
#  #=> #<Sentence: "foo" ("bar" "baz")>
#
def Sentence(*ary)
  Sentence.new(ary)
end

# Sentence class represents a tree with string leaves.
#
class Sentence
  # _ary_ represents a tree.
  # It should be a possibly nested array which contains strings.
  #
  # Note that _ary_ is not copied.
  # Don't modify _ary_ after the sentence object is instantiated.
  #
  #  Sentence.new(["a", "pen"])
  #  #<Sentence: "a" "pen">
  #
  #  Sentence.new(["I", "have", ["a", "pen"]])
  #  #<Sentence: "I" "have" ("a" "pen")>
  #
  def initialize(ary)
    @sent = ary
  end

  # returns a string which is concatenation of all strings.
  # No separator is used.
  #
  #  Sentence("2", "+", "3").to_s
  #  "2+3"
  #
  #  Sentence("2", "+", ["3", "*", "5"]).to_s
  #  "2+3*5"
  #
  def to_s
    @sent.join('')
  end

  # returns a string which is concatenation of all strings separated by _sep_.
  # If _sep_ is not given, single space is used.
  #
  #  Sentence("I", "have", ["a", "pen"]).join
  #  "I have a pen"
  #
  #  Sentence("I", "have", ["a", "pen"]).join("/")
  #  "I/have/a/pen"
  #
  #  Sentence("a", [], "b").join("/")
  #  "a/b"
  #
  def join(sep=' ')
    @sent.flatten.join(sep)
  end

  # returns a tree as a nested array.
  #
  # Note that the result is not copied.
  # Don't modify the result.
  #
  #  Sentence(["foo", "bar"], "baz").to_a
  #  #=> [["foo", "bar"], "baz"]
  #
  def to_a
    @sent
  end

  # returns <i>i</i>th element as a sentence or string.
  #
  #  s = Sentence(["foo", "bar"], "baz")
  #  s    #=> #<Sentence: ("foo" "bar") "baz">
  #  s[0] #=> #<Sentence: "foo" "bar">
  #  s[1] #=> "baz"
  #
  def [](i)
    e = @sent[i]
    e.respond_to?(:to_ary) ? Sentence.new(e) : e
  end

  # returns the number of top level elements.
  #
  #  Sentence.new(%w[foo bar]).length
  #  #=> 2
  #
  #  Sentence(%w[2 * 7], "+", %w[3 * 5]).length
  #  #=> 3
  #
  def length
    @sent.length
  end

  # iterates over children.
  #
  #  Sentence(%w[2 * 7], "+", %w[3 * 5]).each {|v| p v }
  #  #=>
  #  #<Sentence: "2" "*" "7">
  #  "+"
  #  #<Sentence: "3" "*" "5">
  #
  def each # :yield: element
    @sent.each_index {|i|
      yield self[i]
    }
  end
  include Enumerable

  def inspect
    "#<#{self.class}: #{inner_inspect(@sent, '')}>"
  end

  # :stopdoc:
  def inner_inspect(ary, r)
    first = true
    ary.each {|obj|
      r << ' ' if !first
      first = false
      if obj.respond_to? :to_ary
        r << '('
        inner_inspect(obj, r)
        r << ')'
      else
        r << obj.inspect
      end
    }
    r
  end
  # :startdoc:

  # returns new sentence object which
  # _target_ is substituted by the block.
  #
  # Sentence#subst invokes <tt>_target_ === _string_</tt> for each
  # string in the sentence.
  # The strings which === returns true are substituted by the block.
  # The block is invoked with the substituting string.
  #
  #  Sentence.new(%w[2 + 3]).subst("+") { "*" }
  #  #<Sentence: "2" "*" "3">
  #
  #  Sentence.new(%w[2 + 3]).subst(/\A\d+\z/) {|s| ((s.to_i)*2).to_s }
  #  #=> #<Sentence: "4" "+" "6">
  #
  def subst(target, &b) # :yield: string
    Sentence.new(subst_rec(@sent, target, &b))
  end

  # :stopdoc:
  def subst_rec(obj, target, &b)
    if obj.respond_to? :to_ary
      a = []
      obj.each {|e| a << subst_rec(e, target, &b) }
      a
    elsif target === obj
      yield obj
    else
      obj
    end
  end
  # :startdoc:

  # find a subsentence and return it.
  # The block is invoked for each subsentence in preorder manner.
  # The first subsentence which the block returns true is returned.
  #
  #  Sentence(%w[2 * 7], "+", %w[3 * 5]).find_subtree {|s| s[1] == "*" }
  #  #=> #<Sentence: "2" "*" "7">
  #
  def find_subtree(&b) # :yield: sentence
    find_subtree_rec(@sent, &b)
  end

  # :stopdoc:
  def find_subtree_rec(obj, &b)
    if obj.respond_to? :to_ary
      s = Sentence.new(obj)
      if b.call s
        return s
      else
        obj.each {|e|
          r = find_subtree_rec(e, &b)
          return r if r
        }
      end
    end
    nil
  end
  # :startdoc:

  # returns a new sentence object which expands according to the condition
  # given by the block.
  #
  # The block is invoked for each subsentence.
  # The subsentences which the block returns true are
  # expanded into parent.
  #
  #  s = Sentence(%w[2 * 7], "+", %w[3 * 5])
  #  #=> #<Sentence: ("2" "*" "7") "+" ("3" "*" "5")>
  #
  #  s.expand { true }
  #  #=> #<Sentence: "2" "*" "7" "+" "3" "*" "5">
  #
  #  s.expand {|s| s[0] == "3" }
  #  #=> #<Sentence: (("2" "*" "7") "+" "3" "*" "5")>
  #
  def expand(&b) # :yield: sentence
    Sentence.new(expand_rec(@sent, &b))
  end

  # :stopdoc:
  def expand_rec(obj, r=[], &b)
    if obj.respond_to? :to_ary
      obj.each {|o|
        s = Sentence.new(o)
        if b.call s
          expand_rec(o, r, &b)
        else
          a = []
          expand_rec(o, a, &b)
          r << a
        end
      }
    else
      r << obj
    end
    r
  end
  # :startdoc:

  # Sentence.each generates sentences
  # by deriving the start symbol _sym_ using _syntax_.
  # The derivation is restricted by an positive integer _limit_ to
  # avoid infinite generation.
  #
  # Sentence.each yields the block with a generated sentence.
  #
  #  Sentence.each({
  #    :exp => [["n"],
  #             [:exp, "+", :exp],
  #             [:exp, "*", :exp]]
  #    }, :exp, 1) {|sent| p sent }
  #  #=>
  #  #<Sentence: "n">
  #  #<Sentence: ("n") "+" ("n")>
  #  #<Sentence: ("n") "*" ("n")>
  #
  #  Sentence.each({
  #    :exp => [["n"],
  #             [:exp, "+", :exp],
  #             [:exp, "*", :exp]]
  #    }, :exp, 2) {|sent| p sent }
  #  #=>
  #  #<Sentence: "n">