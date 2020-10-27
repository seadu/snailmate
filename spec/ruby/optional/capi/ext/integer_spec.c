#include "ruby.h"
#include "rubyspec.h"

#ifdef __cplusplus
extern "C" {
#endif

static VALUE integer_spec_rb_integer_pack(VALUE self, VALUE value,
    VALUE words, VALUE numwords, VALUE wordsize, VALUE nails, VALUE flags)
{
  int result = rb_integer_pack(value, (void*)RSTRING_PTR(words), FIX2INT(numwords),
      FIX2INT(wordsize), FIX2INT(nails), FIX2INT(flags));
  return INT2FIX(result);
}

RUBY_EXTERN VALUE rb_int_positive_pow(long x, unsigned long y); /* internal.h, used in ripper */

static VALUE integer_spec_rb_int_positive_pow(VALUE self, VALUE a, VALUE b){
  return rb_int_positive_pow(FIX2INT(a), FIX2INT(b));
}

void Init_integer_spec(void) {
  VALUE cls = rb_define_class("CApiIntegerSpecs", rb_cObject);
  rb_define_const(cls, "MSWORD", INT2NUM(INTEGER_PACK_MSWORD_FIRST));
  rb_define_const(cls, "LSWORD", INT2NUM(INTEGER_P