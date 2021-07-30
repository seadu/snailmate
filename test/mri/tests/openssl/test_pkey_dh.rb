# frozen_string_literal: true
require_relative 'utils'

if defined?(OpenSSL) && defined?(OpenSSL::PKey::DH)

class OpenSSL::TestPKeyDH < OpenSSL::PKeyTestCase
  NEW_KEYLEN = 2048

  def test_new_empty
    dh = OpenSSL::PKey::DH.new
    assert_equal nil, dh.p
    assert_equal nil, dh.priv_key
  end

  def test_new_generate
    # This test is slow
    dh = OpenSSL::PKey::DH.new(NEW_KEYLEN)
    assert_key(dh)
  end if ENV["OSSL_TEST_ALL"]

  def test_new_break
    assert_nil(OpenSSL::PKey::DH.new(NEW_KEYLEN) { break })
    assert_raise(RuntimeError) 