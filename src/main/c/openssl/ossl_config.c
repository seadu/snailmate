/*
 * 'OpenSSL for Ruby' project
 * Copyright (C) 2001-2002  Michal Rokos <m.rokos@sh.cvut.cz>
 * All rights reserved.
 */
/*
 * This program is licensed under the same licence as Ruby.
 * (See the file 'LICENCE'.)
 */
#include "ossl.h"

static VALUE cConfig, eConfigError;

static void
nconf_free(void *conf)
{
    NCONF_free(conf);
}

static const rb_data_type_t ossl_config_type = {
    "OpenSSL/CONF",
    {
        0, nconf_free,
    },
    0, 0, RUBY_TYPED_FREE_IMMEDIATELY,
};

CONF *
GetConfig(VALUE obj)
{
    CONF *conf;

    TypedData_Get_Struct(obj, CONF, &ossl_config_type, conf);
    if (!conf)
        rb_raise(rb_eRuntimeError, "CONF is not initialized");
    return conf;
}

static VALUE
config_s_alloc(VALUE klass)
{
    VALUE obj;
    CONF *conf;

    obj = TypedData_Wrap_Struct(klass, &ossl_config_type, 0);
    conf = NCONF_new(NULL);
    if (!conf)
        ossl_raise(eConfigError, "NCONF_new");
    RTYPEDDATA_DATA(obj) = conf;
    return obj;
}

static void
config_load_bio(CONF *conf, BIO *bio)
{
    long eline = -1;

    if (!NCONF_load_bio(conf, bio, &eline)) {
        BIO_free(bio);
        if (eline <= 0)
            ossl_raise(eConfigError, "wrong config format");
        else
            ossl_raise(eConfigError, "error in line %ld", eline);
    }
    BIO_free(bio);

    /*
     * Clear the error queue even if it is parsed successfully.
     * Particularly, when the .include directive refers to a non-existent file,
     * it is only reported in the error queue.
     */
    ossl_clear_error();
}

/*
 * call-seq:
 *    Config.parse(string) -> OpenSSL::Config
 *
 * Parses a given _string_ as a blob that contains configuration for OpenSSL.
 */
static VALUE
config_s_parse(VALUE klass, VALUE str)
{
    VALUE obj = config_s_alloc(klass);
    CONF *conf = GetConfig(obj);
    BIO *bio;

    bio = ossl_obj2bio(&str);
    config_load_bio(conf, bio); /* Consumes BIO */
    return obj;
}

static VALUE config_get_sections(VALUE self);
static VALUE config_get_section(VALUE self, VALUE section);

/*
 * call-seq:
 *    Config.parse_config(io) -> hash
 *
 * Parses the configuration data read from _io_ and returns the whole content
 * as a Hash.
 */
static VALUE
config_s_parse_config(VALUE klass, VALUE io)
{
    VALUE obj, sections, ret;
    long i;

    obj = config_s_parse(klass, io);
    sections = config_get_sections(obj);
    ret = rb_hash_new();
    for (i = 0; i < RARRAY_LEN(sections); i++) {
        VALUE section = rb_ary_entry(sections, i);
        rb_hash_aset(ret, section, config_get_section(obj, section));
    }
    return ret;
}

/*
 * call-seq:
 *    Config.new(filename) -> OpenSSL::Config
 *
 * Creates an instance of OpenSSL::Config from the content of the file
 * specified by _filename_.
 *
 * This can be used in contexts like OpenSSL::X509::ExtensionFactory.config=
 *
 * This can raise IO exceptions based on the access, or availability of the
 * file. A ConfigError exception may be raised depending on the validity of
 * the data being configured.
 */
static VALUE
config_initialize(int argc, VALUE *argv, VALUE self)
{
    CONF *conf = GetConfig(self);
    VALUE filename;

    /* 0-arguments call has no use-case, but is kept for compatibility */
    rb_scan_args(argc, argv, "01", &filename);
    rb_check_frozen(self);
    if (!NIL_P(filename)) {
        BIO *bio = BIO_new_file(StringValueCStr(filename), "rb");
        if (!bio)
            ossl_raise(eConfigError, "BIO_new_file");
        config_load_bio(conf, bio); /* Consumes BIO */
    }
    return self;
}

static VALUE
config_initialize_copy(VALUE self, VALUE other)
{
    CONF *conf = GetConfig(self);
    VALUE str;
    BIO *bio;

    str = rb_funcall(other, rb_intern("to_s"), 0);
    rb_check_frozen(self);
    bio = ossl_obj2bio(&str);
    config_load_bio(conf, bio); /* Consumes BIO */
    return self;
}

/*
 * call-seq:
 *    config.get_value(section, key) -> string
 *
 * Gets the value of _key_ from the given _section_.
 *
 * Given the following configurating file