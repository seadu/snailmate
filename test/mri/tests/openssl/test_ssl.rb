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
      ctx.add_certificate(@svr_cert, @svr_key, [@ca_cert]) # RSA
      ctx.add_certificate(ecdsa_cert, ecdsa_key, [ca2_cert])
    }
    start_server(ctx_proc: ctx_proc) do |port|
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.max_version = :TLS1_2 # TODO: We need this to force certificate type
      ctx.ciphers = "aECDSA"
      server_connect(port, ctx) { |ssl|
        assert_equal ecdsa_cert.subject, ssl.peer_cert.subject
        assert_equal [ecdsa_cert.subject, ca2_cert.subject],
          ssl.peer_cert_chain.map(&:subject)
      }

      ctx = OpenSSL::SSL::SSLContext.new
      ctx.max_version = :TLS1_2
      ctx.ciphers = "aRSA"
      server_connect(port, ctx) { |ssl|
        assert_equal @svr_cert.subject, ssl.peer_cert.subject
        assert_equal [@svr_cert.subject, @ca_cert.subject],
          ssl.peer_cert_chain.map(&:subject)
      }
    end
  end

  def test_sysread_and_syswrite
    start_server { |port|
      server_connect(port) { |ssl|
        str = +("x" * 100 + "\n")
        ssl.syswrite(str)
        newstr = ssl.sysread(str.bytesize)
        assert_equal(str, newstr)

        buf = String.new
        ssl.syswrite(str)
        assert_same buf, ssl.sysread(str.size, buf)
        assert_equal(str, buf)
      }
    }
  end

  def test_getbyte
    start_server { |port|
      server_connect(port) { |ssl|
        str = +("x" * 100 + "\n")
        ssl.syswrite(str)
        newstr = str.bytesize.times.map { |i|
          ssl.getbyte
        }.pack("C*")
        assert_equal(str, newstr)
      }
    }
  end

  def test_sync_close
    start_server do |port|
      begin
        sock = TCPSocket.new("127.0.0.1", port)
        ssl = OpenSSL::SSL::SSLSocket.new(sock)
        ssl.connect
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
        ssl.close
        assert_not_predicate sock, :closed?
      ensure
        sock&.close
      end

      begin
        sock = TCPSocket.new("127.0.0.1", port)
        ssl = OpenSSL::SSL::SSLSocket.new(sock)
        ssl.sync_close = true  # !!
        ssl.connect
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
        ssl.close
        assert_predicate sock, :closed?
      ensure
        sock&.close
      end
    end
  end

  def test_copy_stream
    start_server do |port|
      server_connect(port) do |ssl|
        IO.pipe do |r, w|
          str = "hello world\n"
          w.write(str)
          IO.copy_stream(r, ssl, str.bytesize)
          IO.copy_stream(ssl, w, str.bytesize)
          assert_equal str, r.read(str.bytesize)
        end
      end
    end
  end

  def test_verify_mode_default
    ctx = OpenSSL::SSL::SSLContext.new
    assert_equal OpenSSL::SSL::VERIFY_NONE, ctx.verify_mode
  end

  def test_verify_mode_server_cert
    start_server(ignore_listener_error: true) { |port|
      populated_store = OpenSSL::X509::Store.new
      populated_store.add_cert(@ca_cert)
      empty_store = OpenSSL::X509::Store.new

      # Valid certificate, SSL_VERIFY_PEER
      assert_nothing_raised {
        ctx = OpenSSL::SSL::SSLContext.new
        ctx.verify_mode = OpenSSL::SSL::VERIFY_PEER
        ctx.cert_store = populated_store
        server_connect(port, ctx) { |ssl| ssl.puts("abc"); ssl.gets }
      }

      # Invalid certificate, SSL_VERIFY_NONE
      assert_nothing_raised {
        ctx = OpenSSL::SSL::SSLContext.new
        ctx.verify_mode = OpenSSL::SSL::VERIFY_NONE
        ctx.cert_store = empty_store
        server_connect(port, ctx) { |ssl| ssl.puts("abc"); ssl.gets }
      }

      # Invalid certificate, SSL_VERIFY_PEER
      assert_handshake_error {
        ctx = OpenSSL::SSL::SSLContext.new
        ctx.verify_mode = OpenSSL::SSL::VERIFY_PEER
        ctx.cert_store = empty_store
        server_connect(port, ctx) { |ssl| ssl.puts("abc"); ssl.gets }
      }
    }
  end

  def test_verify_mode_client_cert_required
    # Optional, client certificate not supplied
    vflag = OpenSSL::SSL::VERIFY_PEER
    accept_proc = -> ssl {
      assert_equal nil, ssl.peer_cert
    }
    start_server(verify_mode: vflag, accept_proc: accept_proc) { |port|
      assert_nothing_raised {
        server_connect(port) { |ssl| ssl.puts("abc"); ssl.gets }
      }
    }

    # Required, client certificate not supplied
    vflag = OpenSSL::SSL::VERIFY_PEER|OpenSSL::SSL::VERIFY_FAIL_IF_NO_PEER_CERT
    start_server(verify_mode: vflag, ignore_listener_error: true) { |port|
      assert_handshake_error {
        server_connect(port) { |ssl| ssl.puts("abc"); ssl.gets }
      }
    }
  end

  def test_client_auth_success
    vflag = OpenSSL::SSL::VERIFY_PEER|OpenSSL::SSL::VERIFY_FAIL_IF_NO_PEER_CERT
    start_server(verify_mode: vflag,
      ctx_proc: proc { |ctx|
        ctx.max_version = OpenSSL::SSL::TLS1_2_VERSION if libressl?(3, 2, 0)
    }) { |port|
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.key = @cli_key
      ctx.cert = @cli_cert

      server_connect(port, ctx) { |ssl|
        ssl.puts("foo")
        assert_equal("foo\n", ssl.gets)
      }

      called = nil
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.client_cert_cb = Proc.new{ |sslconn|
        called = true
        [@cli_cert, @cli_key]
      }

      server_connect(port, ctx) { |ssl|
        assert(called)
        ssl.puts("foo")
        assert_equal("foo\n", ssl.gets)
      }
    }
  end

  def test_client_cert_cb_ignore_error
    vflag = OpenSSL::SSL::VERIFY_PEER|OpenSSL::SSL::VERIFY_FAIL_IF_NO_PEER_CERT
    start_server(verify_mode: vflag, ignore_listener_error: true) do |port|
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.client_cert_cb = -> ssl {
        raise "exception in client_cert_cb must be suppressed"
      }
      # 1. Exception in client_cert_cb is suppressed
      # 2. No client certificate will be sent to the server
      # 3. SSL_VERIFY_FAIL_IF_NO_PEER_CERT causes the handshake to fail
      assert_handshake_error {
        server_connect(port, ctx) { |ssl| ssl.puts("abc"); ssl.gets }
      }
    end
  end

  def test_client_ca
    pend "LibreSSL 3.2 has broken client CA support" if libressl?(3, 2, 0)

    ctx_proc = Proc.new do |ctx|
      ctx.client_ca = [@ca_cert]
    end

    vflag = OpenSSL::SSL::VERIFY_PEER|OpenSSL::SSL::VERIFY_FAIL_IF_NO_PEER_CERT
    start_server(verify_mode: vflag, ctx_proc: ctx_proc) { |port|
      ctx = OpenSSL::SSL::SSLContext.new
      client_ca_from_server = nil
      ctx.client_cert_cb = Proc.new do |sslconn|
        client_ca_from_server = sslconn.client_ca
        [@cli_cert, @cli_key]
      end
      server_connect(port, ctx) { |ssl|
        assert_equal([@ca], client_ca_from_server)
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      }
    }
  end

  def test_unstarted_session
    start_server do |port|
      sock = TCPSocket.new("127.0.0.1", port)
      ssl = OpenSSL::SSL::SSLSocket.new(sock)

      assert_raise(OpenSSL::SSL::SSLError) { ssl.syswrite("data") }
      assert_raise(OpenSSL::SSL::SSLError) { ssl.sysread(1) }

      ssl.connect
      ssl.puts "abc"
      assert_equal "abc\n", ssl.gets
    ensure
      ssl&.close
      sock&.close
    end
  end

  def test_parallel
    start_server { |port|
      ssls = []
      10.times{
        sock = TCPSocket.new("127.0.0.1", port)
        ssl = OpenSSL::SSL::SSLSocket.new(sock)
        ssl.connect
        ssl.sync_close = true
        ssls << ssl
      }
      str = "x" * 1000 + "\n"
      ITERATIONS.times{
        ssls.each{|ssl|
          ssl.puts(str)
          assert_equal(str, ssl.gets)
        }
      }
      ssls.each{|ssl| ssl.close }
    }
  end

  def test_verify_result
    start_server(ignore_listener_error: true) { |port|
      sock = TCPSocket.new("127.0.0.1", port)
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.verify_mode = OpenSSL::SSL::VERIFY_PEER
      ssl = OpenSSL::SSL::SSLSocket.new(sock, ctx)
      ssl.sync_close = true
      begin
        assert_raise(OpenSSL::SSL::SSLError){ ssl.connect }
        assert_equal(OpenSSL::X509::V_ERR_SELF_SIGNED_CERT_IN_CHAIN, ssl.verify_result)
      ensure
        ssl.close
      end
    }

    start_server { |port|
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.verify_mode = OpenSSL::SSL::VERIFY_PEER
      ctx.verify_callback = Proc.new do |preverify_ok, store_ctx|
        store_ctx.error = OpenSSL::X509::V_OK
        true
      end
      server_connect(port, ctx) { |ssl|
        assert_equal(OpenSSL::X509::V_OK, ssl.verify_result)
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      }
    }

    start_server(ignore_listener_error: true) { |port|
      sock = TCPSocket.new("127.0.0.1", port)
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.verify_mode = OpenSSL::SSL::VERIFY_PEER
      ctx.verify_callback = Proc.new do |preverify_ok, store_ctx|
        store_ctx.error = OpenSSL::X509::V_ERR_APPLICATION_VERIFICATION
        false
      end
      ssl = OpenSSL::SSL::SSLSocket.new(sock, ctx)
      ssl.sync_close = true
      begin
        assert_raise(OpenSSL::SSL::SSLError){ ssl.connect }
        assert_equal(OpenSSL::X509::V_ERR_APPLICATION_VERIFICATION, ssl.verify_result)
      ensure
        ssl.close
      end
    }
  end

  def test_exception_in_verify_callback_is_ignored
    start_server(ignore_listener_error: true) { |port|
      sock = TCPSocket.new("127.0.0.1", port)
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.verify_mode = OpenSSL::SSL::VERIFY_PEER
      ctx.verify_callback = Proc.new do |preverify_ok, store_ctx|
        store_ctx.error = OpenSSL::X509::V_OK
        raise RuntimeError
      end
      ssl = OpenSSL::SSL::SSLSocket.new(sock, ctx)
      ssl.sync_close = true
      begin
        EnvUtil.suppress_warning do
          # SSLError, not RuntimeError
          assert_raise(OpenSSL::SSL::SSLError) { ssl.connect }
        end
        assert_equal(OpenSSL::X509::V_ERR_CERT_REJECTED, ssl.verify_result)
      ensure
        ssl.close
      end
    }
  end

  def test_finished_messages
    server_finished = nil
    server_peer_finished = nil
    client_finished = nil
    client_peer_finished = nil

    start_server(accept_proc: proc { |server|
      server_finished = server.finished_message
      server_peer_finished = server.peer_finished_message
    }, ctx_proc: proc { |ctx|
      ctx.max_version = OpenSSL::SSL::TLS1_2_VERSION if libressl?(3, 2, 0)
    }) { |port|
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.verify_mode = OpenSSL::SSL::VERIFY_NONE
      ctx.max_version = :TLS1_2 if libressl?(3, 2, 0) && !libressl?(3, 3, 0)
      server_connect(port, ctx) { |ssl|
        ssl.puts "abc"; ssl.gets

        client_finished = ssl.finished_message
        client_peer_finished = ssl.peer_finished_message
      }
    }
    assert_not_nil(server_finished)
    assert_not_nil(client_finished)
    assert_equal(server_finished, client_peer_finished)
    assert_equal(server_peer_finished, client_finished)
  end

  def test_sslctx_set_params
    ctx = OpenSSL::SSL::SSLContext.new
    ctx.set_params

    assert_equal OpenSSL::SSL::VERIFY_PEER, ctx.verify_mode
    ciphers_names = ctx.ciphers.collect{|v, _, _, _| v }
    assert ciphers_names.all?{|v| /A(EC)?DH/ !~ v }, "anon ciphers are disabled"
    assert ciphers_names.all?{|v| /(RC4|MD5|EXP|DES(?!-EDE|-CBC3))/ !~ v }, "weak ciphers are disabled"
    assert_equal 0, ctx.options & OpenSSL::SSL::OP_DONT_INSERT_EMPTY_FRAGMENTS
    assert_equal OpenSSL::SSL::OP_NO_COMPRESSION,
                 ctx.options & OpenSSL::SSL::OP_NO_COMPRESSION
  end

  def test_post_connect_check_with_anon_ciphers
    ctx_proc = -> ctx {
      ctx.ssl_version = :TLSv1_2
      ctx.ciphers = "aNULL"
      ctx.tmp_dh = Fixtures.pkey("dh-1")
      ctx.security_level = 0
    }

    start_server(ctx_proc: ctx_proc) { |port|
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.ssl_version = :TLSv1_2
      ctx.ciphers = "aNULL"
      ctx.security_level = 0
      server_connect(port, ctx) { |ssl|
        assert_raise_with_message(OpenSSL::SSL::SSLError, /anonymous cipher suite/i) {
          ssl.post_connection_check("localhost.localdomain")
        }
      }
    }
  end

  def test_post_connection_check
    sslerr = OpenSSL::SSL::SSLError

    start_server { |port|
      server_connect(port) { |ssl|
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets

        assert_raise(sslerr){ssl.post_connection_check("localhost.localdomain")}
        assert_raise(sslerr){ssl.post_connection_check("127.0.0.1")}
        assert(ssl.post_connection_check("localhost"))
        assert_raise(sslerr){ssl.post_connection_check("foo.example.com")}

        cert = ssl.peer_cert
        assert(!OpenSSL::SSL.verify_certificate_identity(cert, "localhost.localdomain"))
        assert(!OpenSSL::SSL.verify_certificate_identity(cert, "127.0.0.1"))
        assert(OpenSSL::SSL.verify_certificate_identity(cert, "localhost"))
        assert(!OpenSSL::SSL.verify_certificate_identity(cert, "foo.example.com"))
      }
    }

    exts = [
      ["keyUsage","keyEncipherment,digitalSignature",true],
      ["subjectAltName","DNS:localhost.localdomain,IP:127.0.0.1",false],
    ]
    @svr_cert = issue_cert(@svr, @svr_key, 4, exts, @ca_cert, @ca_key)
    start_server { |port|
      server_connect(port) { |ssl|
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets

        assert(ssl.post_connection_check("localhost.localdomain"))
        assert(ssl.post_connection_check("127.0.0.1"))
        assert_raise(sslerr){ssl.post_connection_check("localhost")}
        assert_raise(s