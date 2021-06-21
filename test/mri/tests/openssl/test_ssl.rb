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
        assert_raise(sslerr){ssl.post_connection_check("foo.example.com")}

        cert = ssl.peer_cert
        assert(OpenSSL::SSL.verify_certificate_identity(cert, "localhost.localdomain"))
        assert(OpenSSL::SSL.verify_certificate_identity(cert, "127.0.0.1"))
        assert(!OpenSSL::SSL.verify_certificate_identity(cert, "localhost"))
        assert(!OpenSSL::SSL.verify_certificate_identity(cert, "foo.example.com"))
      }
    }

    exts = [
      ["keyUsage","keyEncipherment,digitalSignature",true],
      ["subjectAltName","DNS:*.localdomain",false],
    ]
    @svr_cert = issue_cert(@svr, @svr_key, 5, exts, @ca_cert, @ca_key)
    start_server { |port|
      server_connect(port) { |ssl|
        ssl.puts "abc"; assert_equal "abc\n", ssl.gets

        assert(ssl.post_connection_check("localhost.localdomain"))
        assert_raise(sslerr){ssl.post_connection_check("127.0.0.1")}
        assert_raise(sslerr){ssl.post_connection_check("localhost")}
        assert_raise(sslerr){ssl.post_connection_check("foo.example.com")}
        cert = ssl.peer_cert
        assert(OpenSSL::SSL.verify_certificate_identity(cert, "localhost.localdomain"))
        assert(!OpenSSL::SSL.verify_certificate_identity(cert, "127.0.0.1"))
        assert(!OpenSSL::SSL.verify_certificate_identity(cert, "localhost"))
        assert(!OpenSSL::SSL.verify_certificate_identity(cert, "foo.example.com"))
      }
    }
  end

  def test_verify_certificate_identity
    [true, false].each do |criticality|
      cert = create_null_byte_SAN_certificate(criticality)
      assert_equal(false, OpenSSL::SSL.verify_certificate_identity(cert, 'www.example.com'))
      assert_equal(true,  OpenSSL::SSL.verify_certificate_identity(cert, "www.example.com\0.evil.com"))
      assert_equal(false, OpenSSL::SSL.verify_certificate_identity(cert, '192.168.7.255'))
      assert_equal(true,  OpenSSL::SSL.verify_certificate_identity(cert, '192.168.7.1'))
      assert_equal(true,  OpenSSL::SSL.verify_certificate_identity(cert, '13::17'))
      assert_equal(false,  OpenSSL::SSL.verify_certificate_identity(cert, '13::18'))
      assert_equal(true,  OpenSSL::SSL.verify_certificate_identity(cert, '13:0:0:0:0:0:0:17'))
      assert_equal(false,  OpenSSL::SSL.verify_certificate_identity(cert, '44:0:0:0:0:0:0:17'))
      assert_equal(true,  OpenSSL::SSL.verify_certificate_identity(cert, '0013:0000:0000:0000:0000:0000:0000:0017'))
      assert_equal(false,  OpenSSL::SSL.verify_certificate_identity(cert, '1313:0000:0000:0000:0000:0000:0000:0017'))
    end
  end

  def test_verify_hostname
    assert_equal(true,  OpenSSL::SSL.verify_hostname("www.example.com", "*.example.com"))
    assert_equal(false, OpenSSL::SSL.verify_hostname("www.subdomain.example.com", "*.example.com"))
  end

  def test_verify_wildcard
    assert_equal(false, OpenSSL::SSL.verify_wildcard("foo", "x*"))
    assert_equal(true,  OpenSSL::SSL.verify_wildcard("foo", "foo"))
    assert_equal(true,  OpenSSL::SSL.verify_wildcard("foo", "f*"))
    assert_equal(true,  OpenSSL::SSL.verify_wildcard("foo", "*"))
    assert_equal(false, OpenSSL::SSL.verify_wildcard("abc*bcd", "abcd"))
    assert_equal(false, OpenSSL::SSL.verify_wildcard("xn--qdk4b9b", "x*"))
    assert_equal(false, OpenSSL::SSL.verify_wildcard("xn--qdk4b9b", "*--qdk4b9b"))
    assert_equal(true,  OpenSSL::SSL.verify_wildcard("xn--qdk4b9b", "xn--qdk4b9b"))
  end

  # Comments in this test is excerpted from http://tools.ietf.org/html/rfc6125#page-27
  def test_post_connection_check_wildcard_san
    # case-insensitive ASCII comparison
    # RFC 6125, section 6.4.1
    #
    # "..matching of the reference identifier against the presented identifier
    # is performed by comparing the set of domain name labels using a
    # case-insensitive ASCII comparison, as clarified by [DNS-CASE] (e.g.,
    # "WWW.Example.Com" would be lower-cased to "www.example.com" for
    # comparison purposes)
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*.example.com'), 'www.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*.Example.COM'), 'www.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*.example.com'), 'WWW.Example.COM'))
    # 1.  The client SHOULD NOT attempt to match a presented identifier in
    #     which the wildcard character comprises a label other than the
    #     left-most label (e.g., do not match bar.*.example.net).
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:www.*.com'), 'www.example.com'))
    # 2.  If the wildcard character is the only character of the left-most
    #     label in the presented identifier, the client SHOULD NOT compare
    #     against anything but the left-most label of the reference
    #     identifier (e.g., *.example.com would match foo.example.com but
    #     not bar.foo.example.com or example.com).
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*.example.com'), 'foo.example.com'))
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*.example.com'), 'bar.foo.example.com'))
    # 3.  The client MAY match a presented identifier in which the wildcard
    #     character is not the only character of the label (e.g.,
    #     baz*.example.net and *baz.example.net and b*z.example.net would
    #     be taken to match baz1.example.net and foobaz.example.net and
    #     buzz.example.net, respectively).  ...
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:baz*.example.com'), 'baz1.example.com'))

    # LibreSSL 3.5.0+ doesn't support other wildcard certificates
    # (it isn't required to, as RFC states MAY, not MUST)
    return if libressl?(3, 5, 0)

    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*baz.example.com'), 'foobaz.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:b*z.example.com'), 'buzz.example.com'))

    # Section 6.4.3 of RFC6125 states that client should NOT match identifier
    # where wildcard is other than left-most label.
    #
    # Also implicitly mentions the wildcard character only in singular form,
    # and discourages matching against more than one wildcard.
    #
    # See RFC 6125, section 7.2, subitem 2.
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*b*.example.com'), 'abc.example.com'))
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*b*.example.com'), 'ab.example.com'))
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:*b*.example.com'), 'bc.example.com'))
    #                                ...  However, the client SHOULD NOT
    #   attempt to match a presented identifier where the wildcard
    #   character is embedded within an A-label or U-label [IDNA-DEFS] of
    #   an internationalized domain name [IDNA-PROTO].
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:xn*.example.com'), 'xn1ca.example.com'))
    # part of A-label
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:xn--*.example.com'), 'xn--1ca.example.com'))
    # part of U-label
    # dNSName in RFC5280 is an IA5String so U-label should NOT be allowed
    # regardless of wildcard.
    #
    # See Section 7.2 of RFC 5280:
    #   IA5String is limited to the set of ASCII characters.
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_san('DNS:á*.example.com'), 'á1.example.com'))
  end

  def test_post_connection_check_wildcard_cn
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*.example.com'), 'www.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*.Example.COM'), 'www.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*.example.com'), 'WWW.Example.COM'))
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('www.*.com'), 'www.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*.example.com'), 'foo.example.com'))
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*.example.com'), 'bar.foo.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('baz*.example.com'), 'baz1.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*baz.example.com'), 'foobaz.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('b*z.example.com'), 'buzz.example.com'))
    # Section 6.4.3 of RFC6125 states that client should NOT match identifier
    # where wildcard is other than left-most label.
    #
    # Also implicitly mentions the wildcard character only in singular form,
    # and discourages matching against more than one wildcard.
    #
    # See RFC 6125, section 7.2, subitem 2.
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*b*.example.com'), 'abc.example.com'))
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*b*.example.com'), 'ab.example.com'))
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('*b*.example.com'), 'bc.example.com'))
    assert_equal(true, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('xn*.example.com'), 'xn1ca.example.com'))
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('xn--*.example.com'), 'xn--1ca.example.com'))
    # part of U-label
    # Subject in RFC5280 states case-insensitive ASCII comparison.
    #
    # See Section 7.2 of RFC 5280:
    #   IA5String is limited to the set of ASCII characters.
    assert_equal(false, OpenSSL::SSL.verify_certificate_identity(
      create_cert_with_name('á*.example.com'), 'á1.example.com'))
  end

  def create_cert_with_san(san)
    ef = OpenSSL::X509::ExtensionFactory.new
    cert = OpenSSL::X509::Certificate.new
    cert.subject = OpenSSL::X509::Name.parse("/DC=some/DC=site/CN=Some Site")
    ext = ef.create_ext('subjectAltName', san)
    cert.add_extension(ext)
    cert
  end

  def create_cert_with_name(name)
    cert = OpenSSL::X509::Certificate.new
    cert.subject = OpenSSL::X509::Name.new([['DC', 'some'], ['DC', 'site'], ['CN', name]])
    cert
  end


  # Create NULL byte SAN certificate
  def create_null_byte_SAN_certificate(critical = false)
    ef = OpenSSL::X509::ExtensionFactory.new
    cert = OpenSSL::X509::Certificate.new
    cert.subject = OpenSSL::X509::Name.parse "/DC=some/DC=site/CN=Some Site"
    ext = ef.create_ext('subjectAltName', 'DNS:placeholder,IP:192.168.7.1,IP:13::17', critical)
    ext_asn1 = OpenSSL::ASN1.decode(ext.to_der)
    san_list_der = ext_asn1.value.reduce(nil) { |memo,val| val.tag == 4 ? val.value : memo }
    san_list_asn1 = OpenSSL::ASN1.decode(san_list_der)
    san_list_asn1.value[0].value = "www.example.com\0.evil.com"
    pos = critical ? 2 : 1
    ext_asn1.value[pos].value = san_list_asn1.to_der
    real_ext = OpenSSL::X509::Extension.new ext_asn1
    cert.add_extension(real_ext)
    cert
  end

  def socketpair
    if defined? UNIXSocket
      UNIXSocket.pair
    else
      Socket.pair(Socket::AF_INET, Socket::SOCK_STREAM, 0)
    end
  end

  def test_tlsext_hostname
    fooctx = OpenSSL::SSL::SSLContext.new
    fooctx.cert = @cli_cert
    fooctx.key = @cli_key

    ctx_proc = proc { |ctx|
      ctx.servername_cb = proc { |ssl, servername|
        case servername
        when "foo.example.com"
          fooctx
        when "bar.example.com"
          nil
        else
          raise "unreachable"
        end
      }
    }
    start_server(ctx_proc: ctx_proc) do |port|
      sock = TCPSocket.new("127.0.0.1", port)
      begin
        ssl = OpenSSL::SSL::SSLSocket.new(sock)
        ssl.hostname = "foo.example.com"
        ssl.connect
        assert_equal @cli_cert.serial, ssl.peer_cert.serial
        assert_predicate fooctx, :frozen?

        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      ensure
        ssl&.close
        sock.close
      end

      sock = TCPSocket.new("127.0.0.1", port)
      begin
        ssl = OpenSSL::SSL::SSLSocket.new(sock)
        ssl.hostname = "bar.example.com"
        ssl.connect
        assert_equal @svr_cert.serial, ssl.peer_cert.serial

        ssl.puts "abc"; assert_equal "abc\n", ssl.gets
      ensure
        ssl&.close
        sock.close
      end
    end
  end

  def test_servername_cb_raises_an_exception_on_unknown_objects
    hostname = 'example.org'

    ctx2 = OpenSSL::SSL::SSLContext.new
    ctx2.cert = @svr_cert
    ctx2.key = @svr_key
    ctx2.servername_cb = lambda { |args| Object.new }

    sock1, sock2 = socketpair

    s2 = OpenSSL::SSL::SSLSocket.new(sock2, ctx2)

    ctx1 = OpenSSL::SSL::SSLContext.new

    s1 = OpenSSL::SSL::SSLSocket.new(sock1, ctx1)
    s1.hostname = hostname
    t = Thread.new {
      assert_raise(OpenSSL::SSL::SSLError) do
        s1.connect
      end
    }

    assert_raise(ArgumentError) do
      s2.accept
    end

    assert t.join
  ensure
    sock1.close if sock1
    sock2.close if sock2
  end

  def test_accept_errors_include_peeraddr
    context = OpenSSL::SSL::SSLContext.new
    context.cert = @svr_cert
    context.key = @svr_key

    server = TCPServer.new("127.0.0.1", 0)
    port = server.connect_address.ip_port

    ssl_server = OpenSSL::SSL::SSLServer.new(server, context)

    t = Thread.new do
      assert_raise_with_message(OpenSSL::SSL::SSLError, /peeraddr=127\.0\.0\.1/) do
        ssl_server.accept
      end
    end

    sock = TCPSocket.new("127.0.0.1", port)
    sock << "\x00" * 1024

    assert t.join
  ensure
    sock&.close
    server.close
  end

  def test_verify_hostname_on_connect
    ctx_proc = proc { |ctx|
      san = "DNS:a.example.com,DNS:*.b.example.com"
      san += ",DNS:c*.example.com,DNS:d.*.example.com" unless libressl?(3, 2, 2)
      exts = [
        ["keyUsage", "keyEncipherment,digitalSignature", true],
        ["subjectAltName", san],
      ]

      ctx.cert = issue_cert(@svr, @svr_key, 4, exts, @ca_cert, @ca_key)
      ctx.key = @svr_key
    }

    start_server(ctx_proc: ctx_proc, ignore_listener_error: true) do |port|
      ctx = OpenSSL::SSL::SSLContext.new
      assert_equal false, ctx.verify_hostname
      ctx.verify_hostname = true
      ctx.cert_store = OpenSSL::X509::Store.new
      ctx.cert_store.add_cert(@ca_cert)
      ctx.verify_mode = OpenSSL::SSL::VERIFY_PEER

      [
        ["a.example.com", true],
        ["A.Example.Com", true],
        ["x.example.com", false],
        ["b.example.com", false],
        ["x.b.example.com", true],
        ["cx.example.com", true],
        ["d.x.example.com", false],
      ].each do |name, expected_ok|
        next if name.start_with?('cx') if libressl?(3, 2, 2)
        begin
          sock = TCPSocket.new("127.0.0.1", port)
          ssl = OpenSSL::SSL::SSLSocket.new(sock, ctx)
          ssl.hostname = name
          if expected_ok
            ssl.connect
            ssl.puts "abc"; assert_equal "abc\n", ssl.gets
          else
            assert_handshake_error { ssl.connect }
          end
        ensure
          ssl.close if ssl
          sock.close if sock
        end
      end
    end
  end

  def test_verify_hostname_failure_error_code
    ctx_proc = proc { |ctx|
      exts = [
        ["keyUsage", "keyEncipherment,digitalSignature", true],
        ["subjectAltName", "DNS:a.example.com"],
      ]
      ctx.cert = issue_cert(@svr, @svr_key, 4, exts, @ca_cert, @ca_key)
      ctx.key = @svr_key
    }

    start_server(ctx_proc: ctx_proc, ignore_listener_error: true) do |port|
      verify_callback_ok = verify_callback_err = nil

      ctx = OpenSSL::SSL::SSLContext.new
      ctx.verify_hostname = true
      ctx.cert_store = OpenSSL::X509::Store.new
      ctx.cert_store.add_cert(@ca_cert)
      ctx.verify_mode = OpenSSL::SSL::VERIFY_PEER
      ctx.verify_callback = -> (preverify_ok, store_ctx) {
        verify_callback_ok = preverify_ok
        verify_callback_err = store_ctx.error
        preverify_ok
      }

      begin
        sock = TCPSocket.new("127.0.0.1", port)
        ssl = OpenSSL::SSL::SSLSocket.new(sock, ctx)
        ssl.hostname = "b.example.com"
        assert_handshake_error { ssl.connect }
        assert_equal false, verify_callback_ok
        assert_equal OpenSSL::X509::V_ERR_HOSTNAME_MISMATCH, verify_callback_err
      ensure
        sock&.close
      end
    end
  end

  def test_connect_certificate_verify_failed_exception_message
    start_server(ignore_listener_error: true) { |port|
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.set_params
      # OpenSSL <= 1.1.0: "self signed certificate in certificate chain"
      # OpenSSL >= 3.0.0: "self-signed certificate in certificate chain"
      assert_raise_with_message(OpenSSL::SSL::SSLError, /self.signed/) {
        server_connect(port, ctx)
      }
    }

    ctx_proc = proc { |ctx|
      now = Time.now
      ctx.cert = issue_cert(@svr, @svr_key, 30, [], @ca_cert, @ca_key,
                            not_before: now - 7200, not_after: now - 3600)
    }
    start_server(ignore_listener_error: true, ctx_proc: ctx_proc) { |port|
      store = OpenSSL::X509::Store.new
      store.add_cert(@ca_cert)
      ctx = OpenSSL::SSL::SSLContext.new
      ctx.set_params(cert_store: store)
      assert_raise_with_message(OpenSSL::SSL::SSLError, /expired/) {
        server_connect(port, ctx)
      }
    }
  end

  def test_unset_OP_ALL
    ctx_proc = Proc.new { |ctx|
      # If OP_DONT_INSERT_EMPTY_FRAGMENTS is not defined, this test is
      # redundant because the default options already are equal to OP_ALL.
      # But it also degrades gracefully, so keep it
      ctx.options = OpenSSL::SSL::OP_ALL
    }
    start_server(ctx_proc: ctx_proc) { |port|
      server_connect(port) { |ssl|
        ssl.puts('hello')
        assert_equal("hello\n", ssl.gets)
      }
    }
  end

  def check_supported_protocol_versions
    possible_versions = [
      OpenSSL::SSL::SSL3_VERSION,
      OpenSSL::SSL::TLS1_VERSION,
      OpenSSL::SSL::TLS1_1_VERSION,
      OpenSSL::SSL::TLS1_2_VERSION,
      # OpenSSL 1.1.1
      defined?(OpenSSL::SSL::TLS1_3_VERSION) && OpenSSL::SSL::TLS1_3_VERSION,
    ].compact

    # Prepare for testing & do sanity check
    supported = []
    possible_versions.each do |ver|
      catch(:unsupported) {
        ctx_proc = proc { |ctx|
          begin
            ctx.min_version = ctx.max_version = ver
          rescue ArgumentError, OpenSSL::SSL::SSLError
            throw :unsupported
          end
        }
        start_server(ctx_proc: ctx_proc, ignore_listener_error: true) do |port|
          begin
            server_connect(port) { |ssl|
              ssl.puts "abc"; assert_equal "abc\n", ssl.gets
            }
          rescue OpenSSL::SSL::SSLError, Errno::ECONNRESET
          else
            supported << ver
          end
        end
      }
    end
    assert_not_empty supported

    supported
  end

  def test_set_params_min_version
    supported = check_supported_protocol_versions
    store = OpenSSL::X509::Store.new
    store.add_cert(@ca_cert)

    if supported.include?(OpenSSL::SSL::SSL3_VERSION)
      # SSLContext#set_params properly disables SSL 3.0 b