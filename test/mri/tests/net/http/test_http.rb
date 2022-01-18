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

      assert http.proxy_from_env?
    end
  end

  def test_addr_port
    http = Net::HTTP.new 'hostname.example', nil, nil
    addr_port = http.__send__ :addr_port
    assert_equal 'hostname.example', addr_port

    http.use_ssl = true
    addr_port = http.__send__ :addr_port
    assert_equal 'hostname.example:80', addr_port

    http = Net::HTTP.new '203.0.113.1', nil, nil
    addr_port = http.__send__ :addr_port
    assert_equal '203.0.113.1', addr_port

    http.use_ssl = true
    addr_port = http.__send__ :addr_port
    assert_equal '203.0.113.1:80', addr_port

    http = Net::HTTP.new '2001:db8::1', nil, nil
    addr_port = http.__send__ :addr_port
    assert_equal '[2001:db8::1]', addr_port

    http.use_ssl = true
    addr_port = http.__send__ :addr_port
    assert_equal '[2001:db8::1]:80', addr_port

  end

  def test_edit_path
    http = Net::HTTP.new 'hostname.example', nil, nil

    edited = http.send :edit_path, '/path'

    assert_equal '/path', edited

    http.use_ssl = true

    edited = http.send :edit_path, '/path'

    assert_equal '/path', edited
  end

  def test_edit_path_proxy
    http = Net::HTTP.new 'hostname.example', nil, 'proxy.example'

    edited = http.send :edit_path, '/path'

    assert_equal 'http://hostname.example/path', edited

    http.use_ssl = true

    edited = http.send :edit_path, '/path'

    assert_equal '/path', edited
  end

  def test_proxy_address
    TestNetHTTPUtils.clean_http_proxy_env do
      http = Net::HTTP.new 'hostname.example', nil, 'proxy.example'
      assert_equal 'proxy.example', http.proxy_address

      http = Net::HTTP.new 'hostname.example', nil
      assert_equal nil, http.proxy_address
    end
  end

  def test_proxy_address_no_proxy
    TestNetHTTPUtils.clean_http_proxy_env do
      http = Net::HTTP.new 'hostname.example', nil, 'proxy.example', nil, nil, nil, 'example'
      assert_nil http.proxy_address

      http = Net::HTTP.new '10.224.1.1', nil, 'proxy.example', nil, nil, nil, 'example,10.224.0.0/22'
      assert_nil http.proxy_address
    end
  end

  def test_proxy_from_env_ENV
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy'] = 'http://proxy.example:8000'

      assert_equal false, Net::HTTP.proxy_class?
      http = Net::HTTP.new 'hostname.example'

      assert_equal true, http.proxy_from_env?
    end
  end

  def test_proxy_address_ENV
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy'] = 'http://proxy.example:8000'

      http = Net::HTTP.new 'hostname.example'

      assert_equal 'proxy.example', http.proxy_address
    end
  end

  def test_proxy_eh_no_proxy
    TestNetHTTPUtils.clean_http_proxy_env do
      assert_equal false, Net::HTTP.new('hostname.example', nil, nil).proxy?
    end
  end

  def test_proxy_eh_ENV
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy'] = 'http://proxy.example:8000'

      http = Net::HTTP.new 'hostname.example'

      assert_equal true, http.proxy?
    end
  end

  def test_proxy_eh_ENV_with_user
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy'] = 'http://foo:bar@proxy.example:8000'

      http = Net::HTTP.new 'hostname.example'

      assert_equal true, http.proxy?
      assert_equal 'foo', http.proxy_user
      assert_equal 'bar', http.proxy_pass
    end
  end

  def test_proxy_eh_ENV_with_urlencoded_user
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy'] = 'http://Y%5CX:R%25S%5D%20%3FX@proxy.example:8000'

      http = Net::HTTP.new 'hostname.example'

      assert_equal true, http.proxy?
      assert_equal "Y\\X", http.proxy_user
      assert_equal "R%S] ?X", http.proxy_pass
    end
  end

  def test_proxy_eh_ENV_none_set
    TestNetHTTPUtils.clean_http_proxy_env do
      assert_equal false, Net::HTTP.new('hostname.example').proxy?
    end
  end

  def test_proxy_eh_ENV_no_proxy
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy'] = 'http://proxy.example:8000'
      ENV['no_proxy']   = 'hostname.example'

      assert_equal false, Net::HTTP.new('hostname.example').proxy?
    end
  end

  def test_proxy_port
    TestNetHTTPUtils.clean_http_proxy_env do
      http = Net::HTTP.new 'example', nil, 'proxy.example'
      assert_equal 'proxy.example', http.proxy_address
      assert_equal 80, http.proxy_port
      http = Net::HTTP.new 'example', nil, 'proxy.example', 8000
      assert_equal 8000, http.proxy_port
      http = Net::HTTP.new 'example', nil
      assert_equal nil, http.proxy_port
    end
  end

  def test_proxy_port_ENV
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy'] = 'http://proxy.example:8000'

      http = Net::HTTP.new 'hostname.example'

      assert_equal 8000, http.proxy_port
    end
  end

  def test_newobj
    TestNetHTTPUtils.clean_http_proxy_env do
      ENV['http_proxy'] = 'http://proxy.example:8000'

      http = Net::HTTP.newobj 'hostname.example'

      assert_equal false, http.proxy?
    end
  end

  def test_failure_message_includes_failed_domain_and_port
    # hostname to be included in the error message
    host = Struct.new(:to_s).new("<example>")
    port = 2119
    # hack to let TCPSocket.open fail
    def host.to_str; raise SocketError, "open failure"; end
    uri = Struct.new(:scheme, :hostname, :port).new("http", host, port)
    assert_raise_with_message(SocketError, /#{host}:#{port}/) do
      TestNetHTTPUtils.clean_http_proxy_env{ Net::HTTP.get(uri) }
    end
  end

end

module TestNetHTTP_version_1_1_methods

  def test_s_start
    begin
      h = Net::HTTP.start(config('host'), config('port'))
    ensure
      h&.finish
    end
    assert_equal config('host'), h.address
    assert_equal config('port'), h.port
    assert_equal true, h.instance_variable_get(:@proxy_from_env)

    begin
      h = Net::HTTP.start(config('host'), config('port'), :ENV)
    ensure
      h&.finish
    end
    assert_equal config('host'), h.address
    assert_equal config('port'), h.port
    assert_equal true, h.instance_variable_get(:@proxy_from_env)

    begin
      h = Net::HTTP.start(config('host'), config('port'), nil)
    ensure
      h&.finish
    end
    assert_equal config('host'), h.address
    assert_equal config('port'), h.port
    assert_equal false, h.instance_variable_get(:@proxy_from_env)
  end

  def test_s_get
    assert_equal $test_net_http_data,
        Net::HTTP.get(config('host'), '/', config('port'))

    assert_equal $test_net_http_data, Net::HTTP.get(
      URI.parse("http://#{config('host')}:#{config('port')}")
    )
    assert_equal $test_net_http_data, Net::HTTP.get(
      URI.parse("http://#{config('host')}:#{config('port')}"), "Accept" => "text/plain"
    )
  end

  def test_s_get_response
    res = Net::HTTP.get_response(
      URI.parse("h