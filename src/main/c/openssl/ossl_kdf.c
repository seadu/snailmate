/*
 * Ruby/OpenSSL Project
 * Copyright (C) 2007, 2017 Ruby/OpenSSL Project Authors
 */
#include "ossl.h"
#if OPENSSL_VERSION_NUMBER >= 0x10100000 && !defined(LIBRESSL_VERSION_NUMBER)
# include <openssl/kdf.h>
#endif

static VALUE mKDF, eKDF;

/*
 * call-seq:
 *   KDF.pbkdf2_hmac(pass, salt:, iterations:, length:, hash:) -> aString
 *
 * PKCS #5 PBKDF2 (Password-Based Key Derivation Function 2) in combination
 * with HMAC. Takes _pass_, _salt_ and _iterations_, and then derives a key
 * of _length_ bytes.
 *
 * For more information about PBKDF2, see RFC 2898 Section 5.2
 * (https://tools.ietf.org/html/rfc2898#section-5.2).
 *
 * === Parameters
 * pass       :: The passphrase.
 * salt       :: The salt. Salts prevent attacks based on dictionaries of common
 *               passwords and attacks based on rainbow tables. It is a public
 *               value that can be safely stored along with the password (e.g.
 *               if the derived value is used for password storage).
 * iterations :: The iteration count. This provides the ability to tune the
 *               algorithm. It is better to use the highest count possible for
 *               the maximum resistance to brute-force attacks.
 * length     :: The desired length of the derived key in octets.
 * hash       :: The hash algorithm used with HMAC for the PRF. May be a String
 *               representing the algorithm name, or an instance of
 *               OpenSSL::Digest.
 */
static VALUE
kdf_pbkdf2_hmac(int argc, VALUE *argv, VALUE self)
{
    VALUE pass, salt, opts, kwargs[4], str;
    static