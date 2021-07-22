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
    TypedData_Wrap_Struct((klass), &ossl_engine_type, 0)
#define SetEngine(obj, engine) do { \
    if (!(engine)) { \
	ossl_raise(rb_eRuntimeError, "ENGINE wasn't initialized."); \
    } \
    RTYPEDDATA_DATA(obj) = (engine); \
} while(0)
#define GetEngine(obj, engine) do { \
    TypedData_Get_Struct((obj), ENGINE, &ossl_engine_type, (engine)); \
    if (!(engine)) { \
        ossl_raise(rb_eRuntimeError, "ENGINE wasn't initialized."); \
    } \
} while (0)

/*
 * Classes
 */
/* Document-class: OpenSSL::Engine
 *
 * This class is the access to openssl's ENGINE cryptographic module
 * implementation.
 *
 * See also, https://www.openssl.org/docs/crypto/engine.html
 */
VALUE cEngine;
/* Document-class: OpenSSL::Engine::EngineError
 *
 * This is the generic exception for OpenSSL::Engine related errors
 */
VALUE eEngineError;

/*
 * Private
 */
#if !defined(LIBRESSL_VERSION_NUMBER) && OPENSSL_VERSION_NUMBER >= 0x10100000
#define OSSL_ENGINE_LOAD_IF_MATCH(engine_name, x) \
do{\
  if(!strcmp(#engine_name, RSTRING_PTR(name))){\
    if (OPENSSL_init_crypto(OPENSSL_INIT_ENGINE_##x, NULL))\
      return Qtrue;\
    else\
      ossl_raise(eEngineError, "OPENSSL_init_crypto"); \
  }\
}while(0)
#else
#define OSSL_ENGINE_LOAD_IF_MATCH(engine_name, x)  \
do{\
  if(!strcmp(#engine_name, RSTRING_PTR(name))){\
    ENGINE_load_##engine_name();\
    return Qtrue;\
  }\
}while(0)
#endif

static void
ossl_engine_free(void *engine)
{
    ENGINE_free(engine);
}

static const rb_data_type_t ossl_engine_type = {
    "OpenSSL/Engine",
    {
	0, ossl_engine_free,
    },
    0, 0, RUBY_TYPED_FREE_IMMEDIATELY,
};

/*
 * call-seq:
 *    OpenSSL::Engine.load(name = nil)
 *
 * This method loads engines. If _name_ is nil, then all builtin engines are
 * loaded. Otherwise, the given _name_, as a String,  is loaded if available to
 * your runtime, and returns true. If _name_ is not found, then nil is
 * returned.
 *
 */
static VALUE
ossl_engine_s_load(int argc, VALUE *argv, VALUE klass)
{
    VALUE name;

    rb_scan_args(argc, argv, "01", &name);
    if(NIL_P(name)){
        ENGINE_load_builtin_engines();
        return Qtrue;
    }
    StringValueCStr(name);
#ifdef HAVE_ENGINE_LOAD_DYNAMIC
    OSSL_ENGINE_LOAD_IF_MATCH(dynamic, DYNAMIC);
#endif
#ifndef OPENSSL_NO_STATIC_ENGINE
#ifdef HAVE_ENGINE_LOAD_4758CCA
    OSSL_ENGINE_LOAD_IF_MATCH(4758cca, 4758CCA);
#endif
#ifdef HAVE_ENGINE_LOAD_AEP
    OSSL_ENGINE_LOAD_IF_MATCH(aep, AEP);
#endif
#ifdef HAVE_ENGINE_LOAD_ATALLA
    OSSL_ENGINE_LOAD_IF_MATCH(atalla, ATALLA);
#endif
#ifdef HAVE_ENGINE_LOAD_CHIL
    OSSL_ENGINE_LOAD_IF_MATCH(chil, CHIL);
#endif
#ifdef HAVE_ENGINE_LOAD_CSWIFT
    OSSL_ENGINE_LOAD_IF_MATCH(cswift, CSWIFT);
#endif
#ifdef HAVE_ENGINE_LOAD_NURON
    OSSL_ENGINE_LOAD_IF_MATCH(nuron, NURON);
#endif
#ifdef HAVE_ENGINE_LOAD_SUREWARE
    OSSL_ENGINE_LOAD_IF_MATCH(sureware, SUREWARE);
#endif
#ifdef HAVE_ENGINE_LOAD_UBSEC
    OSSL_ENGINE_LOAD_IF_MATCH(ubsec, UBSEC);
#endif
#ifdef HAVE_ENGINE_LOAD_PADLOCK
    OSSL_ENGINE_LOAD_IF_MATCH(padlock, PADLOCK);
#endif
#ifdef HAVE_ENGINE_LOAD_CAPI
    OSSL_ENGINE_LOAD_IF_MATCH(capi, CAPI);
#endif
#ifdef HAVE_ENGINE_LOAD_GMP
    OSSL_ENGINE_LOAD_IF_MATCH(gmp, GMP);
#endif
#ifdef HAVE_ENGINE_LOAD_GOST
    OSSL_ENGINE_LOAD_IF_MATCH(gost, GOST);
#endif
#endif
#ifdef HAVE_ENGINE_LOAD_CRYPTODEV
    OSSL_ENGINE_LOAD_IF_MATCH(cryptodev, CRYPTODEV);
#endif
    OSSL_ENGINE_LOAD_IF_MATCH(openssl, OPENSSL);
    rb_warning("no such builtin loader for `%"PRIsVALUE"'", name);
    return Qnil;
}

/*
 * call-seq:
 *    OpenSSL::Engine.cleanup
 *
 * It is only necessary to run cleanup when engines are loaded via
 * OpenSSL::Engine.load. However, running cleanup before exit is recommended.
 *
 * Note that this is needed and works only in OpenSSL < 1.1.0.
 */
static VALUE
ossl_engine_s_cleanup(VALUE self)
{
#if defined(LIBRESSL_VERSION_NUMBER) || OPENSSL_VERSION_NUMBER < 0x10100000
    ENGINE_cleanup();
#endif
    return Qnil;
}

/*
 * call-seq:
 *    OpenSSL::Engine.engines -> [engine, ...]
 *
 * Returns an array of currently loaded engines.
 */
static VALUE
ossl_engine_s_engines(VALUE klass)
{
    ENGINE *e;
    VALUE ary, obj;

    ary = rb_ary_new();
    for(e = ENGINE_get_first(); e; e = ENGINE_get_next(e)){
	obj = NewEngine(klass);
	/* Need a ref count of two here because of ENGINE_free being
	 * called internally by OpenSSL when moving to the next ENGINE
	 * and by us when releasing the ENGINE reference */
	ENGINE_up_ref(e);
	SetEngine(obj, e);
        rb_ary_push(ary, obj);
    }

    return ary;
}

/*
 * call-seq:
 *    OpenSSL::Engine.by_id(name) -> engine
 *
 * Fetches the engine as specified by the _id_ String.
 *
 *   OpenSSL::Engine.by_id("openssl")
 *    => #<OpenSSL::Engine id="openssl" name="Software engine support">
 *
 * See OpenSSL::Engine.engines for the currently loaded engines.
 */
static VALUE
ossl_engine_s_by_id(VALUE klass, VALUE id)
{
    ENGINE *e;
    VALUE obj;

    StringValueCStr(id);
    ossl_engine_s_load(1, &id, klass);
    obj = NewEngine(klass);
    if(!(e = ENGINE_by_id(RSTRING_PTR(id))))
	ossl_raise(eEngineError, NULL);
    SetEngine(obj, e);
    if(rb_block_given_p()) rb_yield(obj);
    if(!ENGINE_init(e))
	ossl_raise(eEngineError, NULL);
    ENGINE_ctrl(e, ENGINE_CTRL_SET_PASSWORD_CALLBACK,
		0, NULL, (void(*)(void))ossl_pem_passwd_cb);
    ossl_clear_error();

    return obj;
}

/*
 * call-seq:
 *    engine.id -> string
 *
 * Gets the id for this engine.
 *
 *    OpenSSL::Engine.load
 *    OpenSSL::Engine.engines #=> [#<OpenSSL::Engine#>, ...]
 *    OpenSSL::Engine.engines.first.id
 *	#=> "rsax"
 */
static VALUE
ossl_engine_get_id(VALUE self)
{
    ENGINE *e;
    GetEngine(self, e);
    return rb_str_new2(ENGINE_get_id(e));
}

/*
 * call-seq:
 *    engine.name -> string
 *
 * Get the descriptive name for this engine.
 *
 *    OpenSSL::Engine.load
 *    OpenSSL::Engine.engines #=> [#<OpenSSL::Engine#>, ...]
 *    OpenSSL::Engine.engines.first.name
 *	#=> "RSAX engine support"
 *
 */
static VALUE
ossl_engine_get_name(VALUE self)
{
    ENGINE *e;
    GetEngine(self, e);
    return rb_str_new2(ENGINE_get_name(e));
}

/*
 * call-seq:
 *    engine.finish -> nil
 *
 * Releases all internal structural references for this engine.
 *
 * May raise an EngineError if the engine is unavailable
 */
static VALUE
ossl_engine_finish(VALUE self)
{
    ENGINE *e;

    GetEngine(self, e);
    if(!ENGINE_finish(e)) ossl_raise(eEngineError, NULL);

    return Qnil;
}

/*
 * call-seq:
 *   engine.cipher(name) -> OpenSSL::Cipher
 *
 * Returns a new instance of OpenSSL::Cipher by _name_, if it is available in
 * this engine.
 *
 * An EngineError will be raised if the cipher is unavailable.
 *
 *    e = OpenSSL::Engine.by_id("openssl")
 