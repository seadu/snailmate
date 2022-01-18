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
      URI.parse("http://#{config('host')}:#{config('port')}")
    )
    assert_equal "application/octet-stream", res["Content-Type"]
    assert_equal $test_net_http_data, res.body

    res = Net::HTTP.get_response(
      URI.parse("http://#{config('host')}:#{config('port')}"), "Accept" => "text/plain"
    )
    assert_equal "text/plain", res["Content-Type"]
    assert_equal $test_net_http_data, res.body
  end

  def test_head
    start {|http|
      res = http.head('/')
      assert_kind_of Net::HTTPResponse, res
      assert_equal $test_net_http_data_type, res['Content-Type']
      unless self.is_a?(TestNetHTTP_v1_2_chunked)
        assert_equal $test_net_http_data.size, res['Content-Length'].to_i
      end
    }
  end

  def test_get
    start {|http|
      _test_get__get http
      _test_get__iter http
      _test_get__chunked http
    }
  end

  def _test_get__get(http)
    res = http.get('/')
    assert_kind_of Net::HTTPResponse, res
    assert_kind_of String, res.body
    unless self.is_a?(TestNetHTTP_v1_2_chunked)
      assert_not_nil res['content-length']
      assert_equal $test_net_http_data.size, res['content-length'].to_i
    end
    assert_equal $test_net_http_data_type, res['Content-Type']
    assert_equal $test_net_http_data.size, res.body.size
    assert_equal $test_net_http_data, res.body

    assert_nothing_raised {
      http.get('/', { 'User-Agent' => 'test' }.freeze)
    }

    assert res.decode_content, '[Bug #7924]' if Net::HTTP::HAVE_ZLIB
  end

  def _test_get__iter(http)
    buf = ''
    res = http.get('/') {|s| buf << s }
    assert_kind_of Net::HTTPResponse, res
    # assert_kind_of String, res.body
    unless self.is_a?(TestNetHTTP_v1_2_chunked)
      assert_not_nil res['content-length']
      assert_equal $test_net_http_data.size, res['content-length'].to_i
    end
    assert_equal $test_net_http_data_type, res['Content-Type']
    assert_equal $test_net_http_data.size, buf.size
    assert_equal $test_net_http_data, buf
    # assert_equal $test_net_http_data.size, res.body.size
    # assert_equal $test_net_http_data, res.body
  end

  def _test_get__chunked(http)
    buf = ''
    res = http.get('/') {|s| buf << s }
    assert_kind_of Net::HTTPResponse, res
    # assert_kind_of String, res.body
    unless self.is_a?(TestNetHTTP_v1_2_chunked)
      assert_not_nil res['content-length']
      assert_equal $test_net_http_data.size, res['content-length'].to_i
    end
    assert_equal $test_net_http_data_type, res['Content-Type']
    assert_equal $test_net_http_data.size, buf.size
    assert_equal $test_net_http_data, buf
    # assert_equal $test_net_http_data.size, res.body.size
    # assert_equal $test_net_http_data, res.body
  end

  def test_get__break
    i = 0
    start {|http|
      http.get('/') do |str|
        i += 1
        break
      end
    }
    assert_equal 1, i
    @log_tester = nil # server may encount ECONNRESET
  end

  def test_get__implicit_start
    res = new().get('/')
    assert_kind_of Net::HTTPResponse, res
    assert_kind_of String, res.body
    unless self.is_a?(TestNetHTTP_v1_2_chunked)
      assert_not_nil res['content-length']
    end
    assert_equal $test_net_http_data_type, res['Content-Type']
    assert_equal $test_net_http_data.size, res.body.size
    assert_equal $test_net_http_data, res.body
  end

  def test_get__crlf
    start {|http|
      assert_raise(ArgumentError) do
        http.get("\r")
      end
      assert_raise(ArgumentError) do
        http.get("\n")
      end
    }
  end

  def test_get2
    start {|http|
      http.get2('/') {|res|
        EnvUtil.suppress_warning do
          assert_kind_of Net::HTTPResponse, res
          assert_kind_of Net::HTTPResponse, res.header
        end

        unless self.is_a?(TestNetHTTP_v1_2_chunked)
          assert_not_nil res['content-length']
        end
        assert_equal $test_net_http_data_type, res['Content-Type']
        assert_kind_of String, res.body
        assert_kind_of String, res.entity
        assert_equal $test_net_http_data.size, res.body.size
        assert_equal $test_net_http_data, res.body
        assert_equal $test_net_http_data, res.entity
      }
    }
  end

  def test_post
    start {|http|
      _test_post__base http
      _test_post__file http
      _test_post__no_data http
    }
  end

  def _test_post__base(http)
    uheader = {}
    uheader['Accept'] = 'application/octet-stream'
    uheader['Content-Type'] = 'application/x-www-form-urlencoded'
    data = 'post data'
    res = http.post('/', data, uheader)
    assert_kind_of Net::HTTPResponse, res
    assert_kind_of String, res.body
    assert_equal data, res.body
    assert_equal data, res.entity
  end

  def _test_post__file(http)
    data = 'post data'
    f = StringIO.new
    http.post('/', data, {'content-type' => 'application/x-www-form-urlencoded'}, f)
    assert_equal data, f.string
  end

  def _test_post__no_data(http)
    unless self.is_a?(TestNetHTTP_v1_2_chunked)
      EnvUtil.suppress_warning do
        data = nil
        res = http.post('/', data)
        assert_not_equal '411', res.code
      end
    end
  end

  def test_s_post
    url = "http://#{config('host')}:#{config('port')}/?q=a"
    res = assert_warning(/Content-Type did not set/) do
      Net::HTTP.post(
              URI.parse(url),
              "a=x")
    end
    assert_equal "application/x-www-form-urlencoded", res["Content-Type"]
    assert_equal "a=x", res.body
    assert_equal url, res["X-request-uri"]

    res = Net::HTTP.post(
              URI.parse(url),
              "hello world",
              "Content-Type" => "text/plain; charset=US-ASCII")
    assert_equal "text/plain; charset=US-ASCII", res["Content-Type"]
    assert_equal "hello world", res.body
  end

  def test_s_post_form
    url = "http://#{config('host')}:#{config('port')}/"
    res = Net::HTTP.post_form(
              URI.parse(url),
              "a" => "x")
    assert_equal ["a=x"], res.body.split(/[;&]/).sort

    res = Net::HTTP.post_form(
              URI.parse(url),
              "a" => "x",
              "b" => "y")
    assert_equal ["a=x", "b=y"], res.body.split(/[;&]/).sort

    res = Net::HTTP.post_form(
              URI.parse(url),
              "a" => ["x1", "x2"],
              "b" => "y")
    assert_equal url, res['X-request-uri']
    assert_equal ["a=x1", "a=x2", "b=y"], res.body.split(/[;&]/).sort

    res = Net::HTTP.post_form(
              URI.parse(url + '?a=x'),
              "b" => "y")
    assert_equal url + '?a=x', res['X-request-uri']
    assert_equal ["b=y"], res.body.split(/[;&]/).sort
  end

  def test_patch
    start {|http|
      _test_patch__base http
    }
  end

  def _test_patch__base(http)
    uheader = {}
    uheader['Accept'] = 'application/octet-str