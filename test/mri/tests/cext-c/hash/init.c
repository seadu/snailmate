
#include "ruby.h"

#define init(n) {void Init_##n(VALUE klass); Init_##n(klass);}

void
Init_hash(void)
{
    VALUE mBug = rb_define_module("Bug");
    VALUE klass = rb_define_class_under(mBug, "Hash", rb_cHash);
    TEST_INIT_FUNCS(init);
}