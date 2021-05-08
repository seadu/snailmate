
/*
 *
 * Ruby BigDecimal(Variable decimal precision) extension library.
 *
 * Copyright(C) 2002 by Shigeo Kobayashi(shigeo@tinyforest.gr.jp)
 *
 */

/* #define BIGDECIMAL_DEBUG 1 */
#ifdef BIGDECIMAL_DEBUG
# define BIGDECIMAL_ENABLE_VPRINT 1
#endif
#include "bigdecimal.h"
#include "ruby/util.h"

#ifndef BIGDECIMAL_DEBUG
# undef NDEBUG
# define NDEBUG
#endif
#include <assert.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif

#include "bits.h"
#include "static_assert.h"

/* #define ENABLE_NUMERIC_STRING */

#define SIGNED_VALUE_MAX INTPTR_MAX
#define SIGNED_VALUE_MIN INTPTR_MIN
#define MUL_OVERFLOW_SIGNED_VALUE_P(a, b) MUL_OVERFLOW_SIGNED_INTEGER_P(a, b, SIGNED_VALUE_MIN, SIGNED_VALUE_MAX)

VALUE rb_cBigDecimal;
VALUE rb_mBigMath;

static ID id_BigDecimal_exception_mode;
static ID id_BigDecimal_rounding_mode;
static ID id_BigDecimal_precision_limit;

static ID id_up;
static ID id_down;
static ID id_truncate;
static ID id_half_up;
static ID id_default;
static ID id_half_down;
static ID id_half_even;
static ID id_banker;
static ID id_ceiling;
static ID id_ceil;
static ID id_floor;
static ID id_to_r;
static ID id_eq;
static ID id_half;

/* MACRO's to guard objects from GC by keeping them in stack */
#ifdef RBIMPL_ATTR_MAYBE_UNUSED
#define ENTER(n) RBIMPL_ATTR_MAYBE_UNUSED() volatile VALUE vStack[n];int iStack=0
#else
#define ENTER(n) volatile VALUE RB_UNUSED_VAR(vStack[n]);int iStack=0
#endif
#define PUSH(x)  (vStack[iStack++] = (VALUE)(x))
#define SAVE(p)  PUSH((p)->obj)
#define GUARD_OBJ(p,y) ((p)=(y), SAVE(p))

#define BASE_FIG  BIGDECIMAL_COMPONENT_FIGURES
#define BASE      BIGDECIMAL_BASE

#define HALF_BASE (BASE/2)
#define BASE1 (BASE/10)

#define LOG10_2 0.3010299956639812

#ifndef RRATIONAL_ZERO_P
# define RRATIONAL_ZERO_P(x) (FIXNUM_P(rb_rational_num(x)) && \
			      FIX2LONG(rb_rational_num(x)) == 0)
#endif

#ifndef RRATIONAL_NEGATIVE_P
# define RRATIONAL_NEGATIVE_P(x) RTEST(rb_funcall((x), '<', 1, INT2FIX(0)))
#endif

#ifndef DECIMAL_SIZE_OF_BITS
#define DECIMAL_SIZE_OF_BITS(n) (((n) * 3010 + 9998) / 9999)
/* an approximation of ceil(n * log10(2)), upto 65536 at least */
#endif

#ifdef PRIsVALUE
# define RB_OBJ_CLASSNAME(obj) rb_obj_class(obj)
# define RB_OBJ_STRING(obj) (obj)
#else
# define PRIsVALUE "s"
# define RB_OBJ_CLASSNAME(obj) rb_obj_classname(obj)
# define RB_OBJ_STRING(obj) StringValueCStr(obj)
#endif

#define BIGDECIMAL_POSITIVE_P(bd) ((bd)->sign > 0)
#define BIGDECIMAL_NEGATIVE_P(bd) ((bd)->sign < 0)

/*
 * ================== Ruby Interface part ==========================
 */
#define DoSomeOne(x,y,f) rb_num_coerce_bin(x,y,f)

/*
 *   VP routines used in BigDecimal part
 */
static unsigned short VpGetException(void);
static void  VpSetException(unsigned short f);
static void VpCheckException(Real *p, bool always);
static VALUE VpCheckGetValue(Real *p);
static void  VpInternalRound(Real *c, size_t ixDigit, DECDIG vPrev, DECDIG v);
static int   VpLimitRound(Real *c, size_t ixDigit);
static Real *VpCopy(Real *pv, Real const* const x);

#ifdef BIGDECIMAL_ENABLE_VPRINT
static int VPrint(FILE *fp,const char *cntl_chr,Real *a);
#endif

/*
 *  **** BigDecimal part ****
 */

static VALUE BigDecimal_nan(void);
static VALUE BigDecimal_positive_infinity(void);
static VALUE BigDecimal_negative_infinity(void);
static VALUE BigDecimal_positive_zero(void);