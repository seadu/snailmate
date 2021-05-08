#include "ruby.h"
#include "rubyspec.h"

#ifdef __cplusplus
extern "C" {
#endif

static VALUE complex_spec_rb_Complex(VALUE self, VALUE num, VALUE den) {
  return rb_Complex(num, den);
}

static VALUE complex_spec_rb_Complex1(VALUE self, VALUE num) {
  return rb_Complex1(num);
}

static VALUE complex_spec_rb_Complex2(VALUE self, VALUE num, VALUE den) {
  return rb_Complex2(num, den);
}

static VALUE complex_spec_rb_complex_new(VALUE self, VALUE num, VALUE den) {
  return rb_complex_new(num, den);
}

static VALUE complex_spec_rb_complex_new1(VALUE self, VALUE num) {
  return rb_complex_new1(num);
}

static VALUE complex_spec_rb_complex_new2(VALUE self, VALUE num, VALUE den) {
  return rb_complex_new2(num, den);
}

void Init_complex_spec(void) {
  VALUE cls = rb_define_class("CApiComplexSpecs", rb_cObject);
  rb_define_method(cls, "rb_Complex", complex_spec_rb_Complex, 2);
  rb_define_method(cls, "rb_Co