# frozen_string_literal: false
require_relative 'functions'
require_relative 'xpath_parser'

module REXML
  # Wrapper class.  Use this class to access the XPath functions.
  class XPath
    include Functions
    # A base Hash object, supposing to be used when initializing a
    # default empty namespaces set, but is currently unused.
    # TODO: either set the namespaces=EMPTY_HASH, or deprecate this.
    EMPTY_HASH = {}

    # Finds and returns the first node that matches the supplied xpath.
    # element::
    #   The context element
    # path::
    #   The xpath to search for.  If not supplied or nil, returns the first
    #   node matching '*'.
    # namespaces::
    #   If supplied, a Hash which defines a namespace mapping.
    # variables::
    #   If supplied, a Hash which maps $variables in the query
    #   to values. This can be used to avoid XPath injection attacks
    #   or to automatically h