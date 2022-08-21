
#include "ruby.h"
#include "rubyspec.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sample_wrapped_struct {
    int foo;
};

void sample_wrapped_struct_free(void* st) {
  free(st);
}

void sample_wrapped_struct_mark(void* st) {
}

VALUE sdaf_alloc_func(VALUE klass) {
  struct sample_wrapped_struct* bar = (struct sample_wrapped_struct*) malloc(sizeof(struct sample_wrapped_struct));
  bar->foo = 42;
  return Data_Wrap_Struct(klass, &sample_wrapped_struct_mark, &sample_wrapped_struct_free, bar);
}

VALUE sdaf_get_struct(VALUE self) {
  struct sample_wrapped_struct* bar;