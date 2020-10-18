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
#define ossl_asn1_set_indefinite_length(o,v) rb_ivar_set((o),sivINDEFINITE_LENGTH,(v))

VALUE mASN1;
VALUE eASN1Error;

VALUE cASN1Data;
VALUE cASN1Primitive;
VALUE cASN1Constructive;

VALUE cASN1EndOfContent;
VALUE cASN1Boolean;                           /* BOOLEAN           */
VALUE cASN1Integer, cASN1Enumerated;          /* INTEGER           */
VALUE cASN1BitString;                         /* BIT STRING        */
VALUE cASN1OctetString, cASN1UTF8String;      /* STRINGs           */
VALUE cASN1NumericString, cASN1PrintableString;
VALUE cASN1T61String, cASN1VideotexString;
VALUE cASN1IA5String, cASN1GraphicString;
VALUE cASN1ISO64String, cASN1GeneralString;
VALUE cASN1UniversalString, cASN1BMPString;
VALUE cASN1Null;                              /* NULL              */
VALUE cASN1ObjectId;                          /* OBJECT IDENTIFIER */
VALUE cASN1UTCTime, cASN1GeneralizedTime;     /* TIME              */
VALUE cASN1Sequence, cASN1Set;                /* CONSTRUCTIVE      */

static VALUE sym_IMPLICIT, sym_EXPLICIT;
static VALUE sym_UNIVERSAL, sym_APPLICATION, sym_CONTEXT_SPECIFIC, sym_PRIVATE;
static ID sivVALUE, sivTAG, sivTAG_CLASS, sivTAGGING, sivINDEFINITE_LENGTH, sivUNUSED_BITS;
static ID id_each;

/*
 * Ruby to ASN1 converters
 */
static ASN1_BOOLEAN
obj_to_asn1bool(VALUE obj)
{
    if (NIL_P(obj))
	ossl_raise(rb_eTypeError, "Can't convert nil into Boolean");

     return RTEST(obj) ? 0xff : 0x0;
}

static ASN1_INTEGER*
obj_to_asn1int(VALUE obj)
{
    return num_to_asn1integer(obj, NULL);
}

static ASN1_BIT_STRING*
obj_to_asn1bstr(VALUE obj, long unused_bits)
{
    ASN1_BIT_STRING *bstr;

    if (unused_bits < 0 || unused_bits > 7)
	ossl_raise(eASN1Error, "unused_bits for a bitstring value must be in "\
		   "the range 0 to 7");
    StringValue(obj);
    if(!(bstr = ASN1_BIT_STRING_new()))
	ossl_raise(eASN1Error, NULL);
    ASN1_BIT_STRING_set(bstr, (unsigned char *)RSTRING_PTR(obj), RSTRING_LENINT(obj));
    bstr->flags &= ~(ASN1_STRING_FLAG_BITS_LEFT|0x07); /* clear */
    bstr->flags |= ASN1_STRING_FLAG_BITS_LEFT | unused_bits;

    return bstr;
}

static ASN1_STRING*
obj_to_asn1str(VALUE obj)
{
    ASN1_STRING *str;

    StringValue(obj);
    if(!(str = ASN1_STRING_new()))
	ossl_raise(eASN1Error, NULL);
    ASN1_STRING_set(str, RSTRING_PTR(obj), RSTRING_LENINT(obj));

    return str;
}

static ASN1_NULL*
obj_to_asn1null(VALUE obj)
{
    ASN1_NULL *null;

    if(!NIL_P(obj))
	ossl_raise(eASN1Error, "nil expected");
    if(!(null = ASN1_NULL_new()))
	ossl_raise(eASN1Error, NULL);

    return null;
}

static ASN1_OBJECT*
obj_to_asn1obj(VALUE obj)
{
    ASN1_OBJECT *a1obj;

    StringValueCStr(obj);
    a1obj = OBJ_txt2obj(RSTRING_PTR(obj), 0);
    if(!a1obj) a1obj = OBJ_txt2obj(RSTRING_PTR(obj), 1);
    if(!a1obj) ossl_raise(eASN1Error, "invalid OBJECT ID %"PRIsVALUE, obj);

    return a1obj;
}

static ASN1_UTCTIME *
obj_to_asn1utime(VALUE time)
{
    time_t sec;
    ASN1_UTCTIME *t;

    int off_days;

    ossl_time_split(time, &sec, &off_days);
    if (!(t = ASN1_UTCTIME_adj(NULL, sec, off_days, 0)))
	ossl_raise(eASN1Error, NULL);

    return t;
}

static ASN1_GENERALIZEDTIME *
obj_to_asn1gtime(VALUE time)
{
    time_t sec;
    ASN1_GENERALIZEDTIME *t;

    int off_days;

    ossl_time_split(time, &sec, &off_days);
    if (!(t = ASN1_GENERALIZEDTIME_adj(NULL, sec, off_days, 0)))
	ossl_raise(eASN1Error, NULL);

    return t;
}

static ASN1_STRING*
obj_to_asn1derstr(VALUE obj)
{
    ASN1_STRING *a1str;
    VALUE str;

    str = ossl_to_der(obj);
    if(!(a1str = ASN1_STRING_new()))
	ossl_raise(eASN1Error, NULL);
    ASN1_STRING_set(a1str, RSTRING_PTR(str), RSTRING_LENINT(str));

    return a1str;
}

/*
 * DER to Ruby converters
 */
static VALUE
decode_bool(unsigned char* der, long length)
{
    const unsigned char *p = der;

    if (length != 3)
	ossl_raise(eASN1Error, "invalid length for BOOLEAN");
    if (p[0] != 1 || p[1] != 1)
	ossl_raise(eASN1Error, "invalid BOOLEAN");

    return p[2] ? Qtrue : Qfalse;
}

static VALUE
decode_int(unsigned char* der, long length)
{
    ASN1_INTEGER *ai;
    const unsigned char *p;
    VALUE ret;
    int status = 0;

    p = der;
    if(!(ai = d2i_ASN1_INTEGER(NULL, &p, length)))
	ossl_raise(eASN1Error, NULL);
    ret = rb_protect(asn1integer_to_num_i,
		     (VALUE)ai, &status);
    ASN1_INTEGER_free(ai);
    if(status) rb_jump_tag(status);

    return ret;
}

static VALUE
decode_bstr(unsigned char* der, long length, long *unused_bits)
{
    ASN1_BIT_STRING *bstr;
    const unsigned char *p;
    long len;
    VALUE ret;

    p = der;
    if(!(bstr = d2i_ASN1_BIT_STRING(NULL, &p, length)))
	ossl_raise(eASN1Error, NULL);
    len = bstr->length;
    *unused_bits = 0;
    if(bstr->flags & ASN1_STRING_FLAG_BITS_LEFT)
	*unused_bits = bstr->flags & 0x07;
    ret = rb_str_new((const char *)bstr->data, len);
    ASN1_BIT_STRING_free(bstr);

    return ret;
}

static VALUE
decode_enum(unsigned char* der, long length)
{
    ASN1_ENUMERATED *ai;
    const unsigned char *p;
    VALUE ret;
    int status = 0;

    p = der;
    if(!(ai = d2i_ASN1_ENUMERATED(NULL, &p, length)))
	ossl_raise(eASN1Error, NULL);
    ret = rb_protect(asn1integer_to_num_i,
		     (VALUE)ai, &status);
    ASN1_ENUMERATED_free(ai);
    if(status) rb_jump_tag(status);

    return ret;
}

static VALUE
decode_null(unsigned char* der, long length)
{
    ASN1_NULL *null;
    const unsigned char *p;

    p = der;
    if(!(null = d2i_ASN1_NULL(NULL, &p, length)))
	ossl_raise(eASN1Error, NULL);
    ASN1_NULL_free(null);

    return Qnil;
}

static VALUE
decode_obj(unsigned char* der, long length)
{
    ASN1_OBJECT *obj;
    const unsigned char *p;
    VALUE ret;
    int nid;
    BIO *bio;

    p = der;
    if(!(obj = d2i_ASN1_OBJECT(NULL, &p, length)))
	ossl_raise(eASN1Error, NULL);
    if((nid = OBJ_obj2nid(obj)) != NID_undef){
	ASN1_OBJECT_free(obj);
	ret = rb_str_new2(OBJ_nid2sn(nid));
    }
    else{
	if(!(bio = BIO_new(BIO_s_mem()))){
	    ASN1_OBJECT_free(obj);
	    ossl_raise(eASN1Error, NULL);
	}
	i2a_ASN1_OBJECT(bio, obj);
	ASN1_OBJECT_free(obj);
	ret = ossl_membio2str(bio);
    }

    return ret;
}

static VALUE
decode_time(unsigned char* der, long length)
{
    ASN1_TIME *time;
    const unsigned char *p;
    VALUE ret;
    int status = 0;

    p = der;
    if(!(time = d2i_ASN1_TIME(NULL, &p, length)))
	ossl_raise(eASN1Error, NULL);
    ret = rb_protect(asn1time_to_time_i,
		     (VALUE)time, &status);
    ASN1_TIME_free(time);
    if(status) rb_jump_tag(status);

    return ret;
}

static VALUE
decode_eoc(unsigned char *der, long length)
{
    if (length != 2 || !(der[0] == 0x00 && der[1] == 0x00))
	ossl_raise(eASN1Error, NULL);

    return rb_str_new("", 0);
}

/****