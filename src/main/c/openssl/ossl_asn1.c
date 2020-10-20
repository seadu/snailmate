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

/********/

typedef struct {
    const char *name;
    VALUE *klass;
} ossl_asn1_info_t;

static const ossl_asn1_info_t ossl_asn1_info[] = {
    { "EOC",               &cASN1EndOfContent,    },  /*  0 */
    { "BOOLEAN",           &cASN1Boolean,         },  /*  1 */
    { "INTEGER",           &cASN1Integer,         },  /*  2 */
    { "BIT_STRING",        &cASN1BitString,       },  /*  3 */
    { "OCTET_STRING",      &cASN1OctetString,     },  /*  4 */
    { "NULL",              &cASN1Null,            },  /*  5 */
    { "OBJECT",            &cASN1ObjectId,        },  /*  6 */
    { "OBJECT_DESCRIPTOR", NULL,                  },  /*  7 */
    { "EXTERNAL",          NULL,                  },  /*  8 */
    { "REAL",              NULL,                  },  /*  9 */
    { "ENUMERATED",        &cASN1Enumerated,      },  /* 10 */
    { "EMBEDDED_PDV",      NULL,                  },  /* 11 */
    { "UTF8STRING",        &cASN1UTF8String,      },  /* 12 */
    { "RELATIVE_OID",      NULL,                  },  /* 13 */
    { "[UNIVERSAL 14]",    NULL,                  },  /* 14 */
    { "[UNIVERSAL 15]",    NULL,                  },  /* 15 */
    { "SEQUENCE",          &cASN1Sequence,        },  /* 16 */
    { "SET",               &cASN1Set,             },  /* 17 */
    { "NUMERICSTRING",     &cASN1NumericString,   },  /* 18 */
    { "PRINTABLESTRING",   &cASN1PrintableString, },  /* 19 */
    { "T61STRING",         &cASN1T61String,       },  /* 20 */
    { "VIDEOTEXSTRING",    &cASN1VideotexString,  },  /* 21 */
    { "IA5STRING",         &cASN1IA5String,       },  /* 22 */
    { "UTCTIME",           &cASN1UTCTime,         },  /* 23 */
    { "GENERALIZEDTIME",   &cASN1GeneralizedTime, },  /* 24 */
    { "GRAPHICSTRING",     &cASN1GraphicString,   },  /* 25 */
    { "ISO64STRING",       &cASN1ISO64String,     },  /* 26 */
    { "GENERALSTRING",     &cASN1GeneralString,   },  /* 27 */
    { "UNIVERSALSTRING",   &cASN1UniversalString, },  /* 28 */
    { "CHARACTER_STRING",  NULL,                  },  /* 29 */
    { "BMPSTRING",         &cASN1BMPString,       },  /* 30 */
};

enum {ossl_asn1_info_size = (sizeof(ossl_asn1_info)/sizeof(ossl_asn1_info[0]))};

static VALUE class_tag_map;

static int ossl_asn1_default_tag(VALUE obj);

ASN1_TYPE*
ossl_asn1_get_asn1type(VALUE obj)
{
    ASN1_TYPE *ret;
    VALUE value, rflag;
    void *ptr;
    void (*free_func)(void *);
    int tag;

    tag = ossl_asn1_default_tag(obj);
    value = ossl_asn1_get_value(obj);
    switch(tag){
    case V_ASN1_BOOLEAN:
	ptr = (void*)(VALUE)obj_to_asn1bool(value);
	free_func = NULL;
	break;
    case V_ASN1_INTEGER:         /* FALLTHROUGH */
    case V_ASN1_ENUMERATED:
	ptr = obj_to_asn1int(value);
	free_func = ASN1_INTEGER_free;
	break;
    case V_ASN1_BIT_STRING:
        rflag = rb_attr_get(obj, sivUNUSED_BITS);
	ptr = obj_to_asn1bstr(value, NUM2INT(rflag));
	free_func = ASN1_BIT_STRING_free;
	break;
    case V_ASN1_NULL:
	ptr = obj_to_asn1null(value);
	free_func = ASN1_NULL_free;
	break;
    case V_ASN1_OCTET_STRING:    /* FALLTHROUGH */
    case V_ASN1_UTF8STRING:      /* FALLTHROUGH */
    case V_ASN1_NUMERICSTRING:   /* FALLTHROUGH */
    case V_ASN1_PRINTABLESTRING: /* FALLTHROUGH */
    case V_ASN1_T61STRING:       /* FALLTHROUGH */
    case V_ASN1_VIDEOTEXSTRING:  /* FALLTHROUGH */
    case V_ASN1_IA5STRING:       /* FALLTHROUGH */
    case V_ASN1_GRAPHICSTRING:   /* FALLTHROUGH */
    case V_ASN1_ISO64STRING:     /* FALLTHROUGH */
    case V_ASN1_GENERALSTRING:   /* FALLTHROUGH */
    case V_ASN1_UNIVERSALSTRING: /* FALLTHROUGH */
    case V_ASN1_BMPSTRING:
	ptr = obj_to_asn1str(value);
	free_func = ASN1_STRING_free;
	break;
    case V_ASN1_OBJECT:
	ptr = obj_to_asn1obj(value);
	free_func = ASN1_OBJECT_free;
	break;
    case V_ASN1_UTCTIME:
	ptr = obj_to_asn1utime(value);
	free_func = ASN1_TIME_free;
	break;
    case V_ASN1_GENERALIZEDTIME:
	ptr = obj_to_asn1gtime(value);
	free_func = ASN1_TIME_free;
	break;
    case V_ASN1_SET:             /* FALLTHROUGH */
    case V_ASN1_SEQUENCE:
	ptr = obj_to_asn1derstr(obj);
	free_func = ASN1_STRING_free;
	break;
    default:
	ossl_raise(eASN1Error, "unsupported ASN.1 type");
    }
    if(!(ret = OPENSSL_malloc(sizeof(ASN1_TYPE)))){
	if(free_func) free_func(ptr);
	ossl_raise(eASN1Error, "ASN1_TYPE alloc failure");
    }
    memset(ret, 0, sizeof(ASN1_TYPE));
    ASN1_TYPE_set(ret, tag, ptr);

    return ret;
}

static int
ossl_asn1_default_tag(VALUE obj)
{
    VALUE tmp_class, tag;

    tmp_class = CLASS_OF(obj);
    while (!NIL_P(tmp_class)) {
	tag = rb_hash_lookup(class_tag_map, tmp_class);
	if (tag != Qnil)
	    return NUM2INT(tag);
	tmp_class = rb_class_superclass(tmp_class);
    }

    return -1;
}

static int
ossl_asn1_tag(VALUE obj)
{
    VALUE tag;

    tag = ossl_asn1_get_tag(obj);
    if(NIL_P(tag))
	ossl_raise(eASN1Error, "tag number not specified");

    return NUM2INT(tag);
}

static int
ossl_asn1_tag_class(VALUE obj)
{
    VALUE s;

    s = ossl_asn1_get_tag_class(obj);
    if (NIL_P(s) || s == sym_UNIVERSAL)
	return V_ASN1_UNIVERSAL;
    else if (s == sym_APPLICATION)
	return V_ASN1_APPLICATION;
    else if (s == sym_CONTEXT_SPECIFIC)
	return V_ASN1_CONTEXT_SPECIFIC;
    else if (s == sym_PRIVATE)
	return V_ASN1_PRIVATE;
    else
	ossl_raise(eASN1Error, "invalid tag class");
}

static VALUE
ossl_asn1_class2sym(int tc)
{
    if((tc & V_ASN1_PRIVATE) == V_ASN1_PRIVATE)
	return sym_PRIVATE;
    else if((tc & V_ASN1_CONTEXT_SPECIFIC) == V_ASN1_CONTEXT_SPECIFIC)
	return sym_CONTEXT_SPECIFIC;
    else if((tc & V_ASN1_APPLICATION) == V_ASN1_APPLICATION)
	return sym_APPLICATION;
    else
	return sym_UNIVERSAL;
}

/*
 * call-seq:
 *    OpenSSL::ASN1::ASN1Data.new(value, tag, tag_class) => ASN1Data
 *
 * _value_: Please have a look at Constructive and Primitive to see how Ruby
 * types are mapped to ASN.1 types and vice versa.
 *
 * _tag_: An Integer indicating the tag number.
 *
 * _tag_class_: A Symbol indicating the tag class. Please cf. ASN1 for
 * possible values.
 *
 * == Example
 *   asn1_int = OpenSSL::ASN1Data.new(42, 2, :UNIVERSAL) # => Same as OpenSSL::ASN1::Integer.new(42)
 *   tagged_int = OpenSSL::ASN1Data.new(42, 0, :CONTEXT_SPECIFIC) # implicitly 0-tagged INTEGER
 */
static VALUE
ossl_asn1data_initialize(VALUE self, VALUE value, VALUE tag, VALUE tag_class)
{
    if(!SYMBOL_P(tag_class))
	ossl_raise(eASN1Error, "invalid tag class");
    ossl_asn1_set_tag(self, tag);
    ossl_asn1_set_value(self, value);
    ossl_asn1_set_tag_class(self, tag_class);
    ossl_asn1_set_indefinite_length(self, Qfalse);

    return self;
}

static VALUE
to_der_internal(VALUE self, int constructed, int indef_len, VALUE body)
{
    int encoding = constructed ? indef_len ? 2 : 1 : 0;
    int tag_class = ossl_asn1_tag_class(self);
    int tag_number = ossl_asn1_tag(self);
    int default_tag_number = ossl_asn1_default_tag(self);
    int body_length, total_length;
    VALUE str;
    unsigned char *p;

    body_length = RSTRING_LENINT(body);
    if (ossl_asn1_get_tagging(self) == sym_EXPLICIT) {
	int inner_length, e_encoding = indef_len ? 2 : 1;

	if (default_tag_number == -1)
	    ossl_raise(eASN1Error, "explicit tagging of unknown tag");

	inner_length = ASN1_object_size(encoding, body_length, default_tag_number);
	total_length = ASN1_object_size(e_encoding, inner_length, tag_number);
	str = rb_str_new(NULL, total_length);
	p = (unsigned char *)RSTRING_PTR(str);
	/* Put explicit tag */
	ASN1_put_object(&p, e_encoding, inner_length, tag_number, tag_class);
	/* Append inner object */
	ASN1_put_object(&p, encoding, body_length, default_tag_number, V_ASN1_UNIVERSAL);
	memcpy(p, RSTRING_PTR(body), body_length);
	p += body_length;
	if (indef_len) {
	    ASN1_put_eoc(&p); /* For inner object */
	    ASN1_put_eoc(&p); /* For wrapper object */
	}
    }
    else {
	total_length = ASN1_object_size(encoding, body_length, tag_number);
	str = rb_str_new(NULL, total_length);
	p = (unsigned char *)RSTRING_PTR(str);
	ASN1_put_object(&p, encoding, body_length, tag_number, tag_class);
	memcpy(p, RSTRING_PTR(body), body_length);
	p += body_length;
	if (indef_len)
	    ASN1_put_eoc(&p);
    }
    assert(p - (unsigned char *)RSTRING_PTR(str) == total_length);
    return str;
}

static VALUE ossl_asn1prim_to_der(VALUE);
static VALUE ossl_asn1cons_to_der(VALUE);
/*
 * call-seq:
 *    asn1.to_der => DER-encoded String
 *
 * Encodes this ASN1Data into a DER-encoded String value. The result is
 * DER-encoded except for the possibility of indefinite length forms.
 * Indefinite length forms are not allowed in strict DER, so strictly speaking
 * the result of such an encoding would be a BER-encoding.
 */
static VALUE
ossl_asn1data_to_der(VALUE self)
{
    VALUE value = ossl_asn1_get_value(self);

    if (rb_obj_is_kind_of(value, rb_cArray))
	return ossl_asn1cons_to_der(self);
    else {
	if (RTEST(ossl_asn1_get_indefinite_length(self)))
	    ossl_raise(eASN1Error, "indefinite length form cannot be used " \
		       "with primitive encoding");
	return ossl_asn1prim_to_der(self);
    }
}

static VALUE
int_ossl_asn1_decode0_prim(unsigned char **pp, long length, long hlen, int tag,
			   VALUE tc, long *num_read)
{
    VALUE value, asn1data;
    unsigned char *p;
    long flag = 0;

    p = *pp;

    if(tc == sym_UNIVERSAL && tag < ossl_asn1_info_size) {
	switch(tag){
	case V_ASN1_EOC:
	    value = decode_eoc(p, hlen+length);
	    break;
	case V_ASN1_BOOLEAN:
	    value = decode_bool(p, hlen+length);
	    break;
	case V_ASN1_INTEGER:
	    value = decode_int(p, hlen+length);
	    break;
	case V_ASN1_BIT_STRING:
	    value = decode_bstr(p, hlen+length, &flag);
	    break;
	case V_ASN1_NULL:
	    value = decode_null(p, hlen+length);
	    break;
	case V_ASN1_ENUMERATED:
	    value = decode_enum(p, hlen+length);
	    break;
	case V_ASN1_OBJECT:
	    value = decode_obj(p, hlen+length);
	    break;
	case V_ASN1_UTCTIME:           /* FALLTHROUGH */
	case V_ASN1_GENERALIZEDTIME:
	    value = decode_time(p, hlen+length);
	    break;
	default:
	    /* use original value */
	    p += hlen;
	    value = rb_str_new((const char *)p, length);
	    break;
	}
    }
    else {
	p += hlen;
	value = rb_str_new((const char *)p, length);
    }

    *pp += hlen + length;
    *num_read = hlen + length;

    if (tc == sym_UNIVERSAL &&
	tag < ossl_asn1_info_size && ossl_asn1_info[tag].klass) {
	VALUE klass = *ossl_asn1_info[tag].klass;
	VALUE args[4];
	args[0] = value;
	args[1] = INT2NUM(tag);
	args[2] = Qnil;
	args[3] = tc;
	asn1data = rb_obj_alloc(klass);
	ossl_asn1_initialize(4, args, asn1data);
	if(tag == V_ASN1_BIT_STRING){
	    rb_ivar_set(asn1data, sivUNUSED_BITS, LONG2NUM(flag));
	}
    }
    else {
	asn1data = rb_obj_alloc(cASN1Data);
	ossl_asn1data_initialize(asn1data, value, INT2NUM(tag), tc);
    }

    return asn1data;
}

static VALUE
int_ossl_asn1_decode0_cons(unsigned char **pp, long max_len, long length,
			   long *offset, int depth, int yield, int j,
			   int tag, VALUE tc, long *num_read)
{
    VALUE value, asn1data, ary;
    int indefinite;
    long available_len, off = *offset;

    indefinite = (j == 0x21);
    ary = rb_ary_new();

    available_len = indefinite ? max_len : length;
    while (available_len > 0) {
	long inner_read = 0;
	value = ossl_asn1_decode0(pp, available_len, &off, depth + 1, yield, &inner_read);
	*num_read += inner_read;
	available_len -= inner_read;

	if (indefinite &&
	    ossl_asn1_tag(value) == V_ASN1_EOC &&
	    ossl_asn1_get_tag_class(value) == sym_UNIVERSAL) {
	    break;
	}
	rb_ary_push(ary, value);
    }

    if (tc == sym_UNIVERSAL) {
	VALUE args[4];
	if (tag == V_ASN1_SEQUENCE || tag == V_ASN1_SET)
	    asn1data = rb_obj_alloc(*ossl_asn1_info[tag].klass);
	else
	    asn1data = rb_obj_alloc(cASN1Constructive);
	args[0] = ary;
	args[1] = INT2NUM(tag);
	args[2] = Qnil;
	args[3] = tc;
	ossl_asn1_initialize(4, args, asn1data);
    }
    else {
	asn1data = rb_obj_alloc(cASN1Data);
	ossl_asn1data_initialize(asn1data, ary, INT2NUM(tag), tc);
    }

    if (indefinite)
	ossl_asn1_set_indefinite_length(asn1data, Qtrue);
    else
	ossl_asn1_set_indefinite_length(asn1data, Qfalse);

    *offset = off;
    return asn1data;
}

static VALUE
ossl_asn1_decode0(unsigned char **pp, long length, long *offset, int depth,
		  int yield, long *num_read)
{
    unsigned char *start, *p;
    const unsigned char *p0;
    long len = 0, inner_read = 0, off = *offset, hlen;
    int tag, tc, j;
    VALUE asn1data, tag_class;

    p = *pp;
    start = p;
    p0 = p;
    j = ASN1_get_object(&p0, &len, &tag, &tc, length);
    p = (unsigned char *)p0;
    if(j & 0x80) ossl_raise(eASN1Error, NULL);
    if(len > length) ossl_raise(eASN1Error, "value is too short");
    if((tc & V_ASN1_PRIVATE) == V_ASN1_PRIVATE)
	tag_class = sym_PRIVATE;
    else if((tc & V_ASN1_CONTEXT_SPECIFIC) == V_ASN1_CONTEXT_SPECIFIC)
	tag_class = sym_CONTEXT_SPECIFIC;
    else if((tc & V_ASN1_APPLICATION) == V_ASN1_APPLICATION)
	tag_class = sym_APPLICATION;
    else
	tag_class = sym_UNIVERSAL;

    hlen = p - start;

    if(yield) {
	VALUE arg = rb_ary_new();
	rb_ary_push(arg, LONG2NUM(depth));
	rb_ary_push(arg, LONG2NUM(*offset));
	rb_ary_push(arg, LONG2NUM(hlen));
	rb_ary_push(arg, LONG2NUM(len));
	rb_ary_push(arg, (j & V_ASN1_CONSTRUCTED) ? Qtrue : Qfalse);
	rb_ary_push(arg, ossl_asn1_class2sym(tc));
	rb_ary_push(arg, INT2NUM(tag));
	rb_yield(arg);
    }

    if(j & V_ASN1_CONSTRUCTED) {
	*pp += hlen;
	off += hlen;
	asn1data = int_ossl_asn1_decode0_cons(pp, length - hlen, len, &off, depth, yield, j, tag, tag_class, &inner_read);
	inner_read += hlen;
    }
    else {
	if ((j & 0x01) && (len == 0))
	    ossl_raise(eASN1Error, "indefinite length for primitive value");
	asn1data = int_ossl_asn1_decode0_prim(pp, len, hlen, tag, tag_class, &inner_read);
	off += hlen + len;
    }
    if (num_read)
	*num_read = inner_read;
    if (len != 0 && inner_read != hlen + len) {
	ossl_raise(eASN1Error,
		   "Type mismatch. Bytes read: %ld Bytes available: %ld",
		   inner_read, hlen + len);
    }

    *offset = off;
    return asn1data;
}

static void
int_ossl_decode_sanity_check(long len, long read, long offset)
{
    if (len != 0 && (read != len || offset != len)) {
	ossl_raise(eASN1Error,
		   "Type mismatch. Total bytes read: %ld Bytes available: %ld Offset: %ld",
		   read, len, offset);
    }
}

/*
 * call-seq:
 *    OpenSSL::ASN1.traverse(asn1) -> nil
 *
 * If a block is given, it prints out each of the elements encountered.
 * Block parameters are (in that order):
 * * depth: The recursion depth, plus one with each constructed value being encountered (Integer)
 * * offset: Current byte offset (Integer)
 * * header length: Combined length in bytes of the Tag and Length headers. (Integer)
 * * length: The overall remaining length of the entire data (Integer)
 * * constructed: Whether this value is constructed or not (Boolean)
 * * tag_class: Current tag class (Symbol)
 * * tag: The current tag number (Integer)
 *
 * == Example
 *   der = File.binread('asn1data.der')
 *   OpenSSL::ASN1.traverse(der) do | depth, offset, header_len, length, constructed, tag_class, tag|
 *     puts "Depth: #{depth} Offset: #{offset} Length: #{length}"
 *     puts "Header length: #{header_len} Tag: #{tag} Tag class: #{tag_class} Constructed: #{constructed}"
 *   end
 */
static VALUE
ossl_asn1_traverse(VALUE self, VALUE obj)
{
    unsigned char *p;
    VALUE tmp;
    long len, read = 0, offset = 0;

    obj = ossl_to_der_if_possible(obj);
    tmp = rb_str_new4(StringValue(obj));
    p = (unsigned char *)RSTRING_PTR(tmp);
    len = RSTRING_LEN(tmp);
    ossl_asn1_decode0(&p, len, &offset, 0, 1, &read);
    RB_GC_GUARD(tmp);
    int_ossl_decode_sanity_check(len, read, offset);
    return Qnil;
}

/*
 * call-seq:
 *    OpenSSL::ASN1.decode(der) -> ASN1Data
 *
 * Decodes a BER- or DER-encoded value and creates an ASN1Data instance. _der_
 * may be a String or any object that features a +.to_der+ method transforming
 * it into a BER-/DER-encoded String+
 *
 * == Example
 *   der = File.binread('asn1data')
 *   asn1 = OpenSSL::ASN1.decode(der)
 */
static VALUE
ossl_asn1_decode(VALUE self, VALUE obj)
{
    VALUE ret;
    unsigned char *p;
    VALUE tmp;
    long len, read = 0, offset = 0;

    obj = ossl_to_der_if_possible(obj);
    tmp = rb_str_new4(StringValue(obj));
    p = (unsigned char *)RSTRING_PTR(tmp);
    len = RSTRING_LEN(tmp);
    ret = ossl_asn1_decode0(&p, len, &offset, 0, 0, &read);
    RB_GC_GUARD(tmp);
    int_ossl_decode_sanity_check(len, read, offset);
    return ret;
}

/*
 * call-seq:
 *    OpenSSL::ASN1.decode_all(der) -> Array of ASN1Data
 *
 * Similar to #decode with the difference that #decode expects one
 * distinct value represented in _der_. #decode_all on the contrary
 * decodes a sequence of sequential BER/DER values lined up in _der_
 * and returns them as an array.
 *
 * == Example
 *   ders = File.binread('asn1data_seq')
 *   asn1_ary = OpenSSL::ASN1.decode_all(ders)
 */
static VALUE
ossl_asn1_decode_all(VALUE self, VALUE obj)
{
    VALUE ary, val;
    unsigned char *p;
    long len, tmp_len = 0, read = 0, offset = 0;
    VALUE tmp;

    obj = ossl_to_der_if_possible(obj);
    tmp = rb_str_new4(StringValue(obj));
    p = (unsigned char *)RSTRING_PTR(tmp);
    len = RSTRING_LEN(tmp);
    tmp_len = len;
    ary = rb_ary_new();
    while (tmp_len > 0) {
	long tmp_read = 0;
	val = ossl_asn1_decode0(&p, tmp_len, &offset, 0, 0, &tmp_read);
	rb_ary_push(ary, val);
	read += tmp_read;
	tmp_len -= tmp_read;
    }
    RB_GC_GUARD(tmp);
    int_ossl_decode_sanity_check(len, read, offset);
    return ary;
}

/*
 * call-seq:
 *    OpenSSL::ASN1::Primitive.new(value [, tag, tagging, tag_class ]) => Primitive
 *
 * _value_: is mandatory.
 *
 * _tag_: optional, may be specified for tagged values. If no _tag_ is
 * specified, the UNIVERSAL tag corresponding to the Primitive sub-class
 * is used by default.
 *
 * _tagging_: may be used as an encoding hint to encode a value either
 * explicitly or implicitly, see ASN1 for possible values.
 *
 * _tag_class_: if _tag_ and _tagging_ are +nil+ then this is set to
 * +:UNIVERSAL+ by default. If either _tag_ or _tagging_ are set then
 * +:CONTEXT_SPECIFIC+ is used as the default. For possible values please
 * cf. ASN1.
 *
 * == Example
 *   int = OpenSSL::ASN1::Integer.new(42)
 *   zero_tagged_int = OpenSSL::ASN1::Integer.new(42, 0, :IMPLICIT)
 *   private_explicit_zero_tagged_int = OpenSSL::ASN1::Integer.new(42, 0, :EXPLICIT, :PRIVATE)
 */
static VALUE
ossl_asn1_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE value, tag, tagging, tag_class;
    int default_tag;

    rb_scan_args(argc, argv, "13", &value, &tag, &tagging, &tag_class);
    default_tag = ossl_asn1_default_tag(self);

    if (default_tag == -1 || argc > 1) {
	if(NIL_P(tag))
	    ossl_raise(eASN1Error, "must specify tag number");
	if(!NIL_P(tagging) && !SYMBOL_P(tagging))
	    ossl_raise(eASN1Error, "invalid tagging method");
	if(NIL_P(tag_class)) {
	    if (NIL_P(tagging))
		tag_class = sym_UNIVERSAL;
	    else
		tag_class = sym_CONTEXT_SPECIFIC;
	}
	if(!SYMBOL_P(tag_class))
	    ossl_raise(eASN1Error, "invalid tag class");
    }
    else{
	tag = INT2NUM(default_tag);
	tagging = Qnil;
	tag_class = sym_UNIVERSAL;
    }
    ossl_asn1_set_tag(self, tag);
    ossl_asn1_set_value(self, value);
    ossl_asn1_set_tagging(self, tagging);
    ossl_asn1_set_tag_class(self, tag_class);
    ossl_asn1_set_indefinite_length(self, Qfalse);
    if (default_tag == V_ASN1_BIT_STRING)
	rb_ivar_set(self, sivUNUSED_BITS, INT2FIX(0));

    return self;
}

static VALUE
ossl_asn1eoc_initialize(VALUE self) {
    VALUE tag, tagging, tag_class, value;
    tag = INT2FIX(0);
    tagging = Qnil;
    tag_class = sym_UNIVERSAL;
    value = rb_str_new("", 0);
    ossl_asn1_set_tag(self, tag);
    ossl_asn1_set_value(self, value);
    ossl_asn1_set_tagging(self, tagging);
    ossl_asn1_set_tag_class(self, tag_class);
    ossl_asn1_set_indefinite_length(self, Qfalse);
    return self;
}

static VALUE
ossl_asn1eoc_to_der(VALUE self)
{
    return rb_str_new("\0\0", 2);
}

/*
 * call-seq:
 *    asn1.to_der => DER-encoded String
 *
 * See ASN1Data#to_der for details.
 */
static VALUE
ossl_asn1prim_to_der(VALUE self)
{
    ASN1_TYPE *asn1;
    long a