#include "ruby.h"
#include "rubyspec.h"

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

static VALUE float_spec_new_zero(VALUE self) {
  double flt = 0;
  return rb_float_new(flt);
}

static VALUE float_spec_new_point_five(VALUE self) {
  double flt = 0.555;
  return rb_float_new(flt);
}

static VALUE float_spec_rb_Float(VALUE self, VALUE float_str) {
  return rb_Float(float_str);
}

static VALUE float_spec_RFLOAT_VALUE(VALUE self, VALUE float_h) {
  return rb_float_new(RFLOAT_VALUE(float_h));
}

static VALUE float_spec_RB_FLOAT_TYPE_P(VALUE self, VALUE val) {
  if (RB_FLOAT_TYPE_P(val)) {
    return Qtrue;
