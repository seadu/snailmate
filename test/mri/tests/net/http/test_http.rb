# coding: US-ASCII
# frozen_string_literal: false
require 'test/unit'
require 'net/http'
require 'stringio'
require_relative 'utils'

class TestNetHTTP < Test::Unit::TestCase

  def test_class_Proxy
    no_proxy_class = Net::HTTP.Proxy nil

    assert_equal Net::HTTP, no_proxy_class

    proxy_class = Net::HTTP.Proxy 'proxy.example', 8000, 'user', 'pass'

    assert_not_equal Net::HTTP, proxy_class

    assert_operator proxy_class, :<, Net::HTTP

    assert_equal 'proxy.example', proxy_class.proxy_address
    assert_equal 8000,            proxy_class.proxy_port
    assert_equal 'user',          proxy_class.proxy_user
    assert_equal 'pass',          proxy_class.proxy_pass

    http = proxy_class.new 'hostname.example'

    assert_not_predicate http, :proxy_from_env?


    proxy_class = Net::HTTP.Proxy 'proxy.example'
    assert_equal 'proxy.example', proxy_class.proxy_address
    assert_equal 80,              proxy_class.proxy_port
  end

  def test_class_Proxy_from_ENV
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy']      = 'http://proxy.example:8000'

      # These are ignored on purpose.  See Bug 4388 and Feature 6546
      ENV['http_proxy_user'] = 'user'
      ENV['http_proxy_pass'] = 'pass'

      proxy_class = Net::HTTP.Proxy :ENV

      assert_not_equal Net::HTTP, proxy_class

      assert_operator proxy_class, :<, Net::HTTP

      assert_nil proxy_class.proxy_address
      assert_nil proxy_class.proxy_user
      assert_nil proxy_class.proxy_pass

      assert_not_equal 8000, proxy_class.proxy_port

      http = proxy_class.new 'hostname.example'

      assert http.pr