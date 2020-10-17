/*
 * 'OpenSSL for Ruby' team members
 * Copyright (C) 2003
 * All rights reserved.
 */
/*
 * This program is licensed under the same licence as Ruby.
 * (See the file 'LICENCE'.)
 */
#include "ossl.h"

static VALUE ossl_asn1_decode0(unsigned char **pp, long length, long *offset,
			       int depth, int yield, long *num_read);
static VALUE ossl_asn1_initialize(int argc, VALUE *argv, VALUE self);

/*
 * DATE conversion
 */
VALUE
asn1time_to_time(const ASN1_TIME *time)
{
    struct tm tm;
    VALUE argv[6];
    int count;

    memset(&tm, 0, sizeof(struct tm));

    switch (time->type) {
    case V_ASN1_UTCTIME:
	count = sscanf((const char *)time->data, "%2d%2d%2d%2d%2d%2dZ",
		&tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min,
		&tm.tm_sec);

	if (count == 5) {
	    tm.tm_sec = 0;
	} else if (count != 6) {
	    ossl_raise(rb_eTypeError, "bad UTCTIME format: \"%s\"",
		    time->data);
	}
	if (tm.tm_year < 69) {
	    tm.tm_year += 2000;
	} else {
	    tm.tm_year += 1900;
	}
	break;
    case V_ASN1_GENERALIZEDTIME:
	count = sscanf((const char *)time->data, "%4d%2d%2d%2d%2d%2dZ",
		&tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min,
		&tm.tm_sec);
	if (count == 5) {
		tm.tm_sec = 0;
	}
	else if (count != 6) {
		ossl_raise(rb_eTypeError, "bad GENERALIZEDTIME format: \"%s\"",
			time->data);
	}
	break;
    default:
	rb_warning("unknown time format");
        return Qnil;
    }
    argv[0] = INT2NUM(tm.tm_year);
    argv[1] = INT2NUM(tm.tm_mon);
    argv[2] = INT2NUM(tm.tm_mday);
    argv[3] = INT2NUM(tm.tm_hour);
    argv[4] = INT2NUM(tm.tm_min);
    argv[5] = INT2NUM(tm.tm_sec);

    return rb_funcall2(rb_cTime, rb_intern("utc"), 6, argv);
}

static VALUE
asn1time_to_time_i(VALUE arg)
{
    return asn1time_to_time((ASN1_TIME *)arg);
}

void
ossl_time_split(VALUE time, time_t *sec, int *days)
{
    VALUE num = rb_Integer(time);

    if (FIXNUM_P(num)) {
	time_t t = FIX2LONG(num);
	*sec = t % 86400;
	*days = rb_long2int(t / 86400);
    }
    else {
	*days = NUM2INT(rb_funcall(num, rb_intern("/"), 1, INT2FIX(86400)));
	*sec = NUM2TIMET(rb_funcall(num, rb_intern("%"), 1, INT2FIX(86400)));
    }
}

/*
 * STRING conversion
 */
VALUE
asn1str_to_str(const ASN1_STRING *str)
{
    return rb_str_new((const char *)str->data, str->length);
}

/*
 * ASN1_INTEGER conversions
 */
VALUE
asn1integer_to_num(const ASN1_INTEGER *ai)
{
    BIGNUM *bn;
    VALUE num;

    if (!ai) {
	ossl_raise(rb_eTypeError, "ASN1_INTEGER is NULL!");
    }
    if (ai->type == V_ASN1_ENUMERATED)
	/* const_cast: workaround for old OpenSSL */
	bn = ASN1_ENUMERATED_to_BN((ASN1_ENUMERATED *)ai, NULL);
    else
	bn = ASN1_INTEGER_to_BN(ai, NULL);

    if (!bn)
	ossl_raise(eOSSLError, NULL);
    num = ossl_bn_new(bn);
    BN_free(bn);

    return num;
}

ASN1_INTEGER *
num_to_asn1integer(VALUE obj, ASN1_INTEGER *ai)
{
    BIGNUM *bn;

    if (NIL_P(obj))
	ossl_raise(rb_eTypeError, "Can't convert nil into Integer");

    bn = GetBNPtr(obj);

    if (!(ai = BN_to_ASN1_INTEGER(bn, ai)))
	ossl_raise(eOSSLError, NULL);

    return ai;
}

static VALUE
asn1integer_to_num_i(VALUE arg)
{
    return asn1integer_to_num((ASN1_INTEGER *)arg);
}

/********/
/*
 * ASN1 module
 */
#define ossl_asn1_get_value(o)           rb_attr_get((o),sivVALUE)
#define ossl_asn1_get_tag(o)             rb_attr_get((o),sivTAG)
#define ossl_asn1_get_tagging(o)         rb_attr_get((o),sivTAGGING)
#define ossl_asn1_get_tag_class(o)       rb_attr_get((o),sivTAG_CLASS)
#define ossl_asn1_get_indefinite_length(o) rb_attr_get((o),sivINDEFINITE_LENGTH)

#define ossl_asn1_set_value(o,v)           rb_ivar_set((o),sivVALUE,(v))
#define ossl_asn1_set_tag(o,v)             rb_ivar_set((o),sivTAG,(v))
#define ossl_asn1_set_tagging(o,v)         rb_ivar_set((o),sivTAGGING,(v))
#define ossl_asn1_set_tag_class(o,v)       rb_ivar_set((o),sivTAG_CLASS,(v))
#define ossl_asn1_set_indefinite_length(o,v) rb_ivar_