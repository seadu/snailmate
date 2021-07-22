/*
 * 'OpenSSL for Ruby' project
 * Copyright (C) 2003  GOTOU Yuuzou <gotoyuzo@notwork.org>
 * All rights reserved.
 */
/*
 * This program is licensed under the same licence as Ruby.
 * (See the file 'LICENCE'.)
 */
#include "ossl.h"

#ifdef OSSL_USE_ENGINE
# include <openssl/engine.h>

#define NewEngine(klass) \
    TypedData_Wrap_Struct((klass), &ossl_engi