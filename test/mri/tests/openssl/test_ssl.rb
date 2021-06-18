# frozen_string_literal: true
require_relative "utils"

if defined?(OpenSSL)

class OpenSSL::TestSSL < OpenSSL::SSLTestCase
  def test_bad_socket
    bad_socket = Struct.new(:sync).new
    assert_raise TypeError do
      socket = OpenSSL::SSL::SSLSocket.new bad_socket
      # if the socket is not a T_FILE, `connect` will segv because it tries
      # to get the underlying file descriptor but the API it calls assumes
      # the object type is T_FILE
      socket.connect
    end
  end

  def test_ctx_options
    ctx = OpenSSL::SSL::SSLContext.new

    assert (OpenSSL::SSL::OP_ALL & ctx.options) == OpenSSL::SSL::OP_ALL,
           "OP_ALL is set by default"
    ctx.options = 4
    assert_equal 4, ctx.options & 4
    if ctx.options != 4
      pend "SSL_CTX_set_options() seems to be modified by distributor"
    end
    ctx.options = nil
    assert_equal OpenSSL::SSL::OP_ALL, ctx.options

    assert_equal true, ctx.setup
    assert_predicate ctx, :frozen?
    assert_equal nil, ctx.setup
  end

  def test_ssl_with_server_cert
    ctx_proc = -> ctx {
      ctx.cert = @svr_cert
      ctx.key = @svr_key
      ctx.extra_chain_cert = [@ca_cert]
    }
    server_proc = -> (ctx, ssl) {
      assert_equal @svr_cert.to_der, ssl.cert.to_der
      assert_equal nil, ssl.peer_cert

      readwrite_loop(ctx, ssl)
    }
    start_server(ctx_proc: ctx_proc, server_proc: server_proc) { |port|
      begin
        sock = TCPSocket.new("127.0.0.1", port)
        ctx = OpenSSL::SSL::SSLContext.new
        ssl = OpenSSL::SSL::SSLSocket.new(sock, ctx)
        ssl.connect

        assert_equal sock, ssl.io
        assert_equal nil, ssl.cert
        assert_equal @svr_cert.to_der, ssl.peer_cert.to_der
        assert_equal 2, ssl.peer_cert_chain.size
        assert_equal @svr_cert.to_der, ssl.peer_cert_chain[0].to_der
        assert_equal @ca_cert.to_der, ssl.peer_cert_chain[1].to_der

        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      ensure
        ssl&.close
        sock&.close
      end
    }
  end

  def test_socket_open
    start_server { |port|
      begin
        ssl = OpenSSL::SSL::SSLSocket.open("127.0.0.1", port)
        ssl.sync_close = true
        ssl.connect

        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      ensure
        ssl&.close
      end
    }
  end

  def test_socket_open_with_context
    start_server { |port|
      begin
        ctx = OpenSSL::SSL::SSLContext.new
        ssl = OpenSSL::SSL::SSLSocket.open("127.0.0.1", port, context: ctx)
        ssl.sync_close = true
        ssl.connect

        assert_equal ssl.context, ctx
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      ensure
        ssl&.close
      end
    }
  end

  def test_socket_open_with_local_address_port_context
    start_server { |port|
      begin
        # Guess a free port number
        random_port = rand(49152..65535)
        ctx = OpenSSL::SSL::SSLContext.new
        ssl = OpenSSL::SSL::SSLSocket.open("127.0.0.1", port, "127.0.0.1", random_port, context: ctx)
        ssl.sync_close = true
        ssl.connect

        assert_equal ctx, ssl.context
        assert_equal random_port, ssl.io.local_address.ip_port
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      rescue Errno::EADDRINUSE, Errno::EACCES
      ensure
        ssl&.close
      end
    }
  end

  def test_add_certificate
    ctx_proc = -> ctx {
      # Unset values set by start_server
      ctx.cert = ctx.key = ctx.extra_chain_cert = nil
      ctx.add_certificate(@svr_cert, @svr_key, [@ca_cert]) # RSA
    }
    start_server(ctx_proc: ctx_proc) do |port|
      server_connect(port) { |ssl|
        assert_equal @svr_cert.subject, ssl.peer_cert.subject
        assert_equal [@svr_cert.subject, @ca_cert.subject],
          ssl.peer_cert_chain.map(&:subject)

        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      }
    end
  end

  def test_add_certificate_multiple_certs
    ca2_key = Fixtures.pkey("rsa-3")
    ca2_exts = [
      ["basicConstraints", "CA:TRUE", true],
      ["keyUsage", "cRLSign, keyCertSign", true],
    ]
    ca2_dn = OpenSSL::X509::Name.parse_rfc2253("CN=CA2")
    ca2_cert = issue_cert(ca2_dn, ca2_key, 123, ca2_exts, nil, nil)

    ecdsa_key = Fixtures.pkey("p256")
    exts = [
      ["keyUsage", "digitalSignature", false],
    ]
    ecdsa_dn = OpenSSL::X509::Name.parse_rfc2253("CN=localhost2")
    ecdsa_cert = issue_cert(ecdsa_dn, ecdsa_key, 456, exts, ca2_cert, ca2_key)

    ctx_proc = -> ctx {
      # Unset values set by start_server
      ctx.cert = ctx.key = ctx.extra_chain_cert = nil
      ctx.add_certificate(@svr_c