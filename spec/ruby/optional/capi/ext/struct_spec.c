#include "ruby.h"
#include "rubyspec.h"

#include "ruby/intern.h"

#ifdef __cplusplus
extern "C" {
#endif

static VALUE struct_spec_rb_struct_aref(VALUE self, VALUE st, VALUE key) {
  return rb_struct_aref(st, key);
}

static VALUE struct_spec_rb_struct_getmember(VALUE self, VALUE st, VALUE key) {
  return rb_struct_getmember(st, SYM2ID(key));
}

