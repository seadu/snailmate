/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 12 "ripper.y"


#if !YYPURE
# error needs pure parser
#endif
#define YYDEBUG 1
#define YYERROR_VERBOSE 1
#define YYSTACK_USE_ALLOCA 0
#define YYLTYPE rb_code_location_t
#define YYLTYPE_IS_DECLARED 1

#include "ruby/internal/config.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>

struct lex_context;

#include "internal.h"
#include "internal/compile.h"
#include "internal/compilers.h"
#include "internal/complex.h"
#include "internal/error.h"
#include "internal/hash.h"
#include "internal/imemo.h"
#include "internal/io.h"
#include "internal/numeric.h"
#include "internal/parse.h"
#include "internal/rational.h"
#include "internal/re.h"
#include "internal/symbol.h"
#include "internal/thread.h"
#include "internal/variable.h"
#include "node.h"
#include "probes.h"
#include "regenc.h"
#include "ruby/encoding.h"
#include "ruby/regex.h"
#include "ruby/ruby.h"
#include "ruby/st.h"
#include "ruby/util.h"
#include "ruby/ractor.h"
#ifdef TRUFFLERUBY
#include <truffleruby/internal/symbol.h>
#else
#include "symbol.h"
#endif

enum shareability {
    shareable_none,
    shareable_literal,
    shareable_copy,
    shareable_everything,
};

struct lex_context {
    unsigned int in_defined: 1;
    unsigned int in_kwarg: 1;
    unsigned int in_argdef: 1;
    unsigned int in_def: 1;
    unsigned int in_class: 1;
    BITFIELD(enum shareability, shareable_constant_value, 2);
};

#include "parse.h"

#define NO_LEX_CTXT (struct lex_context){0}

#define AREF(ary, i) RARRAY_AREF(ary, i)

#ifndef WARN_PAST_SCOPE
# define WARN_PAST_SCOPE 0
#endif

#define TAB_WIDTH 8

#define yydebug (p->debug)	/* disable the global variable definition */

#define YYMALLOC(size)		rb_parser_malloc(p, (size))
#define YYREALLOC(ptr, size)	rb_parser_realloc(p, (ptr), (size))
#define YYCALLOC(nelem, size)	rb_parser_calloc(p, (nelem), (size))
#define YYFREE(ptr)		rb_parser_free(p, (ptr))
#define YYFPRINTF		rb_parser_printf
#define YY_LOCATION_PRINT(File, loc) \
     rb_parser_printf(p, "%d.%d-%d.%d", \
		      (loc).beg_pos.lineno, (loc).beg_pos.column,\
		      (loc).end_pos.lineno, (loc).end_pos.column)
#define YYLLOC_DEFAULT(Current, Rhs, N)					\
    do									\
      if (N)								\
	{								\
	  (Current).beg_pos = YYRHSLOC(Rhs, 1).beg_pos;			\
	  (Current).end_pos = YYRHSLOC(Rhs, N).end_pos;			\
	}								\
      else								\
        {                                                               \
          (Current).beg_pos = YYRHSLOC(Rhs, 0).end_pos;                 \
          (Current).end_pos = YYRHSLOC(Rhs, 0).end_pos;                 \
        }                                                               \
    while (0)
#define YY_(Msgid) \
    (((Msgid)[0] == 'm') && (strcmp((Msgid), "memory exhausted") == 0) ? \
     "nesting too deep" : (Msgid))

#define RUBY_SET_YYLLOC_FROM_STRTERM_HEREDOC(Current)			\
    rb_parser_set_location_from_strterm_heredoc(p, &p->lex.strterm->u.heredoc, &(Current))
#define RUBY_SET_YYLLOC_OF_NONE(Current)					\
    rb_parser_set_location_of_none(p, &(Current))
#define RUBY_SET_YYLLOC(Current)					\
    rb_parser_set_location(p, &(Current))
#define RUBY_INIT_YYLLOC() \
    { \
	{p->ruby_sourceline, (int)(p->lex.ptok - p->lex.pbeg)}, \
	{p->ruby_sourceline, (int)(p->lex.pcur - p->lex.pbeg)}, \
    }

enum lex_state_bits {
    EXPR_BEG_bit,		/* ignore newline, +/- is a sign. */
    EXPR_END_bit,		/* newline significant, +/- is an operator. */
    EXPR_ENDARG_bit,		/* ditto, and unbound braces. */
    EXPR_ENDFN_bit,		/* ditto, and unbound braces. */
    EXPR_ARG_bit,		/* newline significant, +/- is an operator. */
    EXPR_CMDARG_bit,		/* newline significant, +/- is an operator. */
    EXPR_MID_bit,		/* newline significant, +/- is an operator. */
    EXPR_FNAME_bit,		/* ignore newline, no reserved words. */
    EXPR_DOT_bit,		/* right after `.' or `::', no reserved words. */
    EXPR_CLASS_bit,		/* immediate after `class', no here document. */
    EXPR_LABEL_bit,		/* flag bit, label is allowed. */
    EXPR_LABELED_bit,		/* flag bit, just after a label. */
    EXPR_FITEM_bit,		/* symbol literal as FNAME. */
    EXPR_MAX_STATE
};
/* examine combinations */
enum lex_state_e {
#define DEF_EXPR(n) EXPR_##n = (1 << EXPR_##n##_bit)
    DEF_EXPR(BEG),
    DEF_EXPR(END),
    DEF_EXPR(ENDARG),
    DEF_EXPR(ENDFN),
    DEF_EXPR(ARG),
    DEF_EXPR(CMDARG),
    DEF_EXPR(MID),
    DEF_EXPR(FNAME),
    DEF_EXPR(DOT),
    DEF_EXPR(CLASS),
    DEF_EXPR(LABEL),
    DEF_EXPR(LABELED),
    DEF_EXPR(FITEM),
    EXPR_VALUE = EXPR_BEG,
    EXPR_BEG_ANY  =  (EXPR_BEG | EXPR_MID | EXPR_CLASS),
    EXPR_ARG_ANY  =  (EXPR_ARG | EXPR_CMDARG),
    EXPR_END_ANY  =  (EXPR_END | EXPR_ENDARG | EXPR_ENDFN),
    EXPR_NONE = 0
};
#define IS_lex_state_for(x, ls)	((x) & (ls))
#define IS_lex_state_all_for(x, ls) (((x) & (ls)) == (ls))
#define IS_lex_state(ls)	IS_lex_state_for(p->lex.state, (ls))
#define IS_lex_state_all(ls)	IS_lex_state_all_for(p->lex.state, (ls))

# define SET_LEX_STATE(ls) \
    parser_set_lex_state(p, ls, __LINE__)
static inline enum lex_state_e parser_set_lex_state(struct parser_params *p, enum lex_state_e ls, int line);

typedef VALUE stack_type;

static const rb_code_location_t NULL_LOC = { {0, -1}, {0, -1} };

# define SHOW_BITSTACK(stack, name) (p->debug ? rb_parser_show_bitstack(p, stack, name, __LINE__) : (void)0)
# define BITSTACK_PUSH(stack, n) (((p->stack) = ((p->stack)<<1)|((n)&1)), SHOW_BITSTACK(p->stack, #stack"(push)"))
# define BITSTACK_POP(stack)	 (((p->stack) = (p->stack) >> 1), SHOW_BITSTACK(p->stack, #stack"(pop)"))
# define BITSTACK_SET_P(stack)	 (SHOW_BITSTACK(p->stack, #stack), (p->stack)&1)
# define BITSTACK_SET(stack, n)	 ((p->stack)=(n), SHOW_BITSTACK(p->stack, #stack"(set)"))

/* A flag to identify keyword_do_cond, "do" keyword after condition expression.
   Examples: `while ... do`, `until ... do`, and `for ... in ... do` */
#define COND_PUSH(n)	BITSTACK_PUSH(cond_stack, (n))
#define COND_POP()	BITSTACK_POP(cond_stack)
#define COND_P()	BITSTACK_SET_P(cond_stack)
#define COND_SET(n)	BITSTACK_SET(cond_stack, (n))

/* A flag to identify keyword_do_block; "do" keyword after command_call.
   Example: `foo 1, 2 do`. */
#define CMDARG_PUSH(n)	BITSTACK_PUSH(cmdarg_stack, (n))
#define CMDARG_POP()	BITSTACK_POP(cmdarg_stack)
#define CMDARG_P()	BITSTACK_SET_P(cmdarg_stack)
#define CMDARG_SET(n)	BITSTACK_SET(cmdarg_stack, (n))

struct vtable {
    ID *tbl;
    int pos;
    int capa;
    struct vtable *prev;
};

struct local_vars {
    struct vtable *args;
    struct vtable *vars;
    struct vtable *used;
# if WARN_PAST_SCOPE
    struct vtable *past;
# endif
    struct local_vars *prev;
# ifndef RIPPER
    struct {
	NODE *outer, *inner, *current;
    } numparam;
# endif
};

enum {
    ORDINAL_PARAM = -1,
    NO_PARAM = 0,
    NUMPARAM_MAX = 9,
};

#define NUMPARAM_ID_P(id) numparam_id_p(id)
#define NUMPARAM_ID_TO_IDX(id) (unsigned int)(((id) >> ID_SCOPE_SHIFT) - tNUMPARAM_1 + 1)
#define NUMPARAM_IDX_TO_ID(idx) TOKEN2LOCALID((tNUMPARAM_1 + (idx) - 1))
static int
numparam_id_p(ID id)
{
    if (!is_local_id(id)) return 0;
    unsigned int idx = NUMPARAM_ID_TO_IDX(id);
    return idx > 0 && idx <= NUMPARAM_MAX;
}
static void numparam_name(struct parser_params *p, ID id);

#define DVARS_INHERIT ((void*)1)
#define DVARS_TOPSCOPE NULL
#define DVARS_TERMINAL_P(tbl) ((tbl) == DVARS_INHERIT || (tbl) == DVARS_TOPSCOPE)

typedef struct token_info {
    const char *token;
    rb_code_position_t beg;
    int indent;
    int nonspc;
    struct token_info *next;
} token_info;

typedef struct rb_strterm_struct rb_strterm_t;

/*
    Structure of Lexer Buffer:

 lex.pbeg     lex.ptok     lex.pcur     lex.pend
    |            |            |            |
    |------------+------------+------------|
                 |<---------->|
                     token
*/
struct parser_params {
    rb_imemo_tmpbuf_t *heap;

    YYSTYPE *lval;

    struct {
	rb_strterm_t *strterm;
	VALUE (*gets)(struct parser_params*,VALUE);
	VALUE input;
	VALUE prevline;
	VALUE lastline;
	VALUE nextline;
	const char *pbeg;
	const char *pcur;
	const char *pend;
	const char *ptok;
	union {
	    long ptr;
	    VALUE (*call)(VALUE, int);
	} gets_;
	enum lex_state_e state;
	/* track the nest level of any parens "()[]{}" */
	int paren_nest;
	/* keep p->lex.paren_nest at the beginning of lambda "->" to detect tLAMBEG and keyword_do_LAMBDA */
	int lpar_beg;
	/* track the nest level of only braces "{}" */
	int brace_nest;
    } lex;
    stack_type cond_stack;
    stack_type cmdarg_stack;
    int tokidx;
    int toksiz;
    int tokline;
    int heredoc_end;
    int heredoc_indent;
    int heredoc_line_indent;
    char *tokenbuf;
    struct local_vars *lvtbl;
    st_table *pvtbl;
    st_table *pktbl;
    int line_count;
    int ruby_sourceline;	/* current line no. */
    const char *ruby_sourcefile; /* current source file */
    VALUE ruby_sourcefile_string;
    rb_encoding *enc;
    token_info *token_info;
    VALUE case_labels;
    VALUE compile_option;

    VALUE debug_buffer;
    VALUE debug_output;

    ID cur_arg;

    rb_ast_t *ast;
    int node_id;

    int max_numparam;

    struct lex_context ctxt;

    unsigned int command_start:1;
    unsigned int eofp: 1;
    unsigned int ruby__end__seen: 1;
    unsigned int debug: 1;
    unsigned int has_shebang: 1;
    unsigned int token_seen: 1;
    unsigned int token_info_enabled: 1;
# if WARN_PAST_SCOPE
    unsigned int past_scope_enabled: 1;
# endif
    unsigned int error_p: 1;
    unsigned int cr_seen: 1;

#ifndef RIPPER
    /* Ruby core only */

    unsigned int do_print: 1;
    unsigned int do_loop: 1;
    unsigned int do_chomp: 1;
    unsigned int do_split: 1;
    unsigned int keep_script_lines: 1;

    NODE *eval_tree_begin;
    NODE *eval_tree;
    VALUE error_buffer;
    VALUE debug_lines;
    const struct rb_iseq_struct *parent_iseq;
#else
    /* Ripper only */

    struct {
	VALUE token;
	int line;
	int col;
    } delayed;

    VALUE value;
    VALUE result;
    VALUE parsing_thread;
#endif
};

#define intern_cstr(n,l,en) rb_intern3(n,l,en)

#define STR_NEW(ptr,len) rb_enc_str_new((ptr),(len),p->enc)
#define STR_NEW0() rb_enc_str_new(0,0,p->enc)
#define STR_NEW2(ptr) rb_enc_str_new((ptr),strlen(ptr),p->enc)
#define STR_NEW3(ptr,len,e,func) parser_str_new((ptr),(len),(e),(func),p->enc)
#define TOK_INTERN() intern_cstr(tok(p), toklen(p), p->enc)

static st_table *
push_pvtbl(struct parser_params *p)
{
    st_table *tbl = p->pvtbl;
    p->pvtbl = st_init_numtable();
    return tbl;
}

static void
pop_pvtbl(struct parser_params *p, st_table *tbl)
{
    st_free_table(p->pvtbl);
    p->pvtbl = tbl;
}

static st_table *
push_pktbl(struct parser_params *p)
{
    st_table *tbl = p->pktbl;
    p->pktbl = 0;
    return tbl;
}

static void
pop_pktbl(struct parser_params *p, st_table *tbl)
{
    if (p->pktbl) st_free_table(p->pktbl);
    p->pktbl = tbl;
}

RBIMPL_ATTR_NONNULL((1, 2, 3))
static int parser_yyerror(struct parser_params*, const YYLTYPE *yylloc, const char*);
RBIMPL_ATTR_NONNULL((1, 2))
static int parser_yyerror0(struct parser_params*, const char*);
#define yyerror0(msg) parser_yyerror0(p, (msg))
#define yyerror1(loc, msg) parser_yyerror(p, (loc), (msg))
#define yyerror(yylloc, p, msg) parser_yyerror(p, yylloc, msg)
#define token_flush(ptr) ((ptr)->lex.ptok = (ptr)->lex.pcur)

static void token_info_setup(token_info *ptinfo, const char *ptr, const rb_code_location_t *loc);
static void token_info_push(struct parser_params*, const char *token, const rb_code_location_t *loc);
static void token_info_pop(struct parser_params*, const char *token, const rb_code_location_t *loc);
static void token_info_warn(struct parser_params *p, const char *token, token_info *ptinfo_beg, int same, const rb_code_location_t *loc);
static void token_info_drop(struct parser_params *p, const char *token, rb_code_position_t beg_pos);

#ifdef RIPPER
#define compile_for_eval	(0)
#else
#define compile_for_eval	(p->parent_iseq != 0)
#endif

#define token_column		((int)(p->lex.ptok - p->lex.pbeg))

#define CALL_Q_P(q) ((q) == TOKEN2VAL(tANDDOT))
#define NODE_CALL_Q(q) (CALL_Q_P(q) ? NODE_QCALL : NODE_CALL)
#define NEW_QCALL(q,r,m,a,loc) NEW_NODE(NODE_CALL_Q(q),r,m,a,loc)

#define lambda_beginning_p() (p->lex.lpar_beg == p->lex.paren_nest)

#define ANON_BLOCK_ID '&'

static enum yytokentype yylex(YYSTYPE*, YYLTYPE*, struct parser_params*);

#ifndef RIPPER
static inline void
rb_discard_node(struct parser_params *p, NODE *n)
{
    rb_ast_delete_node(p->ast, n);
}
#endif

#ifdef RIPPER
static inline VALUE
add_mark_object(struct parser_params *p, VALUE obj)
{
    if (!SPECIAL_CONST_P(obj)
	&& !RB_TYPE_P(obj, T_NODE) /* Ripper jumbles NODE objects and other objects... */
    ) {
	rb_ast_add_mark_object(p->ast, obj);
    }
    return obj;
}
#else
static NODE* node_newnode_with_locals(struct parser_params *, enum node_type, VALUE, VALUE, const rb_code_location_t*);
#endif

static NODE* node_newnode(struct parser_params *, enum node_type, VALUE, VALUE, VALUE, const rb_code_location_t*);
#define rb_node_newnode(type, a1, a2, a3, loc) node_newnode(p, (type), (a1), (a2), (a3), (loc))

static NODE *nd_set_loc(NODE *nd, const YYLTYPE *loc);

static int
parser_get_node_id(struct parser_params *p)
{
    int node_id = p->node_id;
    p->node_id++;
    return node_id;
}

#ifndef RIPPER
static inline void
set_line_body(NODE *body, int line)
{
    if (!body) return;
    switch (nd_type(body)) {
      case NODE_RESCUE:
      case NODE_ENSURE:
	nd_set_line(body, line);
    }
}

#define yyparse ruby_yyparse

static NODE* cond(struct parser_params *p, NODE *node, const YYLTYPE *loc);
static NODE* method_cond(struct parser_params *p, NODE *node, const YYLTYPE *loc);
#define new_nil(loc) NEW_NIL(loc)
static NODE *new_nil_at(struct parser_params *p, const rb_code_position_t *pos);
static NODE *new_if(struct parser_params*,NODE*,NODE*,NODE*,const YYLTYPE*);
static NODE *new_unless(struct parser_params*,NODE*,NODE*,NODE*,const YYLTYPE*);
static NODE *logop(struct parser_params*,ID,NODE*,NODE*,const YYLTYPE*,const YYLTYPE*);

static NODE *newline_node(NODE*);
static void fixpos(NODE*,NODE*);

static int value_expr_gen(struct parser_params*,NODE*);
static void void_expr(struct parser_params*,NODE*);
static NODE *remove_begin(NODE*);
static NODE *remove_begin_all(NODE*);
#define value_expr(node) value_expr_gen(p, (node))
static NODE *void_stmts(struct parser_params*,NODE*);
static void reduce_nodes(struct parser_params*,NODE**);
static void block_dup_check(struct parser_params*,NODE*,NODE*);

static NODE *block_append(struct parser_params*,NODE*,NODE*);
static NODE *list_append(struct parser_params*,NODE*,NODE*);
static NODE *list_concat(NODE*,NODE*);
static NODE *arg_append(struct parser_params*,NODE*,NODE*,const YYLTYPE*);
static NODE *last_arg_append(struct parser_params *p, NODE *args, NODE *last_arg, const YYLTYPE *loc);
static NODE *rest_arg_append(struct parser_params *p, NODE *args, NODE *rest_arg, const YYLTYPE *loc);
static NODE *literal_concat(struct parser_params*,NODE*,NODE*,const YYLTYPE*);
static NODE *new_evstr(struct parser_params*,NODE*,const YYLTYPE*);
static NODE *new_dstr(struct parser_params*,NODE*,const YYLTYPE*);
static NODE *evstr2dstr(struct parser_params*,NODE*);
static NODE *splat_array(NODE*);
static void mark_lvar_used(struct parser_params *p, NODE *rhs);

static NODE *call_bin_op(struct parser_params*,NODE*,ID,NODE*,const YYLTYPE*,const YYLTYPE*);
static NODE *call_uni_op(struct parser_params*,NODE*,ID,const YYLTYPE*,const YYLTYPE*);
static NODE *new_qcall(struct parser_params* p, ID atype, NODE *recv, ID mid, NODE *args, const YYLTYPE *op_loc, const YYLTYPE *loc);
static NODE *new_command_qcall(struct parser_params* p, ID atype, NODE *recv, ID mid, NODE *args, NODE *block, const YYLTYPE *op_loc, const YYLTYPE *loc);
static NODE *method_add_block(struct parser_params*p, NODE *m, NODE *b, const YYLTYPE *loc) {b->nd_iter = m; b->nd_loc = *loc; return b;}

static bool args_info_empty_p(struct rb_args_info *args);
static NODE *new_args(struct parser_params*,NODE*,NODE*,ID,NODE*,NODE*,const YYLTYPE*);
static NODE *new_args_tail(struct parser_params*,NODE*,ID,ID,const YYLTYPE*);
static NODE *new_array_pattern(struct parser_params *p, NODE *constant, NODE *pre_arg, NODE *aryptn, const YYLTYPE *loc);
static NODE *new_array_pattern_tail(struct parser_params *p, NODE *pre_args, int has_rest, ID rest_arg, NODE *post_args, const YYLTYPE *loc);
static NODE *new_find_pattern(struct parser_params *p, NODE *constant, NODE *fndptn, const YYLTYPE *loc);
static NODE *new_find_pattern_tail(struct parser_params *p, ID pre_rest_arg, NODE *args, ID post_rest_arg, const YYLTYPE *loc);
static NODE *new_hash_pattern(struct parser_params *p, NODE *constant, NODE *hshptn, const YYLTYPE *loc);
static NODE *new_hash_pattern_tail(struct parser_params *p, NODE *kw_args, ID kw_rest_arg, const YYLTYPE *loc);

static NODE *new_kw_arg(struct parser_params *p, NODE *k, const YYLTYPE *loc);
static NODE *args_with_numbered(struct parser_params*,NODE*,int);

static VALUE negate_lit(struct parser_params*, VALUE);
static NODE *ret_args(struct parser_params*,NODE*);
static NODE *arg_blk_pass(NODE*,NODE*);
static NODE *new_yield(struct parser_params*,NODE*,const YYLTYPE*);
static NODE *dsym_node(struct parser_params*,NODE*,const YYLTYPE*);

static NODE *gettable(struct parser_params*,ID,const YYLTYPE*);
static NODE *assignable(struct parser_params*,ID,NODE*,const YYLTYPE*);

static NODE *aryset(struct parser_params*,NODE*,NODE*,const YYLTYPE*);
static NODE *attrset(struct parser_params*,NODE*,ID,ID,const YYLTYPE*);

static void rb_backref_error(struct parser_params*,NODE*);
static NODE *node_assign(struct parser_params*,NODE*,NODE*,struct lex_context,const YYLTYPE*);

static NODE *new_op_assign(struct parser_params *p, NODE *lhs, ID op, NODE *rhs, struct lex_context, const YYLTYPE *loc);
static NODE *new_ary_op_assign(struct parser_params *p, NODE *ary, NODE *args, ID op, NODE *rhs, const YYLTYPE *args_loc, const YYLTYPE *loc);
static NODE *new_attr_op_assign(struct parser_params *p, NODE *lhs, ID atype, ID attr, ID op, NODE *rhs, const YYLTYPE *loc);
static NODE *new_const_op_assign(struct parser_params *p, NODE *lhs, ID op, NODE *rhs, struct lex_context, const YYLTYPE *loc);
static NODE *new_bodystmt(struct parser_params *p, NODE *head, NODE *rescue, NODE *rescue_else, NODE *ensure, const YYLTYPE *loc);

static NODE *const_decl(struct parser_params *p, NODE* path, const YYLTYPE *loc);

static NODE *opt_arg_append(NODE*, NODE*);
static NODE *kwd_append(NODE*, NODE*);

static NODE *new_hash(struct parser_params *p, NODE *hash, const YYLTYPE *loc);
static NODE *new_unique_key_hash(struct parser_params *p, NODE *hash, const YYLTYPE *loc);

static NODE *new_defined(struct parser_params *p, NODE *expr, const YYLTYPE *loc);

static NODE *new_regexp(struct parser_params *, NODE *, int, const YYLTYPE *);

#define make_list(list, loc) ((list) ? (nd_set_loc(list, loc), list) : NEW_ZLIST(loc))

static NODE *new_xstring(struct parser_params *, NODE *, const YYLTYPE *loc);

static NODE *symbol_append(struct parser_params *p, NODE *symbols, NODE *symbol);

static NODE *match_op(struct parser_params*,NODE*,NODE*,const YYLTYPE*,const YYLTYPE*);

static rb_ast_id_table_t *local_tbl(struct parser_params*);

static VALUE reg_compile(struct parser_params*, VALUE, int);
static void reg_fragment_setenc(struct parser_params*, VALUE, int);
static int reg_fragment_check(struct parser_params*, VALUE, int);
static NODE *reg_named_capture_assign(struct parser_params* p, VALUE regexp, const YYLTYPE *loc);

static int literal_concat0(struct parser_params *p, VALUE head, VALUE tail);
static NODE *heredoc_dedent(struct parser_params*,NODE*);

static void check_literal_when(struct parser_params *p, NODE *args, const YYLTYPE *loc);

#define get_id(id) (id)
#define get_value(val) (val)
#define get_num(num) (num)
#else  /* RIPPER */
#define NODE_RIPPER NODE_CDECL
#define NEW_RIPPER(a,b,c,loc) (VALUE)NEW_CDECL(a,b,c,loc)

static inline int ripper_is_node_yylval(VALUE n);

static inline VALUE
ripper_new_yylval(struct parser_params *p, ID a, VALUE b, VALUE c)
{
    if (ripper_is_node_yylval(c)) c = RNODE(c)->nd_cval;
    add_mark_object(p, b);
    add_mark_object(p, c);
    return NEW_RIPPER(a, b, c, &NULL_LOC);
}

static inline int
ripper_is_node_yylval(VALUE n)
{
    return RB_TYPE_P(n, T_NODE) && nd_type_p(RNODE(n), NODE_RIPPER);
}

#define value_expr(node) ((void)(node))
#define remove_begin(node) (node)
#define void_stmts(p,x) (x)
#define rb_dvar_defined(id, base) 0
#define rb_local_defined(id, base) 0
static ID ripper_get_id(VALUE);
#define get_id(id) ripper_get_id(id)
static VALUE ripper_get_value(VALUE);
#define get_value(val) ripper_get_value(val)
#define get_num(num) (int)get_id(num)
static VALUE assignable(struct parser_params*,VALUE);
static int id_is_var(struct parser_params *p, ID id);

#define method_cond(p,node,loc) (node)
#define call_bin_op(p, recv,id,arg1,op_loc,loc) dispatch3(binary, (recv), STATIC_ID2SYM(id), (arg1))
#define match_op(p,node1,node2,op_loc,loc) call_bin_op(0, (node1), idEqTilde, (node2), op_loc, loc)
#define call_uni_op(p, recv,id,op_loc,loc) dispatch2(unary, STATIC_ID2SYM(id), (recv))
#define logop(p,id,node1,node2,op_loc,loc) call_bin_op(0, (node1), (id), (node2), op_loc, loc)

#define new_nil(loc) Qnil

static VALUE new_regexp(struct parser_params *, VALUE, VALUE, const YYLTYPE *);

static VALUE const_decl(struct parser_params *p, VALUE path);

static VALUE var_field(struct parser_params *p, VALUE a);
static VALUE assign_error(struct parser_params *p, const char *mesg, VALUE a);

static VALUE parser_reg_compile(struct parser_params*, VALUE, int, VALUE *);

static VALUE backref_error(struct parser_params*, NODE *, VALUE);
#endif /* !RIPPER */

/* forward declaration */
typedef struct rb_strterm_heredoc_struct rb_strterm_heredoc_t;

RUBY_SYMBOL_EXPORT_BEGIN
VALUE rb_parser_reg_compile(struct parser_params* p, VALUE str, int options);
int rb_reg_fragment_setenc(struct parser_params*, VALUE, int);
enum lex_state_e rb_parser_trace_lex_state(struct parser_params *, enum lex_state_e, enum lex_state_e, int);
VALUE rb_parser_lex_state_name(enum lex_state_e state);
void rb_parser_show_bitstack(struct parser_params *, stack_type, const char *, int);
PRINTF_ARGS(void rb_parser_fatal(struct parser_params *p, const char *fmt, ...), 2, 3);
YYLTYPE *rb_parser_set_location_from_strterm_heredoc(struct parser_params *p, rb_strterm_heredoc_t *here, YYLTYPE *yylloc);
YYLTYPE *rb_parser_set_location_of_none(struct parser_params *p, YYLTYPE *yylloc);
YYLTYPE *rb_parser_set_location(struct parser_params *p, YYLTYPE *yylloc);
RUBY_SYMBOL_EXPORT_END

static void error_duplicate_pattern_variable(struct parser_params *p, ID id, const YYLTYPE *loc);
static void error_duplicate_pattern_key(struct parser_params *p, ID id, const YYLTYPE *loc);
#ifndef RIPPER
static ID formal_argument(struct parser_params*, ID);
#else
static ID formal_argument(struct parser_params*, VALUE);
#endif
static ID shadowing_lvar(struct parser_params*,ID);
static void new_bv(struct parser_params*,ID);

static void local_push(struct parser_params*,int);
static void local_pop(struct parser_params*);
static void local_var(struct parser_params*, ID);
static void arg_var(struct parser_params*, ID);
static int  local_id(struct parser_params *p, ID id);
static int  local_id_ref(struct parser_params*, ID, ID **);
#ifndef RIPPER
static ID   internal_id(struct parser_params*);
static NODE *new_args_forward_call(struct parser_params*, NODE*, const YYLTYPE*, const YYLTYPE*);
#endif
static int check_forwarding_args(struct parser_params*);
static void add_forwarding_args(struct parser_params *p);

static const struct vtable *dyna_push(struct parser_params *);
static void dyna_pop(struct parser_params*, const struct vtable *);
static int dyna_in_block(struct parser_params*);
#define dyna_var(p, id) local_var(p, id)
static int dvar_defined(struct parser_params*, ID);
static int dvar_defined_ref(struct parser_params*, ID, ID**);
static int dvar_curr(struct parser_params*,ID);

static int lvar_defined(struct parser_params*, ID);

static NODE *numparam_push(struct parser_params *p);
static void numparam_pop(struct parser_params *p, NODE *prev_inner);

#ifdef RIPPER
# define METHOD_NOT idNOT
#else
# define METHOD_NOT '!'
#endif

#define idFWD_REST   '*'
#ifdef RUBY3_KEYWORDS
#define idFWD_KWREST idPow /* Use simple "**", as tDSTAR is "**arg" */
#else
#define idFWD_KWREST 0
#endif
#define idFWD_BLOCK  '&'

#define RE_OPTION_ONCE (1<<16)
#define RE_OPTION_ENCODING_SHIFT 8
#define RE_OPTION_ENCODING(e) (((e)&0xff)<<RE_OPTION_ENCODING_SHIFT)
#define RE_OPTION_ENCODING_IDX(o) (((o)>>RE_OPTION_ENCODING_SHIFT)&0xff)
#define RE_OPTION_ENCODING_NONE(o) ((o)&RE_OPTION_ARG_ENCODING_NONE)
#define RE_OPTION_MASK  0xff
#define RE_OPTION_ARG_ENCODING_NONE 32

/* structs for managing terminator of string literal and heredocment */
typedef struct rb_strterm_literal_struct {
    union {
	VALUE dummy;
	long nest;
    } u0;
    union {
	VALUE dummy;
	long func;	    /* STR_FUNC_* (e.g., STR_FUNC_ESCAPE and STR_FUNC_EXPAND) */
    } u1;
    union {
	VALUE dummy;
	long paren;	    /* '(' of `%q(...)` */
    } u2;
    union {
	VALUE dummy;
	long term;	    /* ')' of `%q(...)` */
    } u3;
} rb_strterm_literal_t;

#define HERETERM_LENGTH_BITS ((SIZEOF_VALUE - 1) * CHAR_BIT - 1)

struct rb_strterm_heredoc_struct {
    VALUE lastline;	/* the string of line that contains `<<"END"` */
    long offset;	/* the column of END in `<<"END"` */
    int sourceline;	/* lineno of the line that contains `<<"END"` */
    unsigned length	/* the length of END in `<<"END"` */
#if HERETERM_LENGTH_BITS < SIZEOF_INT * CHAR_BIT
    : HERETERM_LENGTH_BITS
# define HERETERM_LENGTH_MAX ((1U << HERETERM_LENGTH_BITS) - 1)
#else
# define HERETERM_LENGTH_MAX UINT_MAX
#endif
    ;
#if HERETERM_LENGTH_BITS < SIZEOF_INT * CHAR_BIT
    unsigned quote: 1;
    unsigned func: 8;
#else
    uint8_t quote;
    uint8_t func;
#endif
};
STATIC_ASSERT(rb_strterm_heredoc_t, sizeof(rb_strterm_heredoc_t) <= 4 * SIZEOF_VALUE);

#define STRTERM_HEREDOC IMEMO_FL_USER0

struct rb_strterm_struct {
    VALUE flags;
    union {
	rb_strterm_literal_t literal;
	rb_strterm_heredoc_t heredoc;
    } u;
};

#ifndef RIPPER
void
rb_strterm_mark(VALUE obj)
{
    rb_strterm_t *strterm = (rb_strterm_t*)obj;
    if (RBASIC(obj)->flags & STRTERM_HEREDOC) {
	rb_strterm_heredoc_t *heredoc = &strterm->u.heredoc;
	rb_gc_mark(heredoc->lastline);
    }
}
#endif

#define yytnamerr(yyres, yystr) (YYSIZE_T)rb_yytnamerr(p, yyres, yystr)
size_t rb_yytnamerr(struct parser_params *p, char *yyres, const char *yystr);

#define TOKEN2ID(tok) ( \
    tTOKEN_LOCAL_BEGIN<(tok)&&(tok)<tTOKEN_LOCAL_END ? TOKEN2LOCALID(tok) : \
    tTOKEN_INSTANCE_BEGIN<(tok)&&(tok)<tTOKEN_INSTANCE_END ? TOKEN2INSTANCEID(tok) : \
    tTOKEN_GLOBAL_BEGIN<(tok)&&(tok)<tTOKEN_GLOBAL_END ? TOKEN2GLOBALID(tok) : \
    tTOKEN_CONST_BEGIN<(tok)&&(tok)<tTOKEN_CONST_END ? TOKEN2CONSTID(tok) : \
    tTOKEN_CLASS_BEGIN<(tok)&&(tok)<tTOKEN_CLASS_END ? TOKEN2CLASSID(tok) : \
    tTOKEN_ATTRSET_BEGIN<(tok)&&(tok)<tTOKEN_ATTRSET_END ? TOKEN2ATTRSETID(tok) : \
    ((tok) / ((tok)<tPRESERVED_ID_END && ((tok)>=128 || rb_ispunct(tok)))))

/****** Ripper *******/

#ifdef RIPPER
#define RIPPER_VERSION "0.1.0"

static inline VALUE intern_sym(const char *name);

#include "eventids1.c"
#include "eventids2.c"

static VALUE ripper_dispatch0(struct parser_params*,ID);
static VALUE ripper_dispatch1(struct parser_params*,ID,VALUE);
static VALUE ripper_dispatch2(struct parser_params*,ID,VALUE,VALUE);
static VALUE ripper_dispatch3(struct parser_params*,ID,VALUE,VALUE,VALUE);
static VALUE ripper_dispatch4(struct parser_params*,ID,VALUE,VALUE,VALUE,VALUE);
static VALUE ripper_dispatch5(struct parser_params*,ID,VALUE,VALUE,VALUE,VALUE,VALUE);
static VALUE ripper_dispatch7(struct parser_params*,ID,VALUE,VALUE,VALUE,VALUE,VALUE,VALUE,VALUE);
static void ripper_error(struct parser_params *p);

#define dispatch0(n)            ripper_dispatch0(p, TOKEN_PASTE(ripper_id_, n))
#define dispatch1(n,a)          ripper_dispatch1(p, TOKEN_PASTE(ripper_id_, n), (a))
#define dispatch2(n,a,b)        ripper_dispatch2(p, TOKEN_PASTE(ripper_id_, n), (a), (b))
#define dispatch3(n,a,b,c)      ripper_dispatch3(p, TOKEN_PASTE(ripper_id_, n), (a), (b), (c))
#define dispatch4(n,a,b,c,d)    ripper_dispatch4(p, TOKEN_PASTE(ripper_id_, n), (a), (b), (c), (d))
#define dispatch5(n,a,b,c,d,e)  ripper_dispatch5(p, TOKEN_PASTE(ripper_id_, n), (a), (b), (c), (d), (e))
#define dispatch7(n,a,b,c,d,e,f,g) ripper_dispatch7(p, TOKEN_PASTE(ripper_id_, n), (a), (b), (c), (d), (e), (f), (g))

#define yyparse ripper_yyparse

#define ID2VAL(id) STATIC_ID2SYM(id)
#define TOKEN2VAL(t) ID2VAL(TOKEN2ID(t))
#define KWD2EID(t, v) ripper_new_yylval(p, keyword_##t, get_value(v), 0)

#define params_new(pars, opts, rest, pars2, kws, kwrest, blk) \
        dispatch7(params, (pars), (opts), (rest), (pars2), (kws), (kwrest), (blk))

#define escape_Qundef(x) ((x)==Qundef ? Qnil : (x))

static inline VALUE
new_args(struct parser_params *p, VALUE pre_args, VALUE opt_args, VALUE rest_arg, VALUE post_args, VALUE tail, YYLTYPE *loc)
{
    NODE *t = (NODE *)tail;
    VALUE kw_args = t->u1.value, kw_rest_arg = t->u2.value, block = t->u3.value;
    return params_new(pre_args, opt_args, rest_arg, post_args, kw_args, kw_rest_arg, escape_Qundef(block));
}

static inline VALUE
new_args_tail(struct parser_params *p, VALUE kw_args, VALUE kw_rest_arg, VALUE block, YYLTYPE *loc)
{
    NODE *t = rb_node_newnode(NODE_ARGS_AUX, kw_args, kw_rest_arg, block, &NULL_LOC);
    add_mark_object(p, kw_args);
    add_mark_object(p, kw_rest_arg);
    add_mark_object(p, block);
    return (VALUE)t;
}

static inline VALUE
args_with_numbered(struct parser_params *p, VALUE args, int max_numparam)
{
    return args;
}

static VALUE
new_array_pattern(struct parser_params *p, VALUE constant, VALUE pre_arg, VALUE aryptn, const YYLTYPE *loc)
{
    NODE *t = (NODE *)aryptn;
    VALUE pre_args = t->u1.value, rest_arg = t->u2.value, post_args = t->u3.value;

    if (!NIL_P(pre_arg)) {
	if (!NIL_P(pre_args)) {
	    rb_ary_unshift(pre_args, pre_arg);
	}
	else {
	    pre_args = rb_ary_new_from_args(1, pre_arg);
	}
    }
    return dispatch4(aryptn, constant, pre_args, rest_arg, post_args);
}

static VALUE
new_array_pattern_tail(struct parser_params *p, VALUE pre_args, VALUE has_rest, VALUE rest_arg, VALUE post_args, const YYLTYPE *loc)
{
    NODE *t;

    if (has_rest) {
	rest_arg = dispatch1(var_field, rest_arg ? rest_arg : Qnil);
    }
    else {
	rest_arg = Qnil;
    }

    t = rb_node_newnode(NODE_ARYPTN, pre_args, rest_arg, post_args, &NULL_LOC);
    add_mark_object(p, pre_args);
    add_mark_object(p, rest_arg);
    add_mark_object(p, post_args);
    return (VALUE)t;
}

static VALUE
new_find_pattern(struct parser_params *p, VALUE constant, VALUE fndptn, const YYLTYPE *loc)
{
    NODE *t = (NODE *)fndptn;
    VALUE pre_rest_arg = t->u1.value, args = t->u2.value, post_rest_arg = t->u3.value;

    return dispatch4(fndptn, constant, pre_rest_arg, args, post_rest_arg);
}

static VALUE
new_find_pattern_tail(struct parser_params *p, VALUE pre_rest_arg, VALUE args, VALUE post_rest_arg, const YYLTYPE *loc)
{
    NODE *t;

    pre_rest_arg = dispatch1(var_field, pre_rest_arg ? pre_rest_arg : Qnil);
    post_rest_arg = dispatch1(var_field, post_rest_arg ? post_rest_arg : Qnil);

    t = rb_node_newnode(NODE_FNDPTN, pre_rest_arg, args, post_rest_arg, &NULL_LOC);
    add_mark_object(p, pre_rest_arg);
    add_mark_object(p, args);
    add_mark_object(p, post_rest_arg);
    return (VALUE)t;
}

#define new_hash(p,h,l) rb_ary_new_from_args(0)

static VALUE
new_unique_key_hash(struct parser_params *p, VALUE ary, const YYLTYPE *loc)
{
    return ary;
}

static VALUE
new_hash_pattern(struct parser_params *p, VALUE constant, VALUE hshptn, const YYLTYPE *loc)
{
    NODE *t = (NODE *)hshptn;
    VALUE kw_args = t->u1.value, kw_rest_arg = t->u2.value;
    return dispatch3(hshptn, constant, kw_args, kw_rest_arg);
}

static VALUE
new_hash_pattern_tail(struct parser_params *p, VALUE kw_args, VALUE kw_rest_arg, const YYLTYPE *loc)
{
    NODE *t;
    if (kw_rest_arg) {
	kw_rest_arg = dispatch1(var_field, kw_rest_arg);
    }
    else {
	kw_rest_arg = Qnil;
    }
    t = rb_node_newnode(NODE_HSHPTN, kw_args, kw_rest_arg, 0, &NULL_LOC);

    add_mark_object(p, kw_args);
    add_mark_object(p, kw_rest_arg);
    return (VALUE)t;
}

#define new_defined(p,expr,loc) dispatch1(defined, (expr))

static VALUE heredoc_dedent(struct parser_params*,VALUE);

#else
#define ID2VAL(id) (id)
#define TOKEN2VAL(t) ID2VAL(t)
#define KWD2EID(t, v) keyword_##t

static NODE *
set_defun_body(struct parser_params *p, NODE *n, NODE *args, NODE *body, const YYLTYPE *loc)
{
    body = remove_begin(body);
    reduce_nodes(p, &body);
    n->nd_defn = NEW_SCOPE(args, body, loc);
    n->nd_loc = *loc;
    nd_set_line(n->nd_defn, loc->end_pos.lineno);
    set_line_body(body, loc->beg_pos.lineno);
    return n;
}

static NODE *
rescued_expr(struct parser_params *p, NODE *arg, NODE *rescue,
	     const YYLTYPE *arg_loc, const YYLTYPE *mod_loc, const YYLTYPE *res_loc)
{
    YYLTYPE loc = code_loc_gen(mod_loc, res_loc);
    rescue = NEW_RESBODY(0, remove_begin(rescue), 0, &loc);
    loc.beg_pos = arg_loc->beg_pos;
    return NEW_RESCUE(arg, rescue, 0, &loc);
}

#endif /* RIPPER */

static void
restore_defun(struct parser_params *p, NODE *name)
{
    YYSTYPE c = {.val = name->nd_cval};
    p->cur_arg = name->nd_vid;
    p->ctxt.in_def = c.ctxt.in_def;
    p->ctxt.shareable_constant_value = c.ctxt.shareable_constant_value;
}

static void
endless_method_name(struct parser_params *p, NODE *defn, const YYLTYPE *loc)
{
#ifdef RIPPER
    defn = defn->nd_defn;
#endif
    ID mid = defn->nd_mid;
    if (is_attrset_id(mid)) {
	yyerror1(loc, "setter method cannot be defined in an endless method definition");
    }
    token_info_drop(p, "def", loc->beg_pos);
}

#ifndef RIPPER
# define Qnone 0
# define Qnull 0
# define ifndef_ripper(x) (x)
#else
# define Qnone Qnil
# define Qnull Qundef
# define ifndef_ripper(x)
#endif

# define rb_warn0(fmt)         WARN_CALL(WARN_ARGS(fmt, 1))
# define rb_warn1(fmt,a)       WARN_CALL(WARN_ARGS(fmt, 2), (a))
# define rb_warn2(fmt,a,b)     WARN_CALL(WARN_ARGS(fmt, 3), (a), (b))
# define rb_warn3(fmt,a,b,c)   WARN_CALL(WARN_ARGS(fmt, 4), (a), (b), (c))
# define rb_warn4(fmt,a,b,c,d) WARN_CALL(WARN_ARGS(fmt, 5), (a), (b), (c), (d))
# define rb_warning0(fmt)         WARNING_CALL(WARNING_ARGS(fmt, 1))
# define rb_warning1(fmt,a)       WARNING_CALL(WARNING_ARGS(fmt, 2), (a))
# define rb_warning2(fmt,a,b)     WARNING_CALL(WARNING_ARGS(fmt, 3), (a), (b))
# define rb_warning3(fmt,a,b,c)   WARNING_CALL(WARNING_ARGS(fmt, 4), (a), (b), (c))
# define rb_warning4(fmt,a,b,c,d) WARNING_CALL(WARNING_ARGS(fmt, 5), (a), (b), (c), (d))
# define rb_warn0L(l,fmt)         WARN_CALL(WARN_ARGS_L(l, fmt, 1))
# define rb_warn1L(l,fmt,a)       WARN_CALL(WARN_ARGS_L(l, fmt, 2), (a))
# define rb_warn2L(l,fmt,a,b)     WARN_CALL(WARN_ARGS_L(l, fmt, 3), (a), (b))
# define rb_warn3L(l,fmt,a,b,c)   WARN_CALL(WARN_ARGS_L(l, fmt, 4), (a), (b), (c))
# define rb_warn4L(l,fmt,a,b,c,d) WARN_CALL(WARN_ARGS_L(l, fmt, 5), (a), (b), (c), (d))
# define rb_warning0L(l,fmt)         WARNING_CALL(WARNING_ARGS_L(l, fmt, 1))
# define rb_warning1L(l,fmt,a)       WARNING_CALL(WARNING_ARGS_L(l, fmt, 2), (a))
# define rb_warning2L(l,fmt,a,b)     WARNING_CALL(WARNING_ARGS_L(l, fmt, 3), (a), (b))
# define rb_warning3L(l,fmt,a,b,c)   WARNING_CALL(WARNING_ARGS_L(l, fmt, 4), (a), (b), (c))
# define rb_warning4L(l,fmt,a,b,c,d) WARNING_CALL(WARNING_ARGS_L(l, fmt, 5), (a), (b), (c), (d))
#ifdef RIPPER
static ID id_warn, id_warning, id_gets, id_assoc;
# define ERR_MESG() STR_NEW2(mesg) /* to bypass Ripper DSL */
# define WARN_S_L(s,l) STR_NEW(s,l)
# define WARN_S(s) STR_NEW2(s)
# define WARN_I(i) INT2NUM(i)
# define WARN_ID(i) rb_id2str(i)
# define WARN_IVAL(i) i
# define PRIsWARN "s"
# define rb_warn0L_experimental(l,fmt)         WARN_CALL(WARN_ARGS_L(l, fmt, 1))
# define WARN_ARGS(fmt,n) p->value, id_warn, n, rb_usascii_str_new_lit(fmt)
# define WARN_ARGS_L(l,fmt,n) WARN_ARGS(fmt,n)
# ifdef HAVE_VA_ARGS_MACRO
# define WARN_CALL(...) rb_funcall(__VA_ARGS__)
# else
# define WARN_CALL rb_funcall
# endif
# define WARNING_ARGS(fmt,n) p->value, id_warning, n, rb_usascii_str_new_lit(fmt)
# define WARNING_ARGS_L(l, fmt,n) WARNING_ARGS(fmt,n)
# ifdef HAVE_VA_ARGS_MACRO
# define WARNING_CALL(...) rb_funcall(__VA_ARGS__)
# else
# define WARNING_CALL rb_funcall
# endif
PRINTF_ARGS(static void ripper_compile_error(struct parser_params*, const char *fmt, ...), 2, 3);
# define compile_error ripper_compile_error
#else
# define WARN_S_L(s,l) s
# define WARN_S(s) s
# define WARN_I(i) i
# define WARN_ID(i) rb_id2name(i)
# define WARN_IVAL(i) NUM2INT(i)
# define PRIsWARN PRIsVALUE
# define WARN_ARGS(fmt,n) WARN_ARGS_L(p->ruby_sourceline,fmt,n)
# define WARN_ARGS_L(l,fmt,n) p->ruby_sourcefile, (l), (fmt)
# define WARN_CALL rb_compile_warn
# define rb_warn0L_experimental(l,fmt) rb_category_compile_warn(RB_WARN_CATEGORY_EXPERIMENTAL, WARN_ARGS_L(l, fmt, 1))
# define WARNING_ARGS(fmt,n) WARN_ARGS(fmt,n)
# define WARNING_ARGS_L(l,fmt,n) WARN_ARGS_L(l,fmt,n)
# define WARNING_CALL rb_compile_warning
PRINTF_ARGS(static void parser_compile_error(struct parser_params*, const char *fmt, ...), 2, 3);
# define compile_error parser_compile_error
#endif

#define WARN_EOL(tok) \
    (looking_at_eol_p(p) ? \
     (void)rb_warning0("`" tok "' at the end of line without an expression") : \
     (void)0)
static int looking_at_eol_p(struct parser_params *p);

#line 1146 "ripper.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
#ifndef yydebug
extern int yydebug;
#endif
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    END_OF_INPUT = 0,              /* "end-of-input"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    keyword_class = 258,           /* "`class'"  */
    keyword_module = 259,          /* "`module'"  */
    keyword_def = 260,             /* "`def'"  */
    keyword_undef = 261,           /* "`undef'"  */
    keyword_begin = 262,           /* "`begin'"  */
    keyword_rescue = 263,          /* "`rescue'"  */
    keyword_ensure = 264,          /* "`ensure'"  */
    keyword_end = 265,             /* "`end'"  */
    keyword_if = 266,              /* "`if'"  */
    keyword_unless = 267,          /* "`unless'"  */
    keyword_then = 268,            /* "`then'"  */
    keyword_elsif = 269,           /* "`elsif'"  */
    keyword_else = 270,            /* "`else'"  */
    keyword_case = 271,            /* "`case'"  */
    keyword_when = 272,            /* "`when'"  */
    keyword_while = 273,           /* "`while'"  */
    keyword_until = 274,           /* "`until'"  */
    keyword_for = 275,             /* "`for'"  */
    keyword_break = 276,           /* "`break'"  */
    keyword_next = 277,            /* "`next'"  */
    keyword_redo = 278,            /* "`redo'"  */
    keyword_retry = 279,           /* "`retry'"  */
    keyword_in = 280,              /* "`in'"  */
    keyword_do = 281,              /* "`do'"  */
    keyword_do_cond = 282,         /* "`do' for condition"  */
    keyword_do_block = 283,        /* "`do' for block"  */
    keyword_do_LAMBDA = 284,       /* "`do' for lambda"  */
    keyword_return = 285,          /* "`return'"  */
    keyword_yield = 286,           /* "`yield'"  */
    keyword_super = 287,           /* "`super'"  */
    keyword_self = 288,            /* "`self'"  */
    keyword_nil = 289,             /* "`nil'"  */
    keyword_true = 290,            /* "`true'"  */
    keyword_false = 291,           /* "`false'"  */
    keyword_and = 292,             /* "`and'"  */
    keyword_or = 293,              /* "`or'"  */
    keyword_not = 294,             /* "`not'"  */
    modifier_if = 295,             /* "`if' modifier"  */
    modifier_unless = 296,         /* "`unless' modifier"  */
    modifier_while = 297,          /* "`while' modifier"  */
    modifier_until = 298,          /* "`until' modifier"  */
    modifier_rescue = 299,         /* "`rescue' modifier"  */
    keyword_alias = 300,           /* "`alias'"  */
    keyword_defined = 301,         /* "`defined?'"  */
    keyword_BEGIN = 302,           /* "`BEGIN'"  */
    keyword_END = 303,             /* "`END'"  */
    keyword__LINE__ = 304,         /* "`__LINE__'"  */
    keyword__FILE__ = 305,         /* "`__FILE__'"  */
    keyword__ENCODING__ = 306,     /* "`__ENCODING__'"  */
    tIDENTIFIER = 307,             /* "local variable or method"  */
    tFID = 308,                    /* "method"  */
    tGVAR = 309,                   /* "global variable"  */
    tIVAR = 310,                   /* "instance variable"  */
    tCONSTANT = 311,               /* "constant"  */
    tCVAR = 312,                   /* "class variable"  */
    tLABEL = 313,                  /* "label"  */
    tINTEGER = 314,                /* "integer literal"  */
    tFLOAT = 315,                  /* "float literal"  */
    tRATIONAL = 316,               /* "rational literal"  */
    tIMAGINARY = 317,              /* "imaginary literal"  */
    tCHAR = 318,                   /* "char literal"  */
    tNTH_REF = 319,                /* "numbered reference"  */
    tBACK_REF = 320,               /* "back reference"  */
    tSTRING_CONTENT = 321,         /* "literal content"  */
    tREGEXP_END = 322,             /* tREGEXP_END  */
    tSP = 323,                     /* "escaped space"  */
    tUPLUS = 132,                  /* "unary+"  */
    tUMINUS = 133,                 /* "unary-"  */
    tPOW = 134,                    /* "**"  */
    tCMP = 135,                    /* "<=>"  */
    tEQ = 140,                     /* "=="  */
    tEQQ = 141,                    /* "==="  */
    tNEQ = 142,                    /* "!="  */
    tGEQ = 139,                    /* ">="  */
    tLEQ = 138,                    /* "<="  */
    tANDOP = 148,                  /* "&&"  */
    tOROP = 149,                   /* "||"  */
    tMATCH = 143,                  /* "=~"  */
    tNMATCH = 144,                 /* "!~"  */
    tDOT2 = 128,                   /* ".."  */
    tDOT3 = 129,                   /* "..."  */
    tBDOT2 = 130,                  /* "(.."  */
    tBDOT3 = 131,                  /* "(..."  */
    tAREF = 145,                   /* "[]"  */
    tASET = 146,                   /* "[]="  */
    tLSHFT = 136,                  /* "<<"  */
    tRSHFT = 137,                  /* ">>"  */
    tANDDOT = 150,                 /* "&."  */
    tCOLON2 = 147,                 /* "::"  */
    tCOLON3 = 324,                 /* ":: at EXPR_BEG"  */
    tOP_ASGN = 325,                /* "operator-assignment"  */
    tASSOC = 326,                  /* "=>"  */
    tLPAREN = 327,                 /* "("  */
    tLPAREN_ARG = 328,             /* "( arg"  */
    tRPAREN = 329,                 /* ")"  */
    tLBRACK = 330,                 /* "["  */
    tLBRACE = 331,                 /* "{"  */
    tLBRACE_ARG = 332,             /* "{ arg"  */
    tSTAR = 333,                   /* "*"  */
    tDSTAR = 334,                  /* "**arg"  */
    tAMPER = 335,                  /* "&"  */
    tLAMBDA = 336,                 /* "->"  */
    tSYMBEG = 337,                 /* "symbol literal"  */
    tSTRING_BEG = 338,             /* "string literal"  */
    tXSTRING_BEG = 339,            /* "backtick literal"  */
    tREGEXP_BEG = 340,             /* "regexp literal"  */
    tWORDS_BEG = 341,              /* "word list"  */
    tQWORDS_BEG = 342,             /* "verbatim word list"  */
    tSYMBOLS_BEG = 343,            /* "symbol list"  */
    tQSYMBOLS_BEG = 344,           /* "verbatim symbol list"  */
    tSTRING_END = 345,             /* "terminator"  */
    tSTRING_DEND = 346,            /* "'}'"  */
    tSTRING_DBEG = 347,            /* tSTRING_DBEG  */
    tSTRING_DVAR = 348,            /* tSTRING_DVAR  */
    tLAMBEG = 349,                 /* tLAMBEG  */
    tLABEL_END = 350,              /* tLABEL_END  */
    tLOWEST = 351,                 /* tLOWEST  */
    tUMINUS_NUM = 352,             /* tUMINUS_NUM  */
    tLAST_TOKEN = 353              /* tLAST_TOKEN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 1126 "ripper.y"

    VALUE val;
    NODE *node;
    ID id;
    int num;
    st_table *tbl;
    const struct vtable *vars;
    struct rb_strterm_struct *strterm;
    struct lex_context ctxt;

#line 1325 "ripper.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (struct parser_params *p);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end-of-input"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_keyword_class = 3,              /* "`class'"  */
  YYSYMBOL_keyword_module = 4,             /* "`module'"  */
  YYSYMBOL_keyword_def = 5,                /* "`def'"  */
  YYSYMBOL_keyword_undef = 6,              /* "`undef'"  */
  YYSYMBOL_keyword_begin = 7,              /* "`begin'"  */
  YYSYMBOL_keyword_rescue = 8,             /* "`rescue'"  */
  YYSYMBOL_keyword_ensure = 9,             /* "`ensure'"  */
  YYSYMBOL_keyword_end = 10,               /* "`end'"  */
  YYSYMBOL_keyword_if = 11,                /* "`if'"  */
  YYSYMBOL_keyword_unless = 12,            /* "`unless'"  */
  YYSYMBOL_keyword_then = 13,              /* "`then'"  */
  YYSYMBOL_keyword_elsif = 14,             /* "`elsif'"  */
  YYSYMBOL_keyword_else = 15,              /* "`else'"  */
  YYSYMBOL_keyword_case = 16,              /* "`case'"  */
  YYSYMBOL_keyword_when = 17,              /* "`when'"  */
  YYSYMBOL_keyword_while = 18,             /* "`while'"  */
  YYSYMBOL_keyword_until = 19,             /* "`until'"  */
  YYSYMBOL_keyword_for = 20,               /* "`for'"  */
  YYSYMBOL_keyword_break = 21,             /* "`break'"  */
  YYSYMBOL_keyword_next = 22,              /* "`next'"  */
  YYSYMBOL_keyword_redo = 23,              /* "`redo'"  */
  YYSYMBOL_keyword_retry = 24,             /* "`retry'"  */
  YYSYMBOL_keyword_in = 25,                /* "`in'"  */
  YYSYMBOL_keyword_do = 26,                /* "`do'"  */
  YYSYMBOL_keyword_do_cond = 27,           /* "`do' for condition"  */
  YYSYMBOL_keyword_do_block = 28,          /* "`do' for block"  */
  YYSYMBOL_keyword_do_LAMBDA = 29,         /* "`do' for lambda"  */
  YYSYMBOL_keyword_return = 30,            /* "`return'"  */
  YYSYMBOL_keyword_yield = 31,             /* "`yield'"  */
  YYSYMBOL_keyword_super = 32,             /* "`super'"  */
  YYSYMBOL_keyword_self = 33,              /* "`self'"  */
  YYSYMBOL_keyword_nil = 34,               /* "`nil'"  */
  YYSYMBOL_keyword_true = 35,              /* "`true'"  */
  YYSYMBOL_keyword_false = 36,             /* "`false'"  */
  YYSYMBOL_keyword_and = 37,               /* "`and'"  */
  YYSYMBOL_keyword_or = 38,                /* "`or'"  */
  YYSYMBOL_keyword_not = 39,               /* "`not'"  */
  YYSYMBOL_modifier_if = 40,               /* "`if' modifier"  */
  YYSYMBOL_modifier_unless = 41,           /* "`unless' modifier"  */
  YYSYMBOL_modifier_while = 42,            /* "`while' modifier"  */
  YYSYMBOL_modifier_until = 43,            /* "`until' modifier"  */
  YYSYMBOL_modifier_rescue = 44,           /* "`rescue' modifier"  */
  YYSYMBOL_keyword_alias = 45,             /* "`alias'"  */
  YYSYMBOL_keyword_defined = 46,           /* "`defined?'"  */
  YYSYMBOL_keyword_BEGIN = 47,             /* "`BEGIN'"  */
  YYSYMBOL_keyword_END = 48,               /* "`END'"  */
  YYSYMBOL_keyword__LINE__ = 49,           /* "`__LINE__'"  */
  YYSYMBOL_keyword__FILE__ = 50,           /* "`__FILE__'"  */
  YYSYMBOL_keyword__ENCODING__ = 51,       /* "`__ENCODING__'"  */
  YYSYMBOL_tIDENTIFIER = 52,               /* "local variable or method"  */
  YYSYMBOL_tFID = 53,                      /* "method"  */
  YYSYMBOL_tGVAR = 54,                     /* "global variable"  */
  YYSYMBOL_tIVAR = 55,                     /* "instance variable"  */
  YYSYMBOL_tCONSTANT = 56,                 /* "constant"  */
  YYSYMBOL_tCVAR = 57,                     /* "class variable"  */
  YYSYMBOL_tLABEL = 58,                    /* "label"  */
  YYSYMBOL_tINTEGER = 59,                  /* "integer literal"  */
  YYSYMBOL_tFLOAT = 60,                    /* "float literal"  */
  YYSYMBOL_tRATIONAL = 61,                 /* "rational literal"  */
  YYSYMBOL_tIMAGINARY = 62,                /* "imaginary literal"  */
  YYSYMBOL_tCHAR = 63,                     /* "char literal"  */
  YYSYMBOL_tNTH_REF = 64,                  /* "numbered reference"  */
  YYSYMBOL_tBACK_REF = 65,                 /* "back reference"  */
  YYSYMBOL_tSTRING_CONTENT = 66,           /* "literal content"  */
  YYSYMBOL_tREGEXP_END = 67,               /* tREGEXP_END  */
  YYSYMBOL_68_ = 68,                       /* '.'  */
  YYSYMBOL_69_backslash_ = 69,             /* "backslash"  */
  YYSYMBOL_tSP = 70,                       /* "escaped space"  */
  YYSYMBOL_71_escaped_horizontal_tab_ = 71, /* "escaped horizontal tab"  */
  YYSYMBOL_72_escaped_form_feed_ = 72,     /* "escaped form feed"  */
  YYSYMBOL_73_escaped_carriage_return_ = 73, /* "escaped carriage return"  */
  YYSYMBOL_74_escaped_vertical_tab_ = 74,  /* "escaped vertical tab"  */
  YYSYMBOL_tUPLUS = 75,                    /* "unary+"  */
  YYSYMBOL_tUMINUS = 76,                   /* "unary-"  */
  YYSYMBOL_tPOW = 77,                      /* "**"  */
  YYSYMBOL_tCMP = 78,                      /* "<=>"  */
  YYSYMBOL_tEQ = 79,                       /* "=="  */
  YYSYMBOL_tEQQ = 80,                      /* "==="  */
  YYSYMBOL_tNEQ = 81,                      /* "!="  */
  YYSYMBOL_tGEQ = 82,                      /* ">="  */
  YYSYMBOL_tLEQ = 83,                      /* "<="  */
  YYSYMBOL_tANDOP = 84,                    /* "&&"  */
  YYSYMBOL_tOROP = 85,                     /* "||"  */
  YYSYMBOL_tMATCH = 86,                    /* "=~"  */
  YYSYMBOL_tNMATCH = 87,                   /* "!~"  */
  YYSYMBOL_tDOT2 = 88,                     /* ".."  */
  YYSYMBOL_tDOT3 = 89,                     /* "..."  */
  YYSYMBOL_tBDOT2 = 90,                    /* "(.."  */
  YYSYMBOL_tBDOT3 = 91,                    /* "(..."  */
  YYSYMBOL_tAREF = 92,                     /* "[]"  */
  YYSYMBOL_tASET = 93,                     /* "[]="  */
  YYSYMBOL_tLSHFT = 94,                    /* "<<"  */
  YYSYMBOL_tRSHFT = 95,                    /* ">>"  */
  YYSYMBOL_tANDDOT = 96,                   /* "&."  */
  YYSYMBOL_tCOLON2 = 97,                   /* "::"  */
  YYSYMBOL_tCOLON3 = 98,                   /* ":: at EXPR_BEG"  */
  YYSYMBOL_tOP_ASGN = 99,                  /* "operator-assignment"  */
  YYSYMBOL_tASSOC = 100,                   /* "=>"  */
  YYSYMBOL_tLPAREN = 101,                  /* "("  */
  YYSYMBOL_tLPAREN_ARG = 102,              /* "( arg"  */
  YYSYMBOL_tRPAREN = 103,                  /* ")"  */
  YYSYMBOL_tLBRACK = 104,                  /* "["  */
  YYSYMBOL_tLBRACE = 105,                  /* "{"  */
  YYSYMBOL_tLBRACE_ARG = 106,              /* "{ arg"  */
  YYSYMBOL_tSTAR = 107,                    /* "*"  */
  YYSYMBOL_tDSTAR = 108,                   /* "**arg"  */
  YYSYMBOL_tAMPER = 109,                   /* "&"  */
  YYSYMBOL_tLAMBDA = 110,                  /* "->"  */
  YYSYMBOL_tSYMBEG = 111,                  /* "symbol literal"  */
  YYSYMBOL_tSTRING_BEG = 112,              /* "string literal"  */
  YYSYMBOL_tXSTRING_BEG = 113,             /* "backtick literal"  */
  YYSYMBOL_tREGEXP_BEG = 114,              /* "regexp literal"  */
  YYSYMBOL_tWORDS_BEG = 115,               /* "word list"  */
  YYSYMBOL_tQWORDS_BEG = 116,              /* "verbatim word list"  */
  YYSYMBOL_tSYMBOLS_BEG = 117,             /* "symbol list"  */
  YYSYMBOL_tQSYMBOLS_BEG = 118,            /* "verbatim symbol list"  */
  YYSYMBOL_tSTRING_END = 119,              /* "terminator"  */
  YYSYMBOL_tSTRING_DEND = 120,             /* "'}'"  */
  YYSYMBOL_tSTRING_DBEG = 121,             /* tSTRING_DBEG  */
  YYSYMBOL_tSTRING_DVAR = 122,             /* tSTRING_DVAR  */
  YYSYMBOL_tLAMBEG = 123,                  /* tLAMBEG  */
  YYSYMBOL_tLABEL_END = 124,               /* tLABEL_END  */
  YYSYMBOL_tLOWEST = 125,                  /* tLOWEST  */
  YYSYMBOL_126_ = 126,                     /* '='  */
  YYSYMBOL_127_ = 127,                     /* '?'  */
  YYSYMBOL_128_ = 128,                     /* ':'  */
  YYSYMBOL_129_ = 129,                     /* '>'  */
  YYSYMBOL_130_ = 130,                     /* '<'  */
  YYSYMBOL_131_ = 131,                     /* '|'  */
  YYSYMBOL_132_ = 132,                     /* '^'  */
  YYSYMBOL_133_ = 133,                     /* '&'  */
  YYSYMBOL_134_ = 134,                     /* '+'  */
  YYSYMBOL_135_ = 135,                     /* '-'  */
  YYSYMBOL_136_ = 136,                     /* '*'  */
  YYSYMBOL_137_ = 137,                     /* '/'  */
  YYSYMBOL_138_ = 138,                     /* '%'  */
  YYSYMBOL_tUMINUS_NUM = 139,              /* tUMINUS_NUM  */
  YYSYMBOL_140_ = 140,                     /* '!'  */
  YYSYMBOL_141_ = 141,                     /* '~'  */
  YYSYMBOL_tLAST_TOKEN = 142,              /* tLAST_TOKEN  */
  YYSYMBOL_143_ = 143,                     /* '{'  */
  YYSYMBOL_144_ = 144,                     /* '}'  */
  YYSYMBOL_145_ = 145,                     /* '['  */
  YYSYMBOL_146_ = 146,                     /* ','  */
  YYSYMBOL_147_ = 147,                     /* '`'  */
  YYSYMBOL_148_ = 148,                     /* '('  */
  YYSYMBOL_149_ = 149,                     /* ')'  */
  YYSYMBOL_150_ = 150,                     /* ']'  */
  YYSYMBOL_151_ = 151,                     /* ';'  */
  YYSYMBOL_152_ = 152,                     /* ' '  */
  YYSYMBOL_153_n_ = 153,                   /* '\n'  */
  YYSYMBOL_YYACCEPT = 154,                 /* $accept  */
  YYSYMBOL_program = 155,                  /* program  */
  YYSYMBOL_156_1 = 156,                    /* $@1  */
  YYSYMBOL_top_compstmt = 157,             /* top_compstmt  */
  YYSYMBOL_top_stmts = 158,                /* top_stmts  */
  YYSYMBOL_top_stmt = 159,                 /* top_stmt  */
  YYSYMBOL_begin_block = 160,              /* begin_block  */
  YYSYMBOL_bodystmt = 161,                 /* bodystmt  */
  YYSYMBOL_162_2 = 162,                    /* $@2  */
  YYSYMBOL_compstmt = 163,                 /* compstmt  */
  YYSYMBOL_stmts = 164,                    /* stmts  */
  YYSYMBOL_stmt_or_begin = 165,            /* stmt_or_begin  */
  YYSYMBOL_166_3 = 166,                    /* $@3  */
  YYSYMBOL_stmt = 167,                     /* stmt  */
  YYSYMBOL_168_4 = 168,                    /* $@4  */
  YYSYMBOL_command_asgn = 169,             /* command_asgn  */
  YYSYMBOL_command_rhs = 170,              /* command_rhs  */
  YYSYMBOL_expr = 171,                     /* expr  */
  YYSYMBOL_172_5 = 172,                    /* @5  */
  YYSYMBOL_173_6 = 173,                    /* @6  */
  YYSYMBOL_174_7 = 174,                    /* @7  */
  YYSYMBOL_175_8 = 175,                    /* @8  */
  YYSYMBOL_def_name = 176,                 /* def_name  */
  YYSYMBOL_defn_head = 177,                /* defn_head  */
  YYSYMBOL_defs_head = 178,                /* defs_head  */
  YYSYMBOL_179_9 = 179,                    /* $@9  */
  YYSYMBOL_expr_value = 180,               /* expr_value  */
  YYSYMBOL_expr_value_do = 181,            /* expr_value_do  */
  YYSYMBOL_182_10 = 182,                   /* $@10  */
  YYSYMBOL_183_11 = 183,                   /* $@11  */
  YYSYMBOL_command_call = 184,             /* command_call  */
  YYSYMBOL_block_command = 185,            /* block_command  */
  YYSYMBOL_cmd_brace_block = 186,          /* cmd_brace_block  */
  YYSYMBOL_fcall = 187,                    /* fcall  */
  YYSYMBOL_command = 188,                  /* command  */
  YYSYMBOL_mlhs = 189,                     /* mlhs  */
  YYSYMBOL_mlhs_inner = 190,               /* mlhs_inner  */
  YYSYMBOL_mlhs_basic = 191,               /* mlhs_basic  */
  YYSYMBOL_mlhs_item = 192,                /* mlhs_item  */
  YYSYMBOL_mlhs_head = 193,                /* mlhs_head  */
  YYSYMBOL_mlhs_post = 194,                /* mlhs_post  */
  YYSYMBOL_mlhs_node = 195,                /* mlhs_node  */
  YYSYMBOL_lhs = 196,                      /* lhs  */
  YYSYMBOL_cname = 197,                    /* cname  */
  YYSYMBOL_cpath = 198,                    /* cpath  */
  YYSYMBOL_fname = 199,                    /* fname  */
  YYSYMBOL_fitem = 200,                    /* fitem  */
  YYSYMBOL_undef_list = 201,               /* undef_list  */
  YYSYMBOL_202_12 = 202,                   /* $@12  */
  YYSYMBOL_op = 203,                       /* op  */
  YYSYMBOL_reswords = 204,                 /* reswords  */
  YYSYMBOL_arg = 205,                      /* arg  */
  YYSYMBOL_206_13 = 206,                   /* $@13  */
  YYSYMBOL_relop = 207,                    /* relop  */
  YYSYMBOL_rel_expr = 208,                 /* rel_expr  */
  YYSYMBOL_lex_ctxt = 209,                 /* lex_ctxt  */
  YYSYMBOL_arg_value = 210,                /* arg_value  */
  YYSYMBOL_aref_args = 211,                /* aref_args  */
  YYSYMBOL_arg_rhs = 212,                  /* arg_rhs  */
  YYSYMBOL_paren_args = 213,               /* paren_args  */
  YYSYMBOL_opt_paren_args = 214,           /* opt_paren_args  */
  YYSYMBOL_opt_call_args = 215,            /* opt_call_args  */
  YYSYMBOL_call_args = 216,                /* call_args  */
  YYSYMBOL_command_args = 217,             /* command_args  */
  YYSYMBOL_218_14 = 218,                   /* $@14  */
  YYSYMBOL_block_arg = 219,                /* block_arg  */
  YYSYMBOL_opt_block_arg = 220,            /* opt_block_arg  */
  YYSYMBOL_args = 221,                     /* args  */
  YYSYMBOL_mrhs_arg = 222,                 /* mrhs_arg  */
  YYSYMBOL_mrhs = 223,                     /* mrhs  */
  YYSYMBOL_primary = 224,                  /* primary  */
  YYSYMBOL_225_15 = 225,                   /* $@15  */
  YYSYMBOL_226_16 = 226,                   /* $@16  */
  YYSYMBOL_227_17 = 227,                   /* $@17  */
  YYSYMBOL_228_18 = 228,                   /* $@18  */
  YYSYMBOL_229_19 = 229,                   /* @19  */
  YYSYMBOL_230_20 = 230,                   /* @20  */
  YYSYMBOL_231_21 = 231,                   /* $@21  */
  YYSYMBOL_232_22 = 232,                   /* $@22  */
  YYSYMBOL_233_23 = 233,                   /* $@23  */
  YYSYMBOL_primary_value = 234,            /* primary_value  */
  YYSYMBOL_k_begin = 235,                  /* k_begin  */
  YYSYMBOL_k_if = 236,                     /* k_if  */
  YYSYMBOL_k_unless = 237,                 /* k_unless  */
  YYSYMBOL_k_while = 238,                  /* k_while  */
  YYSYMBOL_k_until = 239,                  /* k_until  */
  YYSYMBOL_k_case = 240,                   /* k_case  */
  YYSYMBOL_k_for = 241,                    /* k_for  */
  YYSYMBOL_k_class = 242,                  /* k_class  */
  YYSYMBOL_k_module = 243,                 /* k_module  */
  YYSYMBOL_k_def = 244,                    /* k_def  */
  YYSYMBOL_k_do = 245,                     /* k_do  */
  YYSYMBOL_k_do_block = 246,               /* k_do_block  */
  YYSYMBOL_k_rescue = 247,                 /* k_rescue  */
  YYSYMBOL_k_ensure = 248,                 /* k_ensure  */
  YYSYMBOL_k_when = 249,                   /* k_when  */
  YYSYMBOL_k_else = 250,                   /* k_else  */
  YYSYMBOL_k_elsif = 251,                  /* k_elsif  */
  YYSYMBOL_k_end = 252,                    /* k_end  */
  YYSYMBOL_k_return = 253,                 /* k_return  */
  YYSYMBOL_then = 254,                     /* then  */
  YYSYMBOL_do = 255,                       /* do  */
  YYSYMBOL_if_tail = 256,                  /* if_tail  */
  YYSYMBOL_opt_else = 257,                 /* opt_else  */
  YYSYMBOL_for_var = 258,                  /* for_var  */
  YYSYMBOL_f_marg = 259,                   /* f_marg  */
  YYSYMBOL_f_marg_list = 260,              /* f_marg_list  */
  YYSYMBOL_f_margs = 261,                  /* f_margs  */
  YYSYMBOL_f_rest_marg = 262,              /* f_rest_marg  */
  YYSYMBOL_f_any_kwrest = 263,             /* f_any_kwrest  */
  YYSYMBOL_f_eq = 264,                     /* f_eq  */
  YYSYMBOL_265_24 = 265,                   /* $@24  */
  YYSYMBOL_block_args_tail = 266,          /* block_args_tail  */
  YYSYMBOL_opt_block_args_tail = 267,      /* opt_block_args_tail  */
  YYSYMBOL_excessed_comma = 268,           /* excessed_comma  */
  YYSYMBOL_block_param = 269,              /* block_param  */
  YYSYMBOL_opt_block_param = 270,          /* opt_block_param  */
  YYSYMBOL_block_param_def = 271,          /* block_param_def  */
  YYSYMBOL_opt_bv_decl = 272,              /* opt_bv_decl  */
  YYSYMBOL_bv_decls = 273,                 /* bv_decls  */
  YYSYMBOL_bvar = 274,                     /* bvar  */
  YYSYMBOL_lambda = 275,                   /* lambda  */
  YYSYMBOL_276_25 = 276,                   /* @25  */
  YYSYMBOL_277_26 = 277,                   /* @26  */
  YYSYMBOL_278_27 = 278,                   /* @27  */
  YYSYMBOL_279_28 = 279,                   /* $@28  */
  YYSYMBOL_f_larglist = 280,               /* f_larglist  */
  YYSYMBOL_lambda_body = 281,              /* lambda_body  */
  YYSYMBOL_do_block = 282,                 /* do_block  */
  YYSYMBOL_block_call = 283,               /* block_call  */
  YYSYMBOL_method_call = 284,              /* method_call  */
  YYSYMBOL_brace_block = 285,              /* brace_block  */
  YYSYMBOL_brace_body = 286,               /* brace_body  */
  YYSYMBOL_287_29 = 287,                   /* @29  */
  YYSYMBOL_288_30 = 288,                   /* @30  */
  YYSYMBOL_289_31 = 289,                   /* @31  */
  YYSYMBOL_do_body = 290,                  /* do_body  */
  YYSYMBOL_291_32 = 291,                   /* @32  */
  YYSYMBOL_292_33 = 292,                   /* @33  */
  YYSYMBOL_293_34 = 293,                   /* @34  */
  YYSYMBOL_case_args = 294,                /* case_args  */
  YYSYMBOL_case_body = 295,                /* case_body  */
  YYSYMBOL_cases = 296,                    /* cases  */
  YYSYMBOL_p_case_body = 297,              /* p_case_body  */
  YYSYMBOL_298_35 = 298,                   /* @35  */
  YYSYMBOL_299_36 = 299,                   /* @36  */
  YYSYMBOL_300_37 = 300,                   /* $@37  */
  YYSYMBOL_p_cases = 301,                  /* p_cases  */
  YYSYMBOL_p_top_expr = 302,               /* p_top_expr  */
  YYSYMBOL_p_top_expr_body = 303,          /* p_top_expr_body  */
  YYSYMBOL_p_expr = 304,                   /* p_expr  */
  YYSYMBOL_p_as = 305,                     /* p_as  */
  YYSYMBOL_p_alt = 306,                    /* p_alt  */
  YYSYMBOL_p_lparen = 307,                 /* p_lparen  */
  YYSYMBOL_p_lbracket = 308,               /* p_lbracket  */
  YYSYMBOL_p_expr_basic = 309,             /* p_expr_basic  */
  YYSYMBOL_310_38 = 310,                   /* @38  */
  YYSYMBOL_311_39 = 311,                   /* @39  */
  YYSYMBOL_p_args = 312,                   /* p_args  */
  YYSYMBOL_p_args_head = 313,              /* p_args_head  */
  YYSYMBOL_p_args_tail = 314,              /* p_args_tail  */
  YYSYMBOL_p_find = 315,                   /* p_find  */
  YYSYMBOL_p_rest = 316,                   /* p_rest  */
  YYSYMBOL_p_args_post = 317,              /* p_args_post  */
  YYSYMBOL_p_arg = 318,                    /* p_arg  */
  YYSYMBOL_p_kwargs = 319,                 /* p_kwargs  */
  YYSYMBOL_p_kwarg = 320,                  /* p_kwarg  */
  YYSYMBOL_p_kw = 321,                     /* p_kw  */
  YYSYMBOL_p_kw_label = 322,               /* p_kw_label  */
  YYSYMBOL_p_kwrest = 323,                 /* p_kwrest  */
  YYSYMBOL_p_kwnorest = 324,               /* p_kwnorest  */
  YYSYMBOL_p_any_kwrest = 325,             /* p_any_kwrest  */
  YYSYMBOL_p_value = 326,                  /* p_value  */
  YYSYMBOL_p_primitive = 327,              /* p_primitive  */
  YYSYMBOL_p_variable = 328,               /* p_variable  */
  YYSYMBOL_p_var_ref = 329,                /* p_var_ref  */
  YYSYMBOL_p_expr_ref = 330,               /* p_expr_ref  */
  YYSYMBOL_p_const = 331,                  /* p_const  */
  YYSYMBOL_opt_rescue = 332,               /* opt_rescue  */
  YYSYMBOL_exc_list = 333,                 /* exc_list  */
  YYSYMBOL_exc_var = 334,                  /* exc_var  */
  YYSYMBOL_opt_ensure = 335,               /* opt_ensure  */
  YYSYMBOL_literal = 336,                  /* literal  */
  YYSYMBOL_strings = 337,                  /* strings  */
  YYSYMBOL_string = 338,                   /* string  */
  YYSYMBOL_string1 = 339,                  /* string1  */
  YYSYMBOL_xstring = 340,                  /* xstring  */
  YYSYMBOL_regexp = 341,                   /* regexp  */
  YYSYMBOL_words = 342,                    /* words  */
  YYSYMBOL_word_list = 343,                /* word_list  */
  YYSYMBOL_word = 344,                     /* word  */
  YYSYMBOL_symbols = 345,                  /* symbols  */
  YYSYMBOL_symbol_list = 346,              /* symbol_list  */
  YYSYMBOL_qwords = 347,                   /* qwords  */
  YYSYMBOL_qsymbols = 348,                 /* qsymbols  */
  YYSYMBOL_qword_list = 349,               /* qword_list  */
  YYSYMBOL_qsym_list = 350,                /* qsym_list  */
  YYSYMBOL_string_contents = 351,          /* string_contents  */
  YYSYMBOL_xstring_contents = 352,         /* xstring_contents  */
  YYSYMBOL_regexp_contents = 353,          /* regexp_contents  */
  YYSYMBOL_string_content = 354,           /* string_content  */
  YYSYMBOL_355_40 = 355,                   /* @40  */
  YYSYMBOL_356_41 = 356,                   /* $@41  */
  YYSYMBOL_357_42 = 357,                   /* @42  */
  YYSYMBOL_358_43 = 358,                   /* @43  */
  YYSYMBOL_359_44 = 359,                   /* @44  */
  YYSYMBOL_360_45 = 360,                   /* @45  */
  YYSYMBOL_string_dvar = 361,              /* string_dvar  */
  YYSYMBOL_symbol = 362,                   /* symbol  */
  YYSYMBOL_ssym = 363,                     /* ssym  */
  YYSYMBOL_sym = 364,                      /* sym  */
  YYSYMBOL_dsym = 365,                     /* dsym  */
  YYSYMBOL_numeric = 366,                  /* numeric  */
  YYSYMBOL_simple_numeric = 367,           /* simple_numeric  */
  YYSYMBOL_nonlocal_var = 368,             /* nonlocal_var  */
  YYSYMBOL_user_variable = 369,            /* user_variable  */
  YYSYMBOL_keyword_variable = 370,         /* keyword_variable  */
  YYSYMBOL_var_ref = 371,                  /* var_ref  */
  YYSYMBOL_var_lhs = 372,                  /* var_lhs  */
  YYSYMBOL_backref = 373,                  /* backref  */
  YYSYMBOL_superclass = 374,               /* superclass  */
  YYSYMBOL_375_46 = 375,                   /* $@46  */
  YYSYMBOL_f_opt_paren_args = 376,         /* f_opt_paren_args  */
  YYSYMBOL_f_paren_args = 377,             /* f_paren_args  */
  YYSYMBOL_f_arglist = 378,                /* f_arglist  */
  YYSYMBOL_379_47 = 379,                   /* @47  */
  YYSYMBOL_args_tail = 380,                /* args_tail  */
  YYSYMBOL_opt_args_tail = 381,            /* opt_args_tail  */
  YYSYMBOL_f_args = 382,                   /* f_args  */
  YYSYMBOL_args_forward = 383,             /* args_forward  */
  YYSYMBOL_f_bad_arg = 384,                /* f_bad_arg  */
  YYSYMBOL_f_norm_arg = 385,               /* f_norm_arg  */
  YYSYMBOL_f_arg_asgn = 386,               /* f_arg_asgn  */
  YYSYMBOL_f_arg_item = 387,               /* f_arg_item  */
  YYSYMBOL_f_arg = 388,                    /* f_arg  */
  YYSYMBOL_f_label = 389,                  /* f_label  */
  YYSYMBOL_f_kw = 390,                     /* f_kw  */
  YYSYMBOL_f_block_kw = 391,               /* f_block_kw  */
  YYSYMBOL_f_block_kwarg = 392,            /* f_block_kwarg  */
  YYSYMBOL_f_kwarg = 393,                  /* f_kwarg  */
  YYSYMBOL_kwrest_mark = 394,              /* kwrest_mark  */
  YYSYMBOL_f_no_kwarg = 395,               /* f_no_kwarg  */
  YYSYMBOL_f_kwrest = 396,                 /* f_kwrest  */
  YYSYMBOL_f_opt = 397,                    /* f_opt  */
  YYSYMBOL_f_block_opt = 398,              /* f_block_opt  */
  YYSYMBOL_f_block_optarg = 399,           /* f_block_optarg  */
  YYSYMBOL_f_optarg = 400,                 /* f_optarg  */
  YYSYMBOL_restarg_mark = 401,             /* restarg_mark  */
  YYSYMBOL_f_rest_arg = 402,               /* f_rest_arg  */
  YYSYMBOL_blkarg_mark = 403,              /* blkarg_mark  */
  YYSYMBOL_f_block_arg = 404,              /* f_block_arg  */
  YYSYMBOL_opt_f_block_arg = 405,          /* opt_f_block_arg  */
  YYSYMBOL_singleton = 406,                /* singleton  */
  YYSYMBOL_407_48 = 407,                   /* $@48  */
  YYSYMBOL_assoc_list = 408,               /* assoc_list  */
  YYSYMBOL_assocs = 409,                   /* assocs  */
  YYSYMBOL_assoc = 410,                    /* assoc  */
  YYSYMBOL_operation = 411,                /* operation  */
  YYSYMBOL_operation2 = 412,               /* operation2  */
  YYSYMBOL_operation3 = 413,               /* operation3  */
  YYSYMBOL_dot_or_colon = 414,             /* dot_or_colon  */
  YYSYMBOL_call_op = 415,                  /* call_op  */
  YYSYMBOL_call_op2 = 416,                 /* call_op2  */
  YYSYMBOL_opt_terms = 417,                /* opt_terms  */
  YYSYMBOL_opt_nl = 418,                   /* opt_nl  */
  YYSYMBOL_rparen = 419,                   /* rparen  */
  YYSYMBOL_rbracket = 420,                 /* rbracket  */
  YYSYMBOL_rbrace = 421,                   /* rbrace  */
  YYSYMBOL_trailer = 422,                  /* trailer  */
  YYSYMBOL_term = 423,                     /* term  */
  YYSYMBOL_terms = 424,                    /* terms  */
  YYSYMBOL_none = 425                      /* none  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   15161

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  154
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  272
/* YYNRULES -- Number of rules.  */
#define YYNRULES  785
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1309

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   353


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,    71,
     153,    74,    72,    73,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,   152,   140,     2,     2,     2,   138,   133,     2,
     148,   149,   136,   134,   146,   135,    68,   137,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   128,   151,
     130,   126,   129,   127,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   145,    69,   150,   132,     2,   147,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   143,   131,   144,   141,     2,    88,    89,
      90,    91,    75,    76,    77,    78,    94,    95,    83,    82,
      79,    80,    81,    86,    87,    92,    93,    97,    84,    85,
      96,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    70,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   139,   142
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  1327,  1327,  1327,  1353,  1359,  1366,  1373,  1380,  1386,
    1387,  1393,  1406,  1404,  1415,  1426,  1432,  1439,  1446,  1453,
    1459,  1464,  1463,  1473,  1473,  1480,  1487,  1497,  1506,  1513,
    1521,  1529,  1541,  1553,  1563,  1577,  1578,  1586,  1593,  1601,
    1608,  1611,  1618,  1625,  1633,  1640,  1647,  1655,  1662,  1673,
    1685,  1698,  1712,  1722,  1727,  1736,  1739,  1740,  1744,  1748,
    1752,  1757,  1765,  1756,  1779,  1787,  1778,  1800,  1803,  1820,
    1830,  1829,  1848,  1855,  1855,  1855,  1861,  1862,  1865,  1866,
    1875,  1885,  1895,  1904,  1915,  1922,  1929,  1936,  1943,  1951,
    1959,  1966,  1973,  1982,  1983,  1992,  1993,  2002,  2009,  2016,
    2023,  2030,  2037,  2044,  2051,  2058,  2065,  2074,  2075,  2084,
    2091,  2100,  2107,  2116,  2123,  2130,  2137,  2147,  2154,  2164,
    2171,  2178,  2188,  2195,  2202,  2209,  2216,  2223,  2230,  2237,
    2244,  2254,  2262,  2265,  2272,  2279,  2288,  2289,  2290,  2291,
    2296,  2299,  2306,  2309,  2316,  2316,  2326,  2327,  2328,  2329,
    2330,  2331,  2332,  2333,  2334,  2335,  2336,  2337,  2338,  2339,
    2340,  2341,  2342,  2343,  2344,  2345,  2346,  2347,  2348,  2349,
    2350,  2351,  2352,  2353,  2354,  2355,  2358,  2358,  2358,  2359,
    2359,  2360,  2360,  2360,  2361,  2361,  2361,  2361,  2362,  2362,
    2362,  2362,  2363,  2363,  2363,  2364,  2364,  2364,  2364,  2365,
    2365,  2365,  2365,  2366,  2366,  2366,  2366,  2367,  2367,  2367,
    2367,  2368,  2368,  2368,  2368,  2369,  2369,  2372,  2379,  2386,
    2393,  2400,  2407,  2414,  2422,  2430,  2438,  2447,  2456,  2464,
    2472,  2480,  2488,  2492,  2496,  2500,  2504,  2508,  2512,  2516,
    2520,  2524,  2528,  2532,  2536,  2540,  2541,  2545,  2549,  2553,
    2557,  2561,  2565,  2569,  2573,  2577,  2581,  2585,  2585,  2590,
    2599,  2610,  2622,  2635,  2649,  2655,  2656,  2657,  2658,  2661,
    2665,  2672,  2676,  2682,  2689,  2690,  2694,  2701,  2710,  2715,
    2725,  2732,  2744,  2758,  2759,  2762,  2763,  2764,  2768,  2775,
    2784,  2792,  2799,  2807,  2815,  2819,  2819,  2856,  2863,  2876,
    2880,  2887,  2894,  2901,  2908,  2918,  2919,  2923,  2930,  2937,
    2946,  2947,  2948,  2949,  2950,  2951,  2952,  2953,  2954,  2955,
    2956,  2964,  2963,  2978,  2978,  2985,  2985,  2993,  3001,  3008,
    3015,  3022,  3030,  3037,  3044,  3051,  3058,  3058,  3063,  3067,
    3071,  3078,  3079,  3087,  3088,  3099,  3110,  3120,  3131,  3130,
    3147,  3146,  3161,  3170,  3213,  3212,  3236,  3235,  3258,  3257,
    3280,  3292,  3306,  3313,  3320,  3327,  3336,  3343,  3349,  3366,
    3372,  3378,  3384,  3390,  3396,  3403,  3410,  3417,  3423,  3429,
    3435,  3441,  3447,  3462,  3469,  3475,  3482,  3483,  3484,  3487,
    3488,  3491,  3492,  3504,  3505,  3514,  3515,  3518,  3526,  3535,
    3542,  3551,  3558,  3565,  3572,  3579,  3588,  3596,  3605,  3606,
    3609,  3609,  3611,  3615,  3619,  3623,  3629,  3634,  3639,  3649,
    3653,  3657,  3661,  3665,  3669,  3674,  3678,  3682,  3686,  3690,
    3694,  3698,  3702,  3706,  3712,  3713,  3719,  3729,  3742,  3746,
    3755,  3757,  3761,  3766,  3773,  3779,  3783,  3787,  3772,  3812,
    3821,  3832,  3837,  3843,  3853,  3867,  3874,  3881,  3890,  3899,
    3907,  3915,  3922,  3930,  3938,  3945,  3952,  3965,  3973,  3983,
    3984,  3988,  3983,  4005,  4006,  4010,  4005,  4029,  4037,  4044,
    4052,  4061,  4073,  4074,  4078,  4085,  4089,  4077,  4104,  4105,
    4108,  4109,  4117,  4127,  4128,  4133,  4141,  4145,  4149,  4155,
    4158,  4167,  4170,  4177,  4180,  4181,  4183,  4184,  4185,  4194,
    4203,  4212,  4217,  4226,  4235,  4244,  4249,  4253,  4257,  4263,
    4262,  4274,  4279,  4279,  4286,  4295,  4299,  4308,  4312,  4316,
    4320,  4324,  4327,  4331,  4340,  4344,  4350,  4360,  4364,  4370,
    4371,  4380,  4389,  4393,  4397,  4401,  4407,  4409,  4418,  4426,
    4440,  4441,  4464,  4468,  4474,  4480,  4481,  4484,  4485,  4494,
    4503,  4511,  4519,  4520,  4521,  4522,  4530,  4540,  4541,  4542,
    4543,  4544,  4545,  4546,  4547,  4548,  4555,  4558,  4568,  4579,
    4588,  4597,  4604,  4611,  4620,  4632,  4635,  4642,  4649,  4652,
    4656,  4659,  4666,  4669,  4670,  4673,  4690,  4691,  4692,  4701,
    4711,  4720,  4726,  4736,  4742,  4751,  4753,  4762,  4772,  4778,
    4787,  4796,  4806,  4812,  4822,  4828,  4838,  4848,  4867,  4873,
    4883,  4893,  4934,  4937,  4936,  4953,  4957,  4962,  4966,  4970,
    4952,  4991,  4998,  5005,  5012,  5015,  5016,  5019,  5029,  5030,
    5031,  5032,  5035,  5045,  5046,  5056,  5057,  5058,  5059,  5062,
    5063,  5064,  5067,  5068,  5069,  5070,  5071,  5074,  5075,  5076,
    5077,  5078,  5079,  5080,  5083,  5096,  5105,  5112,  5121,  5122,
    5126,  5125,  5135,  5143,  5144,  5152,  5164,  5165,  5165,  5181,
    5185,  5189,  5193,  5197,  5204,  5209,  5214,  5218,  5222,  5226,
    5230,  5234,  5238,  5242,  5246,  5250,  5254,  5258,  5262,  5266,
    5271,  5277,  5286,  5295,  5304,  5313,  5324,  5325,  5333,  5342,
    5350,  5371,  5373,  5386,  5396,  5405,  5416,  5424,  5434,  5441,
    5451,  5458,  5467,  5468,  5471,  5479,  5487,  5497,  5508,  5519,
    5526,  5535,  5542,  5551,  5552,  5555,  5563,  5573,  5574,  5577,
    5585,  5595,  5599,  5605,  5610,  5610,  5634,  5635,  5644,  5646,
    5669,  5680,  5687,  5696,  5704,  5723,  5724,  5725,  5728,  5729,
    5730,  5731,  5734,  5735,  5736,  5739,  5740,  5743,  5744,  5747,
    5748,  5751,  5752,  5755,  5756,  5759,  5762,  5765,  5768,  5769,
    5770,  5773,  5774,  5777,  5778,  5782
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end-of-input\"", "error", "\"invalid token\"", "\"`class'\"",
  "\"`module'\"", "\"`def'\"", "\"`undef'\"", "\"`begin'\"",
  "\"`rescue'\"", "\"`ensure'\"", "\"`end'\"", "\"`if'\"", "\"`unless'\"",
  "\"`then'\"", "\"`elsif'\"", "\"`else'\"", "\"`case'\"", "\"`when'\"",
  "\"`while'\"", "\"`until'\"", "\"`for'\"", "\"`break'\"", "\"`next'\"",
  "\"`redo'\"", "\"`retry'\"", "\"`in'\"", "\"`do'\"",
  "\"`do' for condition\"", "\"`do' for block\"", "\"`do' for lambda\"",
  "\"`return'\"", "\"`yield'\"", "\"`super'\"", "\"`self'\"", "\"`nil'\"",
  "\"`true'\"", "\"`false'\"", "\"`and'\"", "\"`or'\"", "\"`not'\"",
  "\"`if' modifier\"", "\"`unless' modifier\"", "\"`while' modifier\"",
  "\"`until' modifier\"", "\"`rescue' modifier\"", "\"`alias'\"",
  "\"`defined?'\"", "\"`BEGIN'\"", "\"`END'\"", "\"`__LINE__'\"",
  "\"`__FILE__'\"", "\"`__ENCODING__'\"", "\"local variable or method\"",
  "\"method\"", "\"global variable\"", "\"instance variable\"",
  "\"constant\"", "\"class variable\"", "\"label\"", "\"integer literal\"",
  "\"float literal\"", "\"rational literal\"", "\"imaginary literal\"",
  "\"char literal\"", "\"numbered reference\"", "\"back reference\"",
  "\"literal content\"", "tREGEXP_END", "'.'", "\"backslash\"",
  "\"escaped space\"", "\"escaped horizontal tab\"",
  "\"escaped form feed\"", "\"escaped carriage return\"",
  "\"escaped vertical tab\"", "\"unary+\"", "\"unary-\"", "\"**\"",
  "\"<=>\"", "\"==\"", "\"===\"", "\"!=\"", "\">=\"", "\"<=\"", "\"&&\"",
  "\"||\"", "\"=~\"", "\"!~\"", "\"..\"", "\"...\"", "\"(..\"", "\"(...\"",
  "\"[]\"", "\"[]=\"", "\"<<\"", "\">>\"", "\"&.\"", "\"::\"",
  "\":: at EXPR_BEG\"", "\"operator-assignment\"", "\"=>\"", "\"(\"",
  "\"( arg\"", "\")\"", "\"[\"", "\"{\"", "\"{ arg\"", "\"*\"",
  "\"**arg\"", "\"&\"", "\"->\"", "\"symbol literal\"",
  "\"string literal\"", "\"backtick literal\"", "\"regexp literal\"",
  "\"word list\"", "\"verbatim word list\"", "\"symbol list\"",
  "\"verbatim symbol list\"", "\"terminator\"", "\"'}'\"", "tSTRING_DBEG",
  "tSTRING_DVAR", "tLAMBEG", "tLABEL_END", "tLOWEST", "'='", "'?'", "':'",
  "'>'", "'<'", "'|'", "'^'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "tUMINUS_NUM", "'!'", "'~'", "tLAST_TOKEN", "'{'", "'}'", "'['", "','",
  "'`'", "'('", "')'", "']'", "';'", "' '", "'\\n'", "$accept", "program",
  "$@1", "top_compstmt", "top_stmts", "top_stmt", "begin_block",
  "bodystmt", "$@2", "compstmt", "stmts", "stmt_or_begin", "$@3", "stmt",
  "$@4", "command_asgn", "command_rhs", "expr", "@5", "@6", "@7", "@8",
  "def_name", "defn_head", "defs_head", "$@9", "expr_value",
  "expr_value_do", "$@10", "$@11", "command_call", "block_command",
  "cmd_brace_block", "fcall", "command", "mlhs", "mlhs_inner",
  "mlhs_basic", "mlhs_item", "mlhs_head", "mlhs_post", "mlhs_node", "lhs",
  "cname", "cpath", "fname", "fitem", "undef_list", "$@12", "op",
  "reswords", "arg", "$@13", "relop", "rel_expr", "lex_ctxt", "arg_value",
  "aref_args", "arg_rhs", "paren_args", "opt_paren_args", "opt_call_args",
  "call_args", "command_args", "$@14", "block_arg", "opt_block_arg",
  "args", "mrhs_arg", "mrhs", "primary", "$@15", "$@16", "$@17", "$@18",
  "@19", "@20", "$@21", "$@22", "$@23", "primary_value", "k_begin", "k_if",
  "k_unless", "k_while", "k_until", "k_case", "k_for", "k_class",
  "k_module", "k_def", "k_do", "k_do_block", "k_rescue", "k_ensure",
  "k_when", "k_else", "k_elsif", "k_end", "k_return", "then", "do",
  "if_tail", "opt_else", "for_var", "f_marg", "f_marg_list", "f_margs",
  "f_rest_marg", "f_any_kwrest", "f_eq", "$@24", "block_args_tail",
  "opt_block_args_tail", "excessed_comma", "block_param",
  "opt_block_param", "block_param_def", "opt_bv_decl", "bv_decls", "bvar",
  "lambda", "@25", "@26", "@27", "$@28", "f_larglist", "lambda_body",
  "do_block", "block_call", "method_call", "brace_block", "brace_body",
  "@29", "@30", "@31", "do_body", "@32", "@33", "@34", "case_args",
  "case_body", "cases", "p_case_body", "@35", "@36", "$@37", "p_cases",
  "p_top_expr", "p_top_expr_body", "p_expr", "p_as", "p_alt", "p_lparen",
  "p_lbracket", "p_expr_basic", "@38", "@39", "p_args", "p_args_head",
  "p_args_tail", "p_find", "p_rest", "p_args_post", "p_arg", "p_kwargs",
  "p_kwarg", "p_kw", "p_kw_label", "p_kwrest", "p_kwnorest",
  "p_any_kwrest", "p_value", "p_primitive", "p_variable", "p_var_ref",
  "p_expr_ref", "p_const", "opt_rescue", "exc_list", "exc_var",
  "opt_ensure", "literal", "strings", "string", "string1", "xstring",
  "regexp", "words", "word_list", "word", "symbols", "symbol_list",
  "qwords", "qsymbols", "qword_list", "qsym_list", "string_contents",
  "xstring_contents", "regexp_contents", "string_content", "@40", "$@41",
  "@42", "@43", "@44", "@45", "string_dvar", "symbol", "ssym", "sym",
  "dsym", "numeric", "simple_numeric", "nonlocal_var", "user_variable",
  "keyword_variable", "var_ref", "var_lhs", "backref", "superclass",
  "$@46", "f_opt_paren_args", "f_paren_args", "f_arglist", "@47",
  "args_tail", "opt_args_tail", "f_args", "args_forward", "f_bad_arg",
  "f_norm_arg", "f_arg_asgn", "f_arg_item", "f_arg", "f_label", "f_kw",
  "f_block_kw", "f_block_kwarg", "f_kwarg", "kwrest_mark", "f_no_kwarg",
  "f_kwrest", "f_opt", "f_block_opt", "f_block_optarg", "f_optarg",
  "restarg_mark", "f_rest_arg", "blkarg_mark", "f_block_arg",
  "opt_f_block_arg", "singleton", "$@48", "assoc_list", "assocs", "assoc",
  "operation", "operation2", "operation3", "dot_or_colon", "call_op",
  "call_op2", "opt_terms", "opt_nl", "rparen", "rbracket", "rbrace",
  "trailer", "term", "terms", "none", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1086)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-786)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1086,   168,  4730, -1086, 10445, -1086, -1086, -1086,  9903, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, 10571, 10571, -1086, -1086,
   -1086,  6363,  5922, -1086, -1086, -1086, -1086,   574,  9758,    26,
      48,    92, -1086, -1086, -1086,  5187,  6069, -1086, -1086,  5334,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, 12335, 12335,
   12335, 12335,   273,  7958, 10697, 11201, 11579, 10187, -1086,  9613,
   -1086, -1086, -1086,   196,   239,   259,   302,  1067, 12461, 12335,
   -1086,   418, -1086,  1061, -1086,   774,   345,   345, -1086, -1086,
     195,   441,   364, -1086,   404, 12713, -1086,   405,  3899,   546,
     317,   329, -1086, 12587, 12587, -1086, -1086,  8940, 12835, 12957,
   13079,  9467, 10571, -1086,    70,   122, -1086, -1086,   471, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086,    52,   353, -1086,   518,   678, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086,   486, -1086, -1086, -1086,   505,
   12335,   610,  8109, 12335, 12335, 12335, -1086, 12335,   345,   345,
   -1086,   552,  5753,   602, -1086, -1086,   592,   369,   274,   454,
     641,   464,   615, -1086, -1086,  8814, -1086, 10571, 10823, -1086,
   -1086,  9066, -1086, 12587,   618, -1086,   616,  8260, -1086,  8411,
   -1086, -1086,   621,   651,   195, -1086,   385, -1086,   736,  5900,
    5900,   742, 10697, -1086,  7958,   682,   418, -1086,  1061,    26,
     707, -1086,  1061,    26,   699,    85,   617, -1086,   602,   692,
     617, -1086,    26,   798,  1067, 13201,   713,   713,   719, -1086,
     516,   596,   609,   611, -1086, -1086, -1086, -1086, -1086,    81,
   -1086,   453,   501,   470, -1086, -1086, -1086, -1086,   795, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086,  9192, 12587, 12587, 12587,
   12587, 10697, 12587, 12587,  1785,   748,   754,  7102,  1785, -1086,
     761,  7102, -1086, -1086, -1086,   784, -1086, -1086, -1086, -1086,
   -1086,   823, -1086,  7958, 10316,   749,   823, -1086, 12335, 12335,
   12335, 12335, 12335, -1086, -1086, 12335, 12335, 12335, 12335, 12335,
   12335, 12335, 12335, -1086, 12335, -1086, -1086, 12335, 12335, 12335,
   12335, 12335, 12335, 12335, 12335, 12335, 12335, -1086, -1086, 13664,
   10571, 13754,  7102,   774,   151,   151,  8562, 12587,  8562,   418,
   -1086,   753,   844, -1086, -1086,   622,   884,    91,   121,   129,
     951,  1040, 12587,   379, -1086,   782,   661, -1086, -1086, -1086,
   -1086,    72,   304,   341,   344,   407,   420,   424,   510,   517,
   -1086, -1086, -1086, -1086,   548, -1086, -1086, -1086, 15014, -1086,
   -1086,   823,   823, -1086, -1086,   544, -1086, -1086, -1086,   904,
     804,   806,   823, 12335, 10949, -1086, -1086, 13844, 10571, 13934,
     823,   823, 11327, -1086,    26,   791, -1086, -1086, 12335,    26,
   -1086,   796,    26,   800, -1086,   159, -1086, -1086, -1086, -1086,
   -1086,  9903, -1086, 12335,   808,   813, 13844, 13934,   823,  1061,
      48,    26, -1086, -1086,  9318,   815,    26, -1086, -1086, 11453,
   -1086, -1086, 11579, -1086, -1086, -1086,   616,   669, -1086, -1086,
     821, -1086, 13201, 14024, 10571, 14114, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086,   623,    56,   686,
      68, 12335, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,  1015,
   -1086, -1086, -1086, -1086, -1086,   828, -1086,    26, -1086, -1086,
   -1086,   843, -1086,   830, 12335, -1086,   832,   108, -1086, -1086,
   -1086,   835,   932,   841,   941, -1086, 12461,   987,   992,   418,
   12461,   987,   862, -1086, -1086, -1086,   987, -1086,   987, -1086,
   11705, -1086,    26, 13201,   875, -1086, 11705, -1086,   736,  6047,
    6047,  6047,  6047,  6194,  4208,  6047,  6047,  5900,  5900,   550,
     550, -1086,  6485,  1260,  1260,  1391,   386,   386,   736,   736,
     736,  1376,  1376,  6510,  5481,  6804,  5628, -1086,   651, -1086,
      26,   878,   751, -1086,   768, -1086, -1086,  6216,   987, -1086,
    7253,  1009,  7706,   987,    88,   987,  1007,  1020,   130, 14204,
   10571, 14294, -1086,   774, -1086,   669, -1086, -1086, -1086, 14384,
   10571, 14474,  7102, 12587, -1086, -1086, -1086, -1086, -1086,  3812,
   12461, 12461,  9903, 12335, 12335, 12335, 12335, -1086, 12335,   602,
   -1086,   615,  5039,  5775,    26,   595,   647, 12335, 12335, -1086,
   -1086, -1086, -1086, 11075, -1086, 11327, -1086, -1086, 12587,  5753,
   -1086, -1086,   651,   651, 12335, -1086,   261, -1086, -1086,   617,
   13201,   821,   403,    65,    26,   257,   298,  1528, -1086,   934,
   -1086,    63, -1086,   886, -1086, -1086,    71,   891, -1086,   736,
    1015,  1114, -1086,   890,    26,   900, -1086,   209, -1086, -1086,
   -1086, 12335,   924,  1785, -1086, -1086,   639, -1086, -1086, -1086,
    1785, -1086, -1086,  1696, -1086, -1086,  1011,  5312, -1086, -1086,
   -1086, 11831,   659, -1086, -1086,  1012,  5459, -1086, -1086, -1086,
     913, -1086, -1086, -1086, 12335, -1086,   914,   916,  1021, -1086,
   -1086,   821, 13201, -1086, -1086,  1024,   933,  5165, -1086, -1086,
   -1086,   766,   676,  4880,  4880,   935,   823,   823, -1086,   784,
     923,   797, 10949,   823,   823, -1086, -1086,   784, -1086, -1086,
     801, -1086,  1059, -1086, -1086, -1086, -1086, -1086, -1086,  1020,
     987, -1086, 11957,   987,   105,   297,    26,   138,   147,  8562,
     418, 12587,  7102,  1081,    65, -1086,    26,   987,   159, 10048,
     122,   441, -1086,  5606, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086,   823,   823,   787,   823,   823,    26,   929,   159, -1086,
   -1086, -1086,   433,  1785, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086,    26, -1086,  1015,
   -1086,  1091, -1086, -1086, -1086, -1086, -1086,   936,   940, -1086,
    1029,   828,   944, -1086,   945, -1086,   944, 12335, 12335,   914,
   -1086,   993, -1086, -1086, -1086,  8562, -1086, -1086, -1086, 12335,
   12335,   956, -1086,   956,   949, 12083, 10697,   821, 10697,   823,
   12335, 14564, 10571, 14654, -1086, -1086, -1086,  4593,  4593,   455,
   -1086,  4442,    12,  1045, -1086,   119, -1086, -1086,   215, -1086,
     967, -1086, -1086, -1086,   963, -1086,   971, -1086, 13597, -1086,
   -1086, -1086, -1086,   876, -1086, -1086, -1086,    38, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086,   475, -1086, 12335,
   12461, 12461, -1086, -1086,   823, 12461, 12461, -1086, -1086,  8562,
   12587,   987, -1086, -1086,   987, -1086, -1086,   987, -1086, 12335,
   -1086,   118, -1086,   165,   987,  7102,   418,   987, -1086, -1086,
   -1086, -1086, -1086, -1086, 12335, 12335,   823, 12335, 12335, -1086,
   11327, -1086,    26,    78, -1086, -1086, -1086,   975,   985,  1785,
   -1086,  1696, -1086, -1086,  1696, -1086,  1696, -1086, -1086,  5753,
    5753, 13323,   151, -1086, -1086,  7832,  5753,  5753,  1232,  8411,
   -1086, -1086,  7102, 12335,   989, -1086, -1086, 12461,  5753,  6657,
    6951,    26,   793,   803, -1086, -1086, -1086, -1086, 13597,   286,
      26, 13478, -1086,    26,   994, -1086,   896,   995, -1086, -1086,
     930, -1086, -1086, -1086, -1086, 12587, -1086,  1063, 13564, 13597,
   13597,   896,  1033,  4593,  4593,   455,   465,   634,  4880,  4880,
   -1086, -1086,  5753, -1086, -1086, -1086, -1086, 12461, -1086, -1086,
   -1086, -1086, -1086,   151, -1086, -1086,  4880, -1086, -1086, 12209,
    7404, -1086,   987, -1086, -1086, 12335,  1001,   990,  7102,  8411,
   -1086, -1086,  1091,  1091,   944,  1008,   944,   944,  1082, -1086,
     779,   206,   214,   231,  7102,  1149,   828, -1086,    26,  1031,
     843,  1026, 13445, -1086,  1028, -1086,  1035,  1043, -1086, -1086,
   -1086,  1047,   827,    36, -1086,    60,  1033,  1049, -1086, -1086,
   -1086,    26, -1086, -1086,  1042, -1086, -1086,  1052, -1086,  1056,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,    26,    26,
      26,    26,    26,    26, -1086, -1086,  7253,   151,   969, 12335,
   -1086,   697, -1086, -1086,  1155,   987,  1060,  8688,   985, -1086,
    1696, -1086, -1086, -1086,   283, 14744, 10571, 14834,   992, -1086,
   -1086,  1044, -1086, 13445,  1358, -1086, -1086,  1152,   788,   639,
   -1086,  1358, -1086,  1466, -1086, -1086,  1068, 13597, -1086,   515,
   -1086, -1086, 13597, 13564, -1086, -1086, -1086, -1086, -1086, -1086,
     801, -1086, 12587, 12587, -1086, -1086, -1086, -1086, -1086,   698,
   -1086, -1086, -1086, -1086,  1095,   944,   182,   199,    26,   333,
     338, -1086, -1086,   788, -1086,  1071,  1073, -1086, 14924, -1086,
     828,  1077, -1086,  1079,  1077, 13597,  1096,  1096, -1086, -1086,
   -1086,  7555, -1086, -1086,  1155, -1086, -1086, -1086,   367,  1358,
   -1086,  1466, -1086,  1098,  1102, -1086,  1466, -1086,  1466, -1086,
   -1086,  1096, 13597,   586, -1086,  1077,  1097,  1077,  1077, -1086,
   -1086, -1086, -1086,  1466, -1086, -1086, -1086,  1077, -1086
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     0,     1,     0,   374,   375,   376,     0,   367,
     368,   369,   372,   370,   371,   373,   362,   363,   364,   365,
     385,   295,   295,   658,   657,   659,   660,   773,     0,   773,
       0,     0,   662,   661,   663,   755,   757,   654,   653,   756,
     656,   645,   646,   647,   648,   596,   668,   669,     0,     0,
       0,     0,     0,     0,   323,   785,   785,   105,   444,   616,
     616,   618,   620,     0,     0,     0,     0,     0,     0,     0,
       3,   771,     6,     9,    35,    40,   677,   677,    56,    77,
     295,    76,     0,    93,     0,    97,   107,     0,    67,   245,
     264,     0,   321,     0,     0,    73,    73,   771,     0,     0,
       0,     0,   332,   343,    78,   341,   310,   311,   595,   597,
     312,   313,   314,   316,   315,   317,   594,   635,   636,   593,
     643,   664,   665,   318,     0,   319,    81,     5,     8,   186,
     197,   187,   210,   183,   203,   193,   192,   213,   214,   208,
     191,   190,   185,   211,   215,   216,   195,   184,   198,   202,
     204,   196,   189,   205,   212,   207,   206,   199,   209,   194,
     182,   201,   200,   181,   188,   179,   180,   176,   177,   178,
     136,   138,   137,   171,   172,   167,   149,   150,   151,   158,
     155,   157,   152,   153,   173,   174,   159,   160,   164,   168,
     154,   156,   146,   147,   148,   161,   162,   163,   165,   166,
     169,   170,   175,   141,   143,    28,   139,   140,   142,     0,
     752,     0,     0,     0,     0,   298,   616,     0,   677,   677,
     290,     0,   273,   301,    91,   294,   785,     0,   664,   665,
       0,   319,   785,   748,    92,   773,    89,     0,   785,   464,
      88,   773,   774,     0,     0,    23,   257,     0,    10,     0,
     362,   363,   335,   465,     0,   239,     0,   332,   240,   230,
     231,   329,     0,    21,     0,     0,   771,    17,    20,   773,
      95,    16,   325,   773,     0,   778,   778,   274,     0,     0,
     778,   746,   773,     0,     0,     0,   677,   677,   103,   366,
       0,   113,   114,   121,   445,   640,   639,   641,   638,     0,
     637,     0,     0,     0,   603,   612,   608,   614,   644,    60,
     251,   252,   781,   782,     4,   783,   772,     0,     0,     0,
       0,     0,     0,     0,   700,     0,   676,     0,   700,   674,
       0,     0,   377,   469,   458,    82,   473,   340,   378,   473,
     454,   785,   109,     0,   101,    98,   785,    64,     0,     0,
       0,     0,     0,   267,   268,     0,     0,     0,     0,   228,
     229,     0,     0,    61,     0,   265,   266,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   767,   768,     0,
     785,     0,     0,    72,     0,     0,     0,     0,     0,   771,
     350,   772,     0,   396,   395,     0,     0,   664,   665,   319,
     131,   132,     0,     0,   134,   672,     0,   664,   665,   319,
     358,   206,   199,   209,   194,   176,   177,   178,   136,   137,
     744,    69,    68,   743,     0,    90,   770,   769,     0,   342,
     598,   785,   785,   144,   751,   329,   302,   754,   297,     0,
       0,     0,   785,     0,     0,   291,   300,     0,   785,     0,
     785,   785,     0,   292,   773,     0,   334,   296,   701,   773,
     286,   785,   773,   785,   285,   773,   339,    59,    25,    27,
      26,     0,   336,     0,     0,     0,     0,     0,   785,    19,
       0,   773,   327,    15,   772,    94,   773,   324,   330,   780,
     779,   275,   780,   277,   331,   747,     0,   120,   644,   111,
     106,   676,     0,     0,   785,     0,   446,   622,   642,   625,
     623,   617,   599,   600,   619,   601,   621,     0,     0,     0,
       0,     0,   784,     7,    29,    30,    31,    32,    33,    57,
      58,   707,   704,   703,   702,   705,   713,   722,   701,     0,
     734,   723,   738,   737,   733,   785,   699,   773,   683,   706,
     708,   709,   711,   685,   715,   720,   785,   726,   409,   408,
     731,   685,   736,   685,   740,   682,     0,     0,   785,     0,
       0,     0,     0,   470,   469,    83,     0,   474,     0,   271,
       0,   272,   773,     0,    99,   110,     0,    65,   237,   244,
     246,   247,   248,   255,   256,   249,   250,   226,   227,   253,
     254,    62,   773,   241,   242,   243,   232,   233,   234,   235,
     236,   269,   270,   758,   760,   759,   761,   463,   295,   461,
     773,   785,   758,   760,   759,   761,   462,   295,     0,   387,
       0,   386,     0,     0,     0,     0,   348,     0,   329,     0,
     785,     0,    73,   356,   131,   132,   133,   670,   354,     0,
     785,     0,     0,     0,   765,   766,    70,   758,   759,   295,
       0,     0,     0,     0,     0,     0,     0,   750,     0,   303,
     299,   785,   758,   759,   773,   758,   759,     0,     0,   749,
     333,   775,   280,   287,   282,   289,   338,    24,     0,   258,
      11,    34,     0,   785,     0,    22,    96,    18,   326,   778,
       0,   104,   762,   119,   773,   758,   759,   700,   626,     0,
     602,     0,   605,     0,   610,   607,     0,     0,   611,   238,
       0,   407,   399,   401,   773,   404,   397,     0,   681,   742,
     675,     0,     0,     0,   692,   714,     0,   680,   724,   725,
       0,   695,   735,     0,   697,   739,    48,   260,   384,   360,
     379,   785,   785,   585,   678,    50,   262,   361,   467,   471,
       0,   468,   475,   453,     0,    36,   306,     0,    39,   305,
     108,   102,     0,    55,    41,    53,     0,   278,   301,   217,
      37,     0,   319,     0,     0,     0,   785,   785,   460,    86,
       0,   466,   287,   785,   785,   284,   459,    84,   283,   322,
     785,   388,   785,   346,   390,    74,   389,   347,   484,     0,
       0,   381,     0,     0,   762,   328,   773,   758,   759,     0,
       0,     0,     0,   131,   132,   135,   773,     0,   773,     0,
     455,    79,    42,   278,   218,    52,   225,   145,   753,   304,
     293,   785,   785,   466,   785,   785,   773,   785,   773,   224,
     276,   112,   466,   700,   447,   450,   627,   631,   632,   633,
     624,   634,   604,   606,   613,   609,   615,   773,   406,     0,
     710,     0,   741,   727,   411,   684,   712,   685,   685,   721,
     726,   785,   685,   732,   685,   709,   685,     0,     0,   586,
     587,   785,   588,   380,   382,     0,    12,    14,   592,     0,
       0,   785,    80,   785,   309,     0,     0,   100,     0,   785,
       0,     0,   785,     0,   577,   583,   550,     0,     0,     0,
     522,   773,   519,   538,   616,     0,   576,    66,   493,   499,
     501,   503,   497,   496,   534,   498,   543,   546,   549,   555,
     556,   545,   506,   557,   507,   562,   563,   564,   567,   568,
     569,   570,   571,   573,   572,   574,   575,   553,    63,     0,
       0,     0,    87,   776,   785,     0,     0,    85,   383,     0,
       0,     0,   391,   393,     0,    75,   485,     0,   352,     0,
     477,     0,   351,   466,     0,     0,     0,     0,   466,   359,
     745,    71,   456,   457,     0,     0,   785,     0,     0,   281,
     288,   337,   773,     0,   628,   398,   400,   402,   405,     0,
     688,     0,   690,   679,     0,   696,     0,   693,   698,    49,
     261,     0,     0,   590,   591,     0,    51,   263,   773,     0,
     435,   434,     0,     0,   307,    38,    54,     0,   279,   758,
     759,   773,   758,   759,   565,   566,   132,   581,     0,   524,
     773,   525,   531,   773,     0,   518,     0,     0,   521,   537,
       0,   578,   650,   649,   651,     0,   579,     0,   494,     0,
       0,   544,   548,   560,   561,     0,   505,   504,     0,     0,
     554,   552,   259,    47,   222,    46,   223,     0,    44,   220,
      45,   221,   394,     0,   344,   345,     0,   349,   478,     0,
       0,   353,     0,   671,   355,     0,     0,   438,     0,     0,
     448,   629,     0,     0,   685,   685,   685,   685,     0,   589,
       0,   664,   665,   319,     0,   785,   785,   433,   773,     0,
     709,   417,   717,   718,   785,   729,   417,   417,   415,   472,
     476,   308,   466,   773,   516,   529,   541,   526,   517,   532,
     616,   773,   777,   551,     0,   500,   495,   534,   502,   535,
     539,   547,   542,   558,   559,   582,   515,   511,   773,   773,
     773,   773,   773,   773,    43,   219,     0,     0,   490,     0,
     479,   785,   357,   449,     0,     0,     0,     0,   403,   689,
       0,   686,   691,   694,   329,     0,   785,     0,   785,    13,
     414,     0,   436,     0,   418,   426,   424,     0,   716,     0,
     413,     0,   429,     0,   431,   523,   527,     0,   533,     0,
     520,   580,     0,     0,   508,   509,   510,   512,   513,   514,
     785,   486,     0,     0,   480,   482,   483,   481,   442,   773,
     440,   443,   452,   451,     0,   685,   762,   328,   773,   758,
     759,   584,   437,   728,   416,   417,   417,   329,     0,   719,
     785,   417,   730,   417,   417,     0,   530,   535,   536,   540,
     392,     0,   491,   492,     0,   439,   630,   687,   466,     0,
     421,     0,   423,   762,   328,   412,     0,   430,     0,   427,
     432,   528,     0,   785,   441,   417,   417,   417,   417,   488,
     489,   487,   422,     0,   419,   425,   428,   417,   420
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1086, -1086, -1086,   997, -1086,    47,   772,  -312, -1086,   -24,
   -1086,   769, -1086,    74, -1086,  -427,  -543,   -32, -1086, -1086,
   -1086, -1086,   426,  2327,  2557, -1086,   -12,   -74, -1086, -1086,
     -10, -1086,  -640,  1214,    -9,  1159,  -125,    34,   -42, -1086,
    -406,    32,  3056,  -372,  1160,   -55,   -15, -1086, -1086,    -7,
   -1086,  4007, -1086,  1170, -1086,   927,   668, -1086,   714,     8,
     604,  -377,    97,     6, -1086,  -404,  -193,    17, -1086,  -483,
     -19, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
     918, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086,   512, -1086,   811,  1895,
    -376, -1086,    35,  -777, -1086,  -768,  -788,   549,   401,  -296,
     141, -1086,   244,  -438, -1086, -1086,   373, -1086,  -896, -1086,
       3,  1019, -1086, -1086, -1086, -1086, -1086, -1086,   447, -1086,
   -1086,   -99,   705, -1086, -1086, -1086,   946, -1086, -1086, -1086,
   -1086,  -762, -1086,   -13, -1086, -1086, -1086, -1086, -1086,  -728,
    -258, -1086, -1086, -1086, -1086,   223, -1086, -1086,   -38, -1086,
    -715,  -829,  -963,  -613,  -750,  -119, -1086,   212, -1086, -1086,
   -1086,   222, -1086,  -774,   230, -1086, -1086, -1086,   101, -1086,
   -1086,   176,  1381,  1652, -1086,  1195,  1974,  2151,  2180, -1086,
     794,  2262, -1086,  2645,  2678, -1086, -1086,   -58, -1086, -1086,
    -261, -1086, -1086, -1086, -1086, -1086, -1086, -1086,     9, -1086,
   -1086, -1086, -1086,    30, -1086,  3022,    10,  1211,  3297,  1722,
   -1086, -1086,    29,   727,    23, -1086,  -308,  -381,  -305,  -206,
   -1060,  -513,  -313,  -698,  -451,  -500,   578,   106, -1086, -1086,
    -663, -1086,  -709,  -696, -1085,   113,   585, -1086,  -649, -1086,
    -433,  -532, -1086, -1086, -1086,    43,  -402, -1086,  -324, -1086,
   -1086,   -86, -1086,   -20,   123,   482,   440,   170,  -231,   -61,
      31,    -2
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    70,    71,    72,   248,   567,  1025,   568,
     266,   267,   480,   268,   471,    74,   774,    75,   601,   784,
     587,   783,   421,   218,   219,   829,   384,   386,   387,   975,
      78,    79,   575,   254,    81,    82,   269,    83,    84,    85,
     500,    86,   221,   404,   405,   203,   204,   205,   662,   616,
     207,    88,   473,   375,    89,   580,   223,   274,   779,   617,
     796,   459,   460,   236,   237,   225,   445,   621,   768,   769,
      90,   382,   273,   486,   688,   809,   637,   822,   820,   652,
     256,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   336,   339,   751,   895,   812,   969,   970,   749,   257,
     630,   805,   971,   972,   396,   722,   723,   724,   725,   545,
     731,   732,  1254,  1205,  1206,  1128,  1029,  1030,  1106,  1239,
    1240,   103,   294,   506,   707,  1003,   854,  1110,   340,   104,
     105,   337,   572,   573,   759,   901,   576,   577,   762,   903,
     981,   813,  1237,   810,   976,  1096,  1271,  1301,  1177,   927,
    1146,   929,   930,  1078,  1079,   931,  1056,  1048,  1050,  1051,
    1052,   933,   934,  1159,  1054,   935,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,   946,   947,   752,   891,
    1022,   897,   106,   107,   108,   109,   110,   111,   112,   517,
     711,   113,   519,   114,   115,   518,   520,   299,   302,   303,
     511,   709,   708,   856,  1004,  1111,  1187,   860,   116,   117,
     300,   118,   119,   120,  1066,   228,   229,   123,   230,   231,
     648,   821,   325,   326,   327,   328,   875,   734,   547,   548,
     549,   550,   885,   552,   553,   554,   555,  1133,  1134,   556,
     557,   558,   559,   560,  1135,  1136,   561,   562,   563,   564,
     565,   728,   424,   653,   279,   463,   233,   126,   692,   619,
     656,   651,   428,   314,   455,   456,   791,  1058,   491,   631,
     391,   271
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     127,   206,   301,   620,   298,   381,   429,   220,   220,   632,
     315,   551,   122,   245,   122,   551,   546,   208,   427,   571,
     546,   206,   388,   569,   737,   974,   726,   881,   240,   265,
     239,   646,   462,   226,   226,   876,   315,   208,   289,   453,
     670,   514,   516,   345,   883,   493,   422,   977,   670,   495,
     679,   128,   206,   277,   281,   618,   958,   627,   309,   232,
     232,   383,   383,   122,   122,   383,   289,   292,   932,   932,
     628,   674,   275,   880,   329,   329,    73,   390,    73,   289,
     289,   289,   385,  1008,   878,   389,   335,   270,   334,   288,
     679,   884,  1053,   220,   206,   292,   701,   308,   276,   280,
     331,  1006,   316,   780,   659,  1157,   330,  1108,   398,   408,
     408,   408,  1216,   224,   234,   804,  -122,   832,   835,   226,
     957,   957,   713,   618,  1241,   627,  1262,   704,   272,   507,
    -126,   629,  1129,  -328,   717,  1075,  1067,   507,   377,   481,
    -658,   449,   738,  1044,  1045,   232,  -123,   507,   332,   962,
     243,  -666,   246,   693,  -130,  -129,  -773,   967,   439,   773,
     739,  -328,  -328,  -125,   629,   242,   378,   426,     3,  -658,
     477,  1061,  -127,  1062,  1063,   714,  1064,   771,  -122,   242,
     741,   693,   744,  1076,   509,   510,  1077,   718,   265,   242,
    -124,   247,   509,   510,  1262,  -126,   322,   323,  -113,   425,
     508,  1109,   509,   510,   505,   315,  1217,   726,   868,   465,
    -328,   467,  -128,  -759,  1241,   862,   329,   329,   582,  -122,
    1065,   332,   122,   865,   446,   475,   220,  -123,   220,   220,
     446,   489,  1201,   773,   773,   249,   464,  -113,   490,   312,
     265,   313,   331,   499,  -130,   127,   483,   440,   441,  1169,
    1172,  -117,   226,  -758,   226,   461,   712,   122,   712,   122,
    1268,   239,   334,   816,  1099,   333,   289,  -114,   890,   312,
     453,   313,   122,   826,   122,  -121,  -120,   825,   232,   670,
     232,   670,  -116,   679,  -116,   383,   383,   383,   383,   882,
     529,   530,   886,  -118,   872,   292,  -129,   484,   270,  1163,
    1164,  1147,   312,   872,   313,   524,   525,   526,   527,   641,
     331,  -115,   242,   883,   498,  1067,   876,   693,   542,   265,
    1160,    73,  -128,  -118,  1188,   289,   122,   693,   315,   261,
    -758,   122,   454,  -126,   457,  -126,   479,   122,   333,   581,
     827,   122,   543,   238,   581,  1006,  -125,  -759,   304,  1013,
    -128,  -127,  -128,   122,   292,   383,   726,  -122,   726,  -122,
    1115,  1068,   633,   523,   635,  -123,   907,  -123,  1178,   636,
     643,   220,  -657,  -666,   625,   634,   584,   270,   464,  1137,
    -124,   932,  -130,  -116,  -130,  -366,  1067,   -94,   670,   626,
      73,   305,   122,   957,   551,   528,   122,   377,   122,   546,
    -122,  -657,   855,  -116,  1235,   496,  -116,  -108,   957,  -659,
    -116,   306,  -660,  -366,  -366,   957,   957,  1083,  1085,  1236,
     551,   625,  1088,  1090,  -118,   378,   379,   551,  -117,   581,
     581,   644,  -541,   957,  -129,   645,  -129,   377,  -659,   220,
     581,  -660,   625,  -119,  -118,  -759,   464,  -118,   581,   581,
     863,  -118,  -667,   377,   307,   863,   687,   626,  -115,   446,
     499,   446,  -366,   348,   206,   378,   447,  1160,   850,   338,
     625,  -785,  1160,  1269,   380,  -662,   581,   846,   840,  -123,
     208,   378,   476,   289,  -125,   626,  -125,   671,  -661,  -127,
     341,  -127,  -663,   324,   122,   220,  1010,  1012,   625,  -114,
    1260,  1015,   464,  1017,  -662,  1018,   876,   644,   754,  1080,
     987,  1046,   292,   626,   448,  1160,  1299,  -661,  -124,   507,
    -124,  -663,   372,   373,   374,   928,   928,  1081,  1132,  -117,
     448,   346,   699,   773,   773,  1041,   507,   515,   773,   773,
     551,   499,  1269,   729,  1174,   546,   880,  1047,  1002,  -117,
     342,  -758,  -117,  -667,   729,  1256,  -117,   746,  1114,  -115,
    1116,   755,  1263,   451,   289,  1117,   753,   507,   819,   312,
     765,   313,   512,   806,   509,   510,   775,  1131,  -652,  -115,
    -123,   507,  -115,    60,   377,  -655,  -115,   618,   876,   627,
    -130,   509,   510,   292,  1200,  1138,   670,   767,   679,   726,
     726,   894,  1210,   767,  1266,  1100,   800,  -652,   802,  1267,
     773,   808,   378,   503,  -655,  -773,   654,   431,   242,   446,
     513,   828,   509,   510,   789,   798,   788,   348,   353,   354,
    1296,   220,   433,   797,   625,   795,   509,   510,   464,  1153,
     122,   220,   122,   478,   625,   655,  1124,   837,   464,   626,
     775,   775,  1291,   241,   840,   206,   848,   798,   851,   626,
     773,   504,   122,  1049,  -664,   831,   435,   795,   893,   446,
    -129,   208,   468,  1102,   894,   365,   366,  -665,   442,  -319,
    1072,   289,   469,   470,   370,   371,   372,   373,   374,   507,
     377,   798,  -664,  -664,   844,   913,   551,   536,  1212,  1214,
     788,   795,   443,  1165,  1132,  -665,  -665,  -319,  -319,  1132,
     292,  1132,   894,  1132,   811,  1130,   537,  1176,   378,   639,
    1140,  -125,   241,   466,   278,   785,   847,   242,  1285,   377,
     499,   992,  1126,  1189,  1191,  1192,  1193,  -329,   444,  1245,
     450,  -664,   710,   790,   509,   510,   845,   541,   542,   892,
     898,   485,   507,   289,  -665,   487,  -319,   378,   649,   985,
    1261,   452,  1264,   492,   472,  -329,  -329,   640,   767,   235,
     490,  1138,   543,  -127,   422,   432,   872,   432,  1138,  1132,
    1138,  1132,   292,  -773,   581,   581,  1132,   242,  1132,   383,
    1143,   581,   581,   956,   956,   984,  1185,   790,   973,   238,
     973,  1231,  -130,  1132,  -130,   715,   650,   509,   510,   986,
    1049,   322,   323,   348,  -329,   968,   894,  1280,  1282,  1248,
    1049,  1049,   206,  1287,  -121,  1289,  1290,   790,  1295,   122,
    1297,   482,   122,   -93,   377,   847,   494,  1298,   928,   581,
     581,   478,   581,   581,  1274,   446,  1138,   377,  1138,   488,
     793,   242,  1307,  1138,   497,  1138,   377,  1302,  1304,  1305,
    1306,   324,   378,   911,  1277,   502,  1060,   794,  -129,  1308,
    1138,  1024,   521,   693,   566,   378,  1195,  -125,   434,   729,
    -673,   436,   437,   438,   378,  1258,   996,   570,  -120,  1023,
     574,  1130,   793,   579,  -127,   585,   964,  -116,  1130,  1031,
     638,  1031,   794,   220,   522,   122,   625,   581,  1126,   642,
     464,   912,   647,  -124,  -118,  1126,   122,  1126,   122,  -125,
      91,   626,    91,  -124,  1196,  1103,   964,   956,   956,  -127,
     664,   956,   665,   650,   227,   227,   680,  1151,   383,   790,
     681,   682,   683,  -115,   684,  1092,   685,   686,   956,   790,
     775,   775,   690,  -124,   916,   775,   775,   691,  1093,  1170,
    1173,  -108,   581,   696,  1073,  1074,  1130,   700,   698,  -410,
     507,    91,    91,   537,   727,   290,   733,  -755,   736,   122,
    1035,   740,  1036,  1126,   742,  1126,   227,   743,   857,   858,
    1126,   859,  1126,   745,   581,   122,   507,   748,    46,    47,
     750,  1125,   289,   290,   541,  1139,   758,  1126,  1150,  1232,
    1233,   227,   227,   501,   501,   227,   395,   406,   406,  -652,
     227,   772,   801,   512,   792,   509,   510,   775,   663,   730,
    1156,  1122,   808,   383,  1197,   122,   869,   811,   864,   122,
    1168,  1171,   122,   866,   790,  1057,   871,  -652,  -652,   512,
     874,   509,   510,  1154,  1153,   887,   899,   902,   956,   909,
    -301,   956,   905,   959,   770,   906,  -756,   531,   908,   532,
     533,   534,   535,   963,   894,  1000,  1181,   775,   956,   956,
     956,   739,  1009,   956,   956,  1186,  1011,  1028,   956,   956,
    1014,  1016,  1219,  1021,  -755,  -302,  -652,  1059,  1069,  -755,
    1198,   317,   318,   319,   320,   321,   956,  -762,  -655,  1070,
     122,   667,   669,   289,   843,   914,   720,  1071,   122,   122,
     278,  1112,   721,   898,   729,  1107,    41,    42,    43,    44,
      91,  1113,   729,  1067,   122,  -303,  -655,  -655,  1194,  1152,
    1149,  1184,   408,   531,   852,   532,   533,   534,   535,  -762,
    1183,  1107,  1230,   227,  1190,   227,   227,   669,   893,   227,
     278,   227,  1202,  1244,   790,    91,   531,    91,   532,   533,
     534,   535,  1204,   790,  1209,  1252,   790,  -762,  -762,   973,
      91,  1211,    91,  -756,   289,  -655,   122,   220,  -756,  1213,
     625,  1221,   720,  -304,   464,  1218,   753,   122,  1222,   790,
     383,   383,  1223,   290,  1243,   626,   870,  1238,  1257,   532,
     533,   534,   535,   408,  1265,  1276,    80,  1279,    80,  1281,
    1272,  1273,   735,  1286,  -762,  1288,  -762,   956,   973,  -758,
      80,    80,   956,   956,    91,   227,   227,   227,   227,    91,
     227,   227,  1292,  1303,   474,    91,  -758,  1293,   766,    91,
    -759,  1107,   695,   697,   778,   991,   983,   393,   729,   376,
     410,    91,   290,   830,   896,  1270,   988,    80,    80,   867,
    1007,  1203,  1127,   586,  1057,   956,  1032,  1294,   993,   760,
    1300,   122,    80,  1161,   531,   578,   532,   533,   534,   535,
     536,   973,  1158,  1162,   790,   790,   790,  1155,   227,  1251,
      91,  1199,   956,   430,    91,   227,    91,    80,    80,   537,
     990,    80,   423,   716,   879,  1259,    80,  1255,   877,     0,
     227,  1220,     0,     0,     0,     0,     0,     0,   999,     0,
    1001,   838,     0,   539,     0,     0,   839,   348,     0,   540,
     541,   542,     0,     0,     0,     0,     0,     0,     0,  1005,
       0,   669,     0,   278,   361,   362,     0,     0,   660,   661,
       0,  1055,  1275,     0,     0,   543,   227,     0,   544,   666,
       0,   790,     0,     0,   834,   836,     0,   677,   678,     0,
       0,     0,   757,     0,     0,   242,     0,   761,     0,   763,
       0,   834,   836,   369,   370,   371,   372,   373,   374,   873,
       0,     0,    91,     0,     0,   694,     0,     0,   849,     0,
     531,     0,   532,   533,   534,   535,   536,     0,     0,   889,
     290,     0,   227,     0,     0,     0,    80,     0,     0,     0,
       0,     0,   904,     0,     0,   537,     0,     0,     0,   799,
       0,     0,     0,     0,   803,     0,   807,     0,     0,    80,
       0,    80,    80,   348,     0,    80,     0,    80,     0,   539,
     669,    80,     0,    80,     0,   540,   541,   542,   348,     0,
     361,   362,     0,     0,     0,     0,    80,     0,    80,     0,
     980,  1142,     0,     0,   227,   361,   362,     0,   227,     0,
    1144,   543,     0,  1148,   544,     0,     0,     0,   227,     0,
       0,   290,     0,     0,   781,     0,     0,   367,   368,   369,
     370,   371,   372,   373,   374,     0,  1166,     0,   531,     0,
     532,   533,   534,   535,   536,   370,   371,   372,   373,   374,
      80,    80,    80,    80,    80,    80,    80,    80,     0,     0,
       0,    80,     0,   537,     0,    80,     0,     0,    91,     0,
      91,     0,     0,     0,     0,     0,     0,    80,   227,  1167,
       0,     0,     0,     0,     0,     0,     0,   539,   227,     0,
      91,   227,     0,  1034,   541,   542,     0,     0,   781,   781,
     531,     0,   532,   533,   534,   535,   536,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    80,     0,     0,   543,
      80,    80,    80,     0,     0,   537,   227,     0,     0,     0,
       0,  1227,  1228,  1229,     0,     0,    80,     0,   290,   538,
       0,   978,     0,     0,   982,  1215,     0,     0,     0,   539,
       0,     0,     0,     0,     0,   540,   541,   542,   989,     0,
       0,     0,     0,     0,     0,     0,     0,  1098,     0,     0,
    1224,  1225,  1226,     0,     0,     0,     0,     0,     0,     0,
       0,   543,    80,     0,   544,     0,     0,     0,   278,     0,
       0,     0,     0,     0,  1084,  1086,   853,     0,     0,  1089,
    1091,     0,     0,     0,     0,     0,     0,     0,  1278,     0,
     290,     0,     0,     0,     0,     0,     0,     0,    80,     0,
       0,  1141,     0,     0,     0,     0,     0,     0,  1084,  1086,
       0,  1089,  1091,   960,   961,     0,     0,     0,    80,     0,
     965,   966,     0,     0,   125,     0,   125,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,   227,
      91,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,   533,   534,   535,   536,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1180,   994,   995,
       0,   997,   998,   537,     0,   125,   125,     0,     0,   293,
      80,     0,  1094,     0,    80,  1095,     0,   538,  1097,     0,
       0,     0,     0,     0,    80,  1101,     0,   539,  1104,     0,
      80,  1175,   926,   926,   541,   542,     0,   293,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,  1175,
     399,   409,   409,     0,    91,     0,    91,     0,     0,   543,
     227,     0,     0,     0,     0,     0,  1037,   531,     0,   532,
     533,   534,   535,   536,    80,     0,    80,  1234,     0,     0,
       0,     0,     0,     0,    80,     0,     0,     0,     0,     0,
       0,     0,   537,     0,    80,     0,    80,    80,     0,     0,
       0,     0,     0,     0,    80,    80,   538,     0,   781,   781,
       0,     0,     0,   781,   781,     0,   539,    91,   227,     0,
       0,  1087,   540,   541,   542,     0,     0,   102,     0,   102,
       0,     0,    80,    91,     0,     0,     0,     0,     0,     0,
       0,   102,   102,  1182,     0,     0,     0,     0,   543,     0,
       0,   544,     0,  1105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   125,     0,   926,   926,     0,  1120,
     926,     0,     0,    91,     0,     0,     0,    91,   102,   102,
      91,     0,     0,     0,     0,   781,     0,   926,     0,     0,
       0,     0,     0,   102,     0,     0,     0,     0,     0,   125,
       0,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   227,   125,     0,   125,     0,   102,   102,
       0,     0,   102,     0,     0,     0,  1242,   102,     0,     0,
       0,     0,     0,     0,     0,   781,     0,   293,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,    91,    91,     0,     0,
       0,     0,     0,    80,     0,    80,    80,     0,   125,     0,
       0,     0,    91,   125,     0,     0,     0,     0,     0,   125,
    1208,     0,     0,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   125,   293,   926,     0,     0,
     926,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   926,   926,   926,
       0,     0,   926,   926,    91,     0,     0,   926,   926,     0,
       0,     0,     0,     0,   125,    91,     0,   102,   125,    80,
     125,     0,     0,     0,   227,   926,     0,     0,     0,     0,
      80,  1253,    80,     0,     0,     0,    80,     0,     0,     0,
     102,     0,   102,   102,     0,     0,   102,     0,   102,     0,
       0,     0,   102,     0,   102,     0,     0,     0,     0,     0,
     227,   227,     0,     0,     0,     0,     0,   102,     0,   102,
       0,     0,     0,     0,   948,   948,     0,     0,     0,     0,
       0,     0,     0,     0,    80,    80,     0,     0,     0,    80,
      80,     0,     0,    80,    80,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,     0,     0,     0,     0,     0,   125,     0,     0,     0,
       0,   102,   102,   102,   102,   102,   102,   102,   102,     0,
       0,     0,   102,     0,   293,     0,   102,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   926,     0,   102,    80,
       0,   926,   926,    80,     0,     0,    80,     0,     0,     0,
       0,    80,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,   102,     0,    80,
       0,   102,   102,   102,   926,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   102,   948,   948,
       0,    80,   948,     0,     0,   293,     0,     0,   782,     0,
       0,   926,     0,     0,    80,     0,     0,     0,     0,   948,
       0,     0,    80,    80,     0,     0,     0,     0,     0,    76,
       0,    76,     0,     0,     0,     0,     0,     0,    80,     0,
       0,     0,     0,   102,     0,     0,     0,     0,     0,     0,
       0,     0,   125,     0,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   125,     0,     0,     0,     0,   102,
      76,    76,   782,   782,   286,     0,     0,     0,     0,     0,
      80,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,    80,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,   286,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   293,     0,     0,   286,   286,   286,     0,   948,
       0,   861,   948,     0,     0,   949,   949,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    80,    80,     0,   948,
     948,   948,     0,     0,   948,   948,     0,     0,     0,   948,
     948,   102,     0,     0,     0,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,   948,     0,     0,
       0,   102,     0,     0,     0,    80,     0,     0,     0,     0,
       0,     0,     0,     0,   293,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,   102,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,    76,
       0,   125,     0,     0,   125,   102,     0,   102,   102,     0,
       0,     0,     0,     0,     0,   102,   102,     0,     0,    77,
       0,    77,     0,     0,     0,     0,     0,     0,     0,   949,
     949,     0,     0,   949,    76,     0,    76,     0,     0,     0,
       0,     0,     0,   102,     0,     0,     0,     0,     0,    76,
     949,    76,     0,     0,     0,     0,     0,     0,   948,     0,
       0,     0,     0,   948,   948,     0,     0,     0,     0,     0,
      77,    77,   286,     0,   287,     0,     0,   125,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   125,     0,
     125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   287,    76,     0,     0,   948,     0,    76,     0,
       0,     0,     0,     0,    76,   287,   287,   287,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      76,   286,     0,   948,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   782,     0,     0,     0,   782,   782,     0,
       0,   125,     0,     0,     0,     0,     0,     0,     0,     0,
     949,     0,     0,   949,     0,     0,     0,   125,     0,    76,
       0,     0,     0,    76,   102,    76,   102,   102,     0,     0,
     949,   949,   949,     0,     0,   949,   949,     0,     0,     0,
     949,   949,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1123,     0,     0,     0,   125,   949,     0,
       0,   125,     0,     0,   125,     0,     0,   950,   950,   782,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,   102,    77,     0,    77,   102,     0,   782,
       0,    76,     0,     0,     0,     0,     0,     0,     0,    77,
       0,    77,   125,     0,     0,     0,     0,     0,     0,   286,
     125,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   287,     0,     0,     0,   125,     0,     0,     0,
       0,     0,     0,     0,   409,   102,   102,     0,     0,     0,
     102,   102,     0,     0,   102,   102,     0,     0,     0,   949,
       0,     0,     0,    77,   949,   949,     0,     0,    77,     0,
     102,     0,     0,     0,    77,     0,     0,     0,    77,     0,
       0,   950,   950,     0,     0,   950,     0,     0,   125,     0,
      77,   287,     0,     0,     0,     0,     0,     0,     0,   125,
     286,     0,   950,    76,     0,     0,     0,   949,     0,     0,
     102,     0,     0,     0,   102,   409,     0,   102,     0,     0,
       0,     0,   102,     0,   951,   951,     0,     0,     0,    77,
       0,     0,     0,    77,   949,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    76,     0,    76,
     102,     0,     0,   952,   952,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,   102,     0,     0,     0,     0,    76,    76,     0,
       0,     0,     0,   125,     0,   102,     0,     0,     0,     0,
       0,     0,     0,   102,   102,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   950,     0,   121,   950,   121,   286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,   950,   950,   950,   953,   953,   950,   950,     0,
       0,     0,   950,   950,     0,     0,     0,     0,    87,   287,
      87,     0,     0,     0,     0,     0,     0,     0,   951,   951,
     950,   102,   951,     0,     0,   121,   121,     0,     0,   291,
       0,     0,   102,     0,     0,     0,     0,     0,     0,   951,
       0,   102,     0,     0,     0,     0,     0,   952,   952,   286,
       0,   952,     0,     0,     0,     0,     0,   291,     0,    87,
      87,     0,     0,     0,     0,     0,     0,     0,   952,     0,
     397,   407,   407,   407,     0,     0,     0,   102,   102,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     287,     0,     0,    77,     0,     0,    76,     0,     0,    76,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   953,
     953,     0,     0,   953,     0,     0,     0,    77,     0,    77,
       0,   950,     0,     0,     0,     0,   950,   950,     0,   951,
     953,     0,   951,     0,     0,     0,     0,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,    77,    77,   951,
     951,   951,    76,     0,   951,   951,     0,     0,   952,   951,
     951,   952,     0,    76,   121,    76,     0,     0,     0,   950,
       0,     0,     0,     0,     0,     0,     0,   951,   952,   952,
     952,     0,     0,   952,   952,     0,     0,   287,   952,   952,
       0,     0,     0,     0,     0,     0,   950,     0,    87,   121,
       0,   121,     0,     0,     0,     0,   952,     0,     0,     0,
       0,     0,     0,     0,   121,     0,   121,    76,    76,     0,
       0,     0,    76,    76,     0,     0,    76,     0,     0,   124,
       0,   124,     0,    87,     0,    87,     0,   291,     0,     0,
     953,     0,    76,   953,     0,     0,     0,     0,    87,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,   287,
     953,   953,   953,     0,     0,   953,   953,     0,   121,     0,
     953,   953,     0,   121,     0,     0,     0,     0,   286,   121,
     124,   124,    76,   121,     0,     0,    76,     0,   953,    76,
       0,     0,     0,     0,    76,   121,   291,     0,   951,     0,
       0,     0,    87,   951,   951,     0,    77,    87,     0,    77,
       0,     0,     0,    87,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   952,     0,    87,
       0,     0,   952,   952,   121,     0,     0,     0,   121,     0,
     121,     0,     0,     0,    76,     0,   951,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    76,   954,   954,
       0,     0,     0,     0,     0,    76,    76,     0,    87,     0,
       0,     0,    87,   951,    87,   952,     0,     0,     0,     0,
       0,    76,    77,     0,     0,     0,     0,     0,     0,   286,
       0,   955,   955,    77,     0,    77,     0,     0,     0,     0,
       0,     0,   952,     0,     0,     0,     0,     0,     0,   953,
       0,     0,     0,     0,   953,   953,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,     0,     0,   121,     0,     0,   124,
       0,     0,     0,     0,    76,     0,     0,    77,    77,     0,
       0,     0,    77,    77,   291,     0,    77,   953,     0,     0,
     286,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,    77,     0,   124,     0,   124,     0,     0,     0,
       0,     0,     0,     0,   953,     0,     0,     0,     0,   124,
       0,   124,   954,   954,     0,     0,   954,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   287,     0,
       0,     0,    77,   954,     0,     0,    77,     0,     0,    77,
       0,     0,     0,     0,    77,   955,   955,     0,    76,   955,
       0,     0,     0,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,   124,     0,     0,   955,     0,   124,     0,
       0,     0,     0,     0,   124,     0,     0,     0,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     124,     0,   776,     0,    77,     0,     0,     0,     0,     0,
       0,     0,   121,     0,   121,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,    77,    77,     0,     0,     0,
       0,     0,     0,     0,   121,     0,     0,     0,     0,   124,
       0,    77,     0,   124,     0,   124,    87,     0,    87,   287,
       0,     0,     0,   954,     0,     0,   954,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,   954,   954,   954,   776,   776,   954,   954,
       0,     0,   291,   954,   954,     0,   955,     0,     0,   955,
       0,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,   954,     0,     0,    77,     0,   955,   955,   955,     0,
       0,   955,   955,     0,     0,     0,   955,   955,     0,     0,
     287,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   955,     0,     0,     0,     0,     0,
       0,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -785,     0,     0,     0,     0,     0,     0,     0,
    -785,  -785,  -785,     0,     0,  -785,  -785,  -785,    77,  -785,
       0,     0,     0,     0,     0,     0,     0,  -785,  -785,  -785,
       0,   121,     0,     0,   121,     0,     0,     0,     0,  -785,
    -785,     0,  -785,  -785,  -785,  -785,  -785,     0,     0,     0,
       0,     0,   954,     0,     0,     0,     0,   954,   954,     0,
       0,     0,     0,     0,     0,    87,     0,     0,    87,     0,
    -785,     0,     0,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   955,     0,     0,     0,     0,
     955,   955,     0,     0,     0,     0,     0,     0,  -785,  -785,
     954,     0,     0,     0,     0,     0,     0,   121,     0,     0,
       0,     0,     0,     0,   347,     0,     0,   124,   121,   124,
     121,     0,  -785,     0,     0,     0,     0,   954,     0,     0,
       0,     0,     0,   955,     0,     0,     0,     0,     0,   124,
       0,    87,     0,     0,     0,  -785,  -785,   124,   124,     0,
     238,  -785,    87,  -785,    87,  -785,     0,     0,     0,     0,
     955,     0,     0,     0,     0,     0,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
       0,   121,     0,   361,   362,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,   121,     0,     0,
       0,     0,     0,     0,     0,     0,   776,   776,     0,     0,
       0,   776,   776,   222,   222,    87,   364,     0,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,     0,     0,
       0,    87,     0,  1121,     0,     0,     0,   121,     0,     0,
       0,   121,     0,     0,   121,   255,   258,   259,   260,     0,
       0,     0,   222,   222,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,   311,  1119,     0,     0,
       0,    87,     0,     0,     0,    87,     0,     0,    87,     0,
       0,     0,     0,   776,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,   124,     0,     0,   124,
       0,     0,   121,     0,     0,     0,     0,     0,     0,     0,
     121,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   776,     0,     0,   121,     0,     0,     0,
       0,     0,     0,     0,   407,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    87,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,     0,     0,     0,     0,     0,   121,     0,
       0,     0,     0,   124,     0,   124,     0,     0,     0,   121,
       0,     0,     0,     0,     0,     0,     0,   222,     0,     0,
     222,   222,   222,     0,   310,   407,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,    87,   222,   222,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   124,   124,     0,
       0,     0,   124,   124,     0,     0,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,     0,     0,   348,   349,   350,   351,   352,
     353,   354,   355,   121,   357,   358,     0,     0,     0,     0,
       0,     0,   361,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,     0,     0,     0,   124,    87,     0,   124,
       0,     0,     0,     0,   124,     0,     0,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,     0,     0,     0,
       0,     0,     0,     0,     0,   588,   589,   590,   591,   592,
       0,     0,   593,   594,   595,   596,   597,   598,   599,   600,
       0,   602,     0,     0,   603,   604,   605,   606,   607,   608,
     609,   610,   611,   612,   124,     0,     0,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   124,     0,     0,
       0,     0,     0,     0,     0,   124,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,   222,     0,     0,     0,   222,     0,     0,     0,   222,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,     0,   124,     0,    23,    24,    25,    26,     0,
     689,     0,     0,     0,   124,     0,     0,     0,     0,     0,
       0,    32,    33,    34,   914,     0,   222,     0,   915,   222,
       0,    41,    42,    43,    44,    45,     0,     0,     0,     0,
       0,   222,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   719,     0,
       0,     0,   917,   918,     0,     0,     0,     0,     0,     0,
     919,     0,     0,   920,     0,     0,   921,   922,     0,   923,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
      66,   222,     0,     0,     0,     0,     0,     0,   124,     0,
       0,     0,     0,   747,   925,     0,     0,   756,     0,     0,
       0,   284,     0,     0,     0,     0,     0,   222,     0,     0,
       0,     0,     0,   777,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,   222,     0,     0,
       0,     0,    41,    42,    43,    44,    45,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   833,   833,     0,
     222,   747,   756,   833,     0,   222,     0,     0,     0,     0,
       0,     0,     0,     0,   833,   833,     0,     0,     0,     0,
     222,     0,   222,     0,     0,     0,     0,     0,     0,     0,
       0,   833,     0,    58,    59,    60,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -785,     4,   284,     5,     6,     7,     8,     9,   222,     0,
       0,    10,    11,     0,     0,     0,    12,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,   222,     0,
      20,    21,    22,    23,    24,    25,    26,     0,     0,    27,
       0,   222,     0,     0,     0,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,   222,
       0,     0,     0,     0,     0,    48,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   222,
      50,    51,     0,     0,     0,     0,     0,     0,    52,     0,
       0,    53,    54,     0,    55,    56,     0,    57,     0,     0,
      58,    59,    60,    61,    62,    63,    64,    65,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -785,     0,  -785,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1019,  1020,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1026,  1027,     0,     0,
       0,     0,   222,    23,    24,    25,    26,  1038,     0,   222,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,   914,     0,     0,     0,   915,     0,   916,    41,
      42,    43,    44,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,     0,     0,     0,  1082,   833,   833,     0,
     917,   918,   833,   833,     0,     0,     0,     0,   919,     0,
       0,   920,     0,     0,   921,   922,   222,   923,   541,     0,
      58,    59,   924,    61,    62,    63,    64,    65,    66,     0,
       0,   833,   833,     0,   833,   833,     0,   222,     0,     0,
       0,     0,   925,     0,     0,     0,     0,     0,     0,   284,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -762,
     222,     0,     0,     0,   833,     0,     0,  -762,  -762,  -762,
       0,     0,  -762,  -762,  -762,     0,  -762,     0,     0,     0,
       0,     0,     0,     0,  -762,  -762,  -762,  -762,  -762,     0,
       0,     0,     0,     0,     0,     0,  -762,  -762,     0,  -762,
    -762,  -762,  -762,  -762,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   833,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,  -762,     0,     0,
       0,     0,   833,     0,     0,     0,  -762,  -762,  -762,  -762,
    -762,  -762,  -762,  -762,  -762,  -762,  -762,  -762,  -762,     0,
       0,     0,     0,  -762,  -762,  -762,  -762,     0,   841,  -762,
       0,     0,     0,     0,     0,  -762,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -762,
       0,     0,  -762,     0,     0,  -126,  -762,  -762,  -762,  -762,
    -762,  -762,  -762,  -762,  -762,  -762,  -762,  -762,     0,     0,
       0,     0,  -762,  -762,  -762,  -762,   222,  -652,  -762,  -762,
    -762,     0,  -762,     0,     0,  -652,  -652,  -652,     0,     0,
    -652,  -652,  -652,   222,  -652,     0,     0,     0,     0,   910,
       0,     0,  -652,     0,  -652,  -652,  -652,     0,     0,     0,
       0,     0,     0,     0,  -652,  -652,     0,  -652,  -652,  -652,
    -652,  -652,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,  -652,     0,     0,     0,   361,
     362,     0,     0,     0,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,     0,     0,     0,
       0,  -652,  -652,  -652,  -652,     0,  -652,  -652,     0,     0,
       0,     0,   364,  -652,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,     0,     0,     0,  -652,     0,     0,
    -652,  -273,     0,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,     0,     0,     0,     0,
       0,  -652,  -652,  -652,  -655,     0,  -652,  -652,  -652,     0,
    -652,     0,  -655,  -655,  -655,     0,     0,  -655,  -655,  -655,
       0,  -655,     0,     0,     0,     0,   888,     0,     0,  -655,
       0,  -655,  -655,  -655,     0,     0,     0,     0,     0,     0,
       0,  -655,  -655,     0,  -655,  -655,  -655,  -655,  -655,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,  -655,     0,     0,     0,   361,   362,     0,     0,
       0,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,     0,     0,     0,     0,  -655,  -655,
    -655,  -655,     0,  -655,  -655,     0,     0,     0,     0,   364,
    -655,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,     0,     0,     0,  -655,     0,     0,  -655,     0,     0,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,     0,     0,     0,     0,     0,  -655,  -655,
    -655,  -763,     0,  -655,  -655,  -655,     0,  -655,     0,  -763,
    -763,  -763,     0,     0,  -763,  -763,  -763,     0,  -763,     0,
       0,     0,     0,   900,     0,     0,  -763,  -763,  -763,  -763,
    -763,     0,     0,     0,     0,     0,     0,     0,  -763,  -763,
       0,  -763,  -763,  -763,  -763,  -763,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,  -763,
       0,     0,     0,   361,   362,     0,     0,     0,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,     0,     0,     0,     0,  -763,  -763,  -763,  -763,     0,
       0,  -763,     0,     0,     0,     0,   364,  -763,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,     0,     0,
       0,  -763,     0,     0,  -763,     0,     0,     0,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
       0,     0,     0,     0,  -763,  -763,  -763,  -763,  -764,     0,
    -763,  -763,  -763,     0,  -763,     0,  -764,  -764,  -764,     0,
       0,  -764,  -764,  -764,     0,  -764,     0,     0,     0,     0,
     910,     0,     0,  -764,  -764,  -764,  -764,  -764,     0,     0,
       0,     0,     0,     0,     0,  -764,  -764,     0,  -764,  -764,
    -764,  -764,  -764,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,  -764,     0,     0,     0,
     361,   362,     0,     0,     0,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,     0,     0,
       0,     0,  -764,  -764,  -764,  -764,     0,     0,  -764,     0,
       0,     0,     0,   364,  -764,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,     0,     0,     0,  -764,     0,
       0,  -764,     0,     0,     0,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,     0,     0,     0,
       0,  -764,  -764,  -764,  -764,  -328,     0,  -764,  -764,  -764,
       0,  -764,     0,  -328,  -328,  -328,     0,     0,  -328,  -328,
    -328,     0,  -328,     0,     0,     0,     0,     0,     0,     0,
    -328,     0,  -328,  -328,  -328,     0,     0,     0,     0,     0,
       0,     0,  -328,  -328,     0,  -328,  -328,  -328,  -328,  -328,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,  -328,     0,     0,     0,   361,   362,     0,
       0,     0,  -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,
    -328,  -328,  -328,  -328,  -328,     0,     0,     0,     0,  -328,
    -328,  -328,  -328,     0,   842,  -328,     0,     0,     0,     0,
     364,  -328,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,     0,     0,     0,  -328,     0,     0,  -328,     0,
       0,  -128,  -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,
    -328,  -328,  -328,  -328,     0,     0,     0,     0,     0,  -328,
    -328,  -328,  -465,     0,  -328,  -328,  -328,     0,  -328,     0,
    -465,  -465,  -465,     0,     0,  -465,  -465,  -465,     0,  -465,
       0,     0,     0,     0,     0,     0,     0,  -465,  -465,  -465,
    -465,     0,     0,     0,     0,     0,     0,     0,     0,  -465,
    -465,     0,  -465,  -465,  -465,  -465,  -465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,  -786,  -786,
    -465,     0,     0,     0,   361,   362,     0,     0,     0,  -465,
    -465,  -465,  -465,  -465,  -465,  -465,  -465,  -465,  -465,  -465,
    -465,  -465,     0,     0,     0,     0,  -465,  -465,  -465,  -465,
       0,     0,  -465,     0,     0,     0,     0,     0,  -465,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,     0,
       0,     0,  -465,     0,     0,     0,     0,     0,     0,  -465,
       0,  -465,  -465,  -465,  -465,  -465,  -465,  -465,  -465,  -465,
    -465,     0,     0,     0,     0,  -465,  -465,  -465,  -465,  -320,
     238,  -465,  -465,  -465,     0,  -465,     0,  -320,  -320,  -320,
       0,     0,  -320,  -320,  -320,     0,  -320,     0,     0,     0,
       0,     0,     0,     0,  -320,     0,  -320,  -320,  -320,     0,
       0,     0,     0,     0,     0,     0,  -320,  -320,     0,  -320,
    -320,  -320,  -320,  -320,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   348,  -786,  -786,  -786,  -786,   353,
     354,     0,     0,  -786,  -786,     0,     0,  -320,     0,     0,
       0,   361,   362,     0,     0,     0,  -320,  -320,  -320,  -320,
    -320,  -320,  -320,  -320,  -320,  -320,  -320,  -320,  -320,     0,
       0,     0,     0,  -320,  -320,  -320,  -320,     0,     0,  -320,
       0,     0,     0,     0,     0,  -320,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,     0,     0,     0,  -320,
       0,     0,  -320,     0,     0,     0,  -320,  -320,  -320,  -320,
    -320,  -320,  -320,  -320,  -320,  -320,  -320,  -320,     0,     0,
       0,     0,     0,  -320,  -320,  -320,  -785,     0,  -320,  -320,
    -320,     0,  -320,     0,  -785,  -785,  -785,     0,     0,  -785,
    -785,  -785,     0,  -785,     0,     0,     0,     0,     0,     0,
       0,  -785,  -785,  -785,  -785,     0,     0,     0,     0,     0,
       0,     0,     0,  -785,  -785,     0,  -785,  -785,  -785,  -785,
    -785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,   350,   351,   352,   353,   354,     0,     0,
     357,   358,     0,     0,  -785,     0,     0,     0,   361,   362,
       0,     0,     0,  -785,  -785,  -785,  -785,  -785,  -785,  -785,
    -785,  -785,  -785,  -785,  -785,  -785,     0,     0,     0,     0,
    -785,  -785,  -785,  -785,     0,     0,  -785,     0,     0,     0,
       0,     0,  -785,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,     0,     0,     0,  -785,     0,     0,     0,
       0,     0,     0,  -785,     0,  -785,  -785,  -785,  -785,  -785,
    -785,  -785,  -785,  -785,  -785,     0,     0,     0,     0,  -785,
    -785,  -785,  -785,  -335,   238,  -785,  -785,  -785,     0,  -785,
       0,  -335,  -335,  -335,     0,     0,  -335,  -335,  -335,     0,
    -335,     0,     0,     0,     0,     0,     0,     0,  -335,     0,
    -335,  -335,     0,     0,     0,     0,     0,     0,     0,     0,
    -335,  -335,     0,  -335,  -335,  -335,  -335,  -335,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -335,     0,     0,     0,     0,     0,     0,     0,     0,
    -335,  -335,  -335,  -335,  -335,  -335,  -335,  -335,  -335,  -335,
    -335,  -335,  -335,     0,     0,     0,     0,  -335,  -335,  -335,
    -335,     0,     0,  -335,     0,     0,     0,     0,     0,  -335,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -335,     0,     0,     0,     0,     0,     0,
    -335,     0,  -335,  -335,  -335,  -335,  -335,  -335,  -335,  -335,
    -335,  -335,     0,     0,     0,     0,     0,  -335,  -335,  -335,
    -762,   235,  -335,  -335,  -335,     0,  -335,     0,  -762,  -762,
    -762,     0,     0,     0,  -762,  -762,     0,  -762,     0,     0,
       0,     0,     0,     0,     0,  -762,  -762,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -762,  -762,     0,
    -762,  -762,  -762,  -762,  -762,     0,     0,     0,     0,     0,
       0,     0,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,     0,     0,     0,  -762,   361,
     362,     0,     0,     0,     0,     0,     0,  -762,  -762,  -762,
    -762,  -762,  -762,  -762,  -762,  -762,  -762,  -762,  -762,  -762,
       0,     0,     0,     0,  -762,  -762,  -762,  -762,     0,   786,
    -762,     0,   364,     0,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,     0,     0,     0,     0,     0,     0,
    -762,     0,     0,     0,     0,     0,  -126,  -762,   242,  -762,
    -762,  -762,  -762,  -762,  -762,  -762,  -762,  -762,  -762,     0,
       0,     0,     0,  -762,  -762,  -762,  -117,  -762,     0,  -762,
       0,  -762,     0,  -762,     0,  -762,  -762,  -762,     0,     0,
       0,  -762,  -762,     0,  -762,     0,     0,     0,     0,     0,
       0,     0,  -762,  -762,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -762,  -762,     0,  -762,  -762,  -762,
    -762,  -762,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -762,     0,     0,     0,     0,
       0,     0,     0,     0,  -762,  -762,  -762,  -762,  -762,  -762,
    -762,  -762,  -762,  -762,  -762,  -762,  -762,     0,     0,     0,
       0,  -762,  -762,  -762,  -762,     0,   786,  -762,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -762,     0,     0,
       0,     0,     0,  -126,  -762,     0,  -762,  -762,  -762,  -762,
    -762,  -762,  -762,  -762,  -762,  -762,     0,     0,     0,     0,
    -762,  -762,  -762,  -762,  -328,     0,  -762,     0,  -762,     0,
    -762,     0,  -328,  -328,  -328,     0,     0,     0,  -328,  -328,
       0,  -328,     0,     0,     0,     0,     0,     0,     0,  -328,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -328,  -328,     0,  -328,  -328,  -328,  -328,  -328,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -328,     0,     0,     0,     0,     0,     0,     0,
       0,  -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,
    -328,  -328,  -328,  -328,     0,     0,     0,     0,  -328,  -328,
    -328,  -328,     0,   787,  -328,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -328,     0,     0,     0,     0,     0,
    -128,  -328,     0,  -328,  -328,  -328,  -328,  -328,  -328,  -328,
    -328,  -328,  -328,     0,     0,     0,     0,     0,  -328,  -328,
    -119,  -328,     0,  -328,     0,  -328,     0,  -328,     0,  -328,
    -328,  -328,     0,     0,     0,  -328,  -328,     0,  -328,     0,
       0,     0,     0,     0,     0,     0,  -328,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -328,  -328,
       0,  -328,  -328,  -328,  -328,  -328,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -328,
       0,     0,     0,     0,     0,     0,     0,     0,  -328,  -328,
    -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,
    -328,     0,     0,     0,     0,  -328,  -328,  -328,  -328,     0,
     787,  -328,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -328,     0,     0,     0,     0,     0,  -128,  -328,     0,
    -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,  -328,
       0,     0,     0,     0,     0,  -328,  -328,  -328,     0,     0,
    -328,     0,  -328,   262,  -328,     5,     6,     7,     8,     9,
    -785,  -785,  -785,    10,    11,     0,     0,  -785,    12,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,    28,    29,   263,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      52,     0,     0,    53,    54,     0,    55,    56,     0,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
      66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -785,   262,  -785,     5,     6,     7,     8,
       9,     0,     0,  -785,    10,    11,     0,  -785,  -785,    12,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    20,    21,    22,    23,    24,    25,    26,
       0,     0,    27,     0,     0,     0,     0,     0,    28,    29,
     263,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,     0,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    52,     0,     0,    53,    54,     0,    55,    56,     0,
      57,     0,     0,    58,    59,    60,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -785,   262,  -785,     5,     6,     7,
       8,     9,     0,     0,  -785,    10,    11,     0,     0,  -785,
      12,  -785,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,     0,     0,     0,     0,     0,    28,
      29,   263,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     0,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    52,     0,     0,    53,    54,     0,    55,    56,
       0,    57,     0,     0,    58,    59,    60,    61,    62,    63,
      64,    65,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -785,   262,  -785,     5,     6,
       7,     8,     9,     0,     0,  -785,    10,    11,     0,     0,
    -785,    12,     0,    13,    14,    15,    16,    17,    18,    19,
    -785,     0,     0,     0,     0,    20,    21,    22,    23,    24,
      25,    26,     0,     0,    27,     0,     0,     0,     0,     0,
      28,    29,   263,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     0,    41,    42,    43,    44,    45,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    52,     0,     0,    53,    54,     0,    55,
      56,     0,    57,     0,     0,    58,    59,    60,    61,    62,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -785,   262,  -785,     5,
       6,     7,     8,     9,     0,     0,  -785,    10,    11,     0,
       0,  -785,    12,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    20,    21,    22,    23,
      24,    25,    26,     0,     0,    27,     0,     0,     0,     0,
       0,    28,    29,   263,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,     0,    41,    42,    43,    44,    45,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,     0,     0,    52,     0,     0,    53,    54,     0,
      55,    56,     0,    57,     0,     0,    58,    59,    60,    61,
      62,    63,    64,    65,    66,     0,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     5,     6,     7,     8,     9,
       0,  -785,  -785,    10,    11,    67,    68,    69,    12,     0,
      13,    14,    15,    16,    17,    18,    19,  -785,     0,  -785,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,    28,    29,   263,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      52,     0,     0,    53,    54,     0,    55,    56,     0,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
      66,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,    67,    68,    69,    12,     0,    13,    14,    15,    16,
      17,    18,    19,  -785,     0,  -785,     0,     0,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,     0,     0,
       0,     0,     0,    28,    29,   263,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,     0,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    52,     0,     0,   264,
      54,     0,    55,    56,     0,    57,     0,     0,    58,    59,
      60,    61,    62,    63,    64,    65,    66,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
       0,     0,     0,     0,     0,     0,     0,  -785,     0,  -785,
     262,  -785,     5,     6,     7,     8,     9,     0,     0,     0,
      10,    11,     0,     0,     0,    12,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    20,
      21,    22,    23,    24,    25,    26,     0,     0,    27,     0,
       0,     0,     0,     0,    28,    29,   263,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     0,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    52,     0,     0,
      53,    54,     0,    55,    56,     0,    57,     0,     0,    58,
      59,    60,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,     0,     0,     0,     0,     0,     0,     0,  -785,     0,
    -785,     4,  -785,     5,     6,     7,     8,     9,     0,     0,
       0,    10,    11,     0,     0,     0,    12,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      20,    21,    22,    23,    24,    25,    26,     0,     0,    27,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    52,     0,
       0,    53,    54,     0,    55,    56,     0,    57,     0,     0,
      58,    59,    60,    61,    62,    63,    64,    65,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,     0,     0,  -785,     0,     0,     0,     0,     0,
       0,  -785,   262,  -785,     5,     6,     7,     8,     9,     0,
       0,     0,    10,    11,     0,     0,     0,    12,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,     0,     0,     0,     0,     0,    28,    29,   263,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,     0,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    52,
       0,     0,    53,    54,     0,    55,    56,     0,    57,     0,
       0,    58,    59,    60,    61,    62,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,     0,     0,  -785,     0,     0,     0,     0,
       0,     0,  -785,   262,  -785,     5,     6,     7,     8,     9,
       0,     0,  -785,    10,    11,     0,     0,     0,    12,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,    28,    29,   263,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      52,     0,     0,    53,    54,     0,    55,    56,     0,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
      66,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,    67,    68,    69,    12,     0,    13,    14,    15,    16,
      17,    18,    19,  -785,     0,  -785,     0,     0,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,     0,     0,
       0,     0,     0,    28,    29,   263,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,     0,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    52,     0,     0,    53,
      54,     0,    55,    56,     0,    57,     0,     0,    58,    59,
      60,    61,    62,    63,    64,    65,    66,     0,  -785,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       0,     9,     0,     0,     0,    10,    11,    67,    68,    69,
      12,     0,    13,    14,    15,    16,    17,    18,    19,  -785,
       0,  -785,     0,     0,    20,    21,    22,    23,    24,    25,
      26,     0,     0,   209,     0,     0,     0,     0,     0,     0,
      29,     0,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,   210,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
       0,     0,   211,     0,     0,   212,    54,     0,    55,    56,
       0,   213,   214,   215,    58,    59,   216,    61,    62,    63,
      64,    65,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     6,     7,     0,     9,     0,     0,
       0,    10,    11,    67,   217,    69,    12,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,   242,     0,     0,
      20,    21,    22,    23,    24,    25,    26,     0,     0,    27,
       0,     0,     0,     0,     0,     0,    29,     0,     0,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,     0,     0,     0,     0,     0,   211,     0,
       0,   212,    54,     0,    55,    56,     0,     0,     0,     0,
      58,    59,    60,    61,    62,    63,    64,    65,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     0,     9,     0,     0,     0,    10,    11,    67,
      68,    69,    12,     0,    13,    14,    15,    16,    17,    18,
      19,   312,     0,   313,     0,     0,    20,    21,    22,    23,
      24,    25,    26,     0,     0,    27,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    40,     0,    41,    42,    43,    44,    45,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,     0,     0,   211,     0,     0,   212,    54,     0,
      55,    56,     0,     0,     0,     0,    58,    59,    60,    61,
      62,    63,    64,    65,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     8,     9,
       0,     0,     0,    10,    11,    67,    68,    69,    12,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,   242,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      52,     0,     0,    53,    54,     0,    55,    56,     0,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
      66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,    67,    68,    69,    12,     0,    13,    14,    15,    16,
      17,    18,    19,   522,     0,     0,     0,     0,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,     0,     0,
       0,     0,     0,    28,    29,   263,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,     0,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    52,     0,     0,    53,
      54,     0,    55,    56,     0,    57,     0,     0,    58,    59,
      60,    61,    62,    63,    64,    65,    66,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   522,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,     0,     0,     0,   153,   154,   155,
     411,   412,   413,   414,   160,   161,   162,     0,     0,     0,
       0,     0,   163,   164,   165,   166,   415,   416,   417,   418,
     171,    37,    38,   419,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,   182,   183,     0,     0,     0,     0,   184,
     185,   186,   187,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   188,   189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,     0,   200,   201,     0,
       0,     0,     0,     0,   202,   420,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
       0,     0,     0,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,     0,     0,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   295,   296,   172,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,   182,
     183,     0,     0,     0,     0,   184,   185,   186,   187,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     188,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,     0,   200,   201,     0,     0,     0,     0,     0,
     202,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,     0,     0,     0,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,     0,     0,
       0,     0,     0,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   244,     0,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,     0,   182,   183,     0,     0,     0,     0,
     184,   185,   186,   187,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   188,   189,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,     0,   200,   201,
       0,     0,     0,     0,     0,   202,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
       0,     0,     0,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,     0,     0,   163,   164,
     165,   166,   167,   168,   169,   170,   171,     0,     0,   172,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,   182,
     183,     0,     0,     0,     0,   184,   185,   186,   187,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     188,   189,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,     0,   200,   201,     0,     0,     0,     0,     0,
     202,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,     0,     0,     0,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,     0,     0,
       0,     0,     0,   163,   164,   165,   166,   167,   168,   169,
     170,   171,     0,     0,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,     0,   182,   183,     0,     0,     0,     0,
     184,   185,   186,   187,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   188,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,     0,   200,   201,
       5,     6,     7,     0,     9,   202,     0,     0,    10,    11,
       0,     0,     0,    12,     0,    13,    14,    15,   250,   251,
      18,    19,     0,     0,     0,     0,     0,    20,   252,   253,
      23,    24,    25,    26,     0,     0,   209,     0,     0,     0,
       0,     0,     0,   282,     0,     0,    32,    33,    34,    35,
      36,    37,    38,    39,    40,     0,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   283,     0,     0,   212,    54,
       0,    55,    56,     0,     0,     0,     0,    58,    59,    60,
      61,    62,    63,    64,    65,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     0,     9,     0,     0,   284,    10,    11,     0,
       0,     0,    12,   285,    13,    14,    15,   250,   251,    18,
      19,     0,     0,     0,     0,     0,    20,   252,   253,    23,
      24,    25,    26,     0,     0,   209,     0,     0,     0,     0,
       0,     0,   282,     0,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    40,     0,    41,    42,    43,    44,    45,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   283,     0,     0,   212,    54,     0,
      55,    56,     0,     0,     0,     0,    58,    59,    60,    61,
      62,    63,    64,    65,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   284,    10,    11,     0,     0,
       0,    12,   583,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    20,    21,    22,    23,    24,
      25,    26,     0,     0,    27,     0,     0,     0,     0,     0,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     0,    41,    42,    43,    44,    45,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    52,     0,     0,    53,    54,     0,    55,
      56,     0,    57,     0,     0,    58,    59,    60,    61,    62,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,    67,    68,    69,    12,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
     209,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,   210,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,     0,     0,   211,
       0,     0,   212,    54,     0,    55,    56,     0,   213,   214,
     215,    58,    59,   216,    61,    62,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,     0,    10,    11,
      67,   217,    69,    12,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    20,    21,    22,
      23,    24,    25,    26,     0,     0,    27,     0,     0,     0,
       0,     0,    28,    29,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,     0,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    52,     0,     0,    53,    54,
       0,    55,    56,     0,    57,     0,     0,    58,    59,    60,
      61,    62,    63,    64,    65,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    67,    68,    69,    12,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    20,    21,    22,    23,    24,    25,    26,
       0,     0,   209,     0,     0,     0,     0,     0,     0,    29,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,   210,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,   458,     0,     0,     0,     0,     0,
       0,   211,     0,     0,   212,    54,     0,    55,    56,     0,
     213,   214,   215,    58,    59,   216,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,    67,   217,    69,    12,     0,    13,    14,    15,
     250,   251,    18,    19,     0,     0,     0,     0,     0,    20,
     252,   253,    23,    24,    25,    26,     0,     0,   209,     0,
       0,     0,     0,     0,     0,    29,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,   210,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,     0,     0,   211,     0,     0,
     212,    54,     0,    55,    56,     0,   668,   214,   215,    58,
      59,   216,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,    67,   217,
      69,    12,     0,    13,    14,    15,   250,   251,    18,    19,
       0,     0,     0,     0,     0,    20,   252,   253,    23,    24,
      25,    26,     0,     0,   209,     0,     0,     0,     0,     0,
       0,    29,     0,     0,    32,    33,    34,    35,    36,    37,
      38,    39,    40,   210,    41,    42,    43,    44,    45,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,   458,     0,     0,     0,
       0,     0,     0,   211,     0,     0,   212,    54,     0,    55,
      56,     0,   668,   214,   215,    58,    59,   216,    61,    62,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,    67,   217,    69,    12,     0,    13,
      14,    15,   250,   251,    18,    19,     0,     0,     0,     0,
       0,    20,   252,   253,    23,    24,    25,    26,     0,     0,
     209,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,   210,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,     0,     0,   211,
       0,     0,   212,    54,     0,    55,    56,     0,   213,   214,
       0,    58,    59,   216,    61,    62,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
      67,   217,    69,    12,     0,    13,    14,    15,   250,   251,
      18,    19,     0,     0,     0,     0,     0,    20,   252,   253,
      23,    24,    25,    26,     0,     0,   209,     0,     0,     0,
       0,     0,     0,    29,     0,     0,    32,    33,    34,    35,
      36,    37,    38,    39,    40,   210,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,     0,     0,   211,     0,     0,   212,    54,
       0,    55,    56,     0,     0,   214,   215,    58,    59,   216,
      61,    62,    63,    64,    65,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    67,   217,    69,    12,
       0,    13,    14,    15,   250,   251,    18,    19,     0,     0,
       0,     0,     0,    20,   252,   253,    23,    24,    25,    26,
       0,     0,   209,     0,     0,     0,     0,     0,     0,    29,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,   210,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,     0,
       0,   211,     0,     0,   212,    54,     0,    55,    56,     0,
     668,   214,     0,    58,    59,   216,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,    67,   217,    69,    12,     0,    13,    14,    15,
     250,   251,    18,    19,     0,     0,     0,     0,     0,    20,
     252,   253,    23,    24,    25,    26,     0,     0,   209,     0,
       0,     0,     0,     0,     0,    29,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,   210,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,     0,     0,   211,     0,     0,
     212,    54,     0,    55,    56,     0,     0,   214,     0,    58,
      59,   216,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,    67,   217,
      69,    12,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    20,    21,    22,    23,    24,
      25,    26,     0,     0,   209,     0,     0,     0,     0,     0,
       0,    29,     0,     0,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     0,    41,    42,    43,    44,    45,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,     0,     0,   211,     0,     0,   212,    54,     0,    55,
      56,     0,   764,     0,     0,    58,    59,    60,    61,    62,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,    67,   217,    69,    12,     0,    13,
      14,    15,   250,   251,    18,    19,     0,     0,     0,     0,
       0,    20,   252,   253,    23,    24,    25,    26,     0,     0,
     209,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,     0,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,     0,     0,   211,
       0,     0,   212,    54,     0,    55,    56,     0,   764,     0,
       0,    58,    59,    60,    61,    62,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
      67,   217,    69,    12,     0,    13,    14,    15,   250,   251,
      18,    19,     0,     0,     0,     0,     0,    20,   252,   253,
      23,    24,    25,    26,     0,     0,   209,     0,     0,     0,
       0,     0,     0,    29,     0,     0,    32,    33,    34,    35,
      36,    37,    38,    39,    40,     0,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,     0,     0,   211,     0,     0,   212,    54,
       0,    55,    56,     0,   979,     0,     0,    58,    59,    60,
      61,    62,    63,    64,    65,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    67,   217,    69,    12,
       0,    13,    14,    15,   250,   251,    18,    19,     0,     0,
       0,     0,     0,    20,   252,   253,    23,    24,    25,    26,
       0,     0,   209,     0,     0,     0,     0,     0,     0,    29,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,     0,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,     0,
       0,   211,     0,     0,   212,    54,     0,    55,    56,     0,
    1033,     0,     0,    58,    59,    60,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,    67,   217,    69,    12,     0,    13,    14,    15,
     250,   251,    18,    19,     0,     0,     0,     0,     0,    20,
     252,   253,    23,    24,    25,    26,     0,     0,   209,     0,
       0,     0,     0,     0,     0,    29,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     0,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,     0,     0,   211,     0,     0,
     212,    54,     0,    55,    56,     0,  1179,     0,     0,    58,
      59,    60,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,    67,   217,
      69,    12,     0,    13,    14,    15,   250,   251,    18,    19,
       0,     0,     0,     0,     0,    20,   252,   253,    23,    24,
      25,    26,     0,     0,   209,     0,     0,     0,     0,     0,
       0,    29,     0,     0,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     0,    41,    42,    43,    44,    45,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,     0,     0,   211,     0,     0,   212,    54,     0,    55,
      56,     0,     0,     0,     0,    58,    59,    60,    61,    62,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,    67,   217,    69,    12,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
     209,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,     0,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,     0,     0,   211,
       0,     0,   212,    54,     0,    55,    56,     0,     0,     0,
       0,    58,    59,    60,    61,    62,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
      67,   217,    69,    12,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    20,    21,    22,
      23,    24,    25,    26,     0,     0,    27,     0,     0,     0,
       0,     0,     0,    29,     0,     0,    32,    33,    34,    35,
      36,    37,    38,    39,    40,     0,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,     0,     0,   211,     0,     0,   212,    54,
       0,    55,    56,     0,     0,     0,     0,    58,    59,    60,
      61,    62,    63,    64,    65,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    67,    68,    69,    12,
       0,    13,    14,    15,   250,   251,    18,    19,     0,     0,
       0,     0,     0,    20,   252,   253,    23,    24,    25,    26,
       0,     0,   209,     0,     0,     0,     0,     0,     0,   282,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,     0,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   283,     0,     0,   343,    54,     0,    55,    56,     0,
     344,     0,     0,    58,    59,    60,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,     0,     0,
       0,    12,   284,    13,    14,    15,   250,   251,    18,    19,
       0,     0,     0,     0,     0,    20,   252,   253,    23,    24,
      25,    26,     0,     0,   209,     0,     0,     0,     0,     0,
       0,   282,     0,     0,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     0,    41,    42,    43,    44,    45,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   392,     0,     0,    53,    54,     0,    55,
      56,     0,    57,     0,     0,    58,    59,    60,    61,    62,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
       0,     0,     0,    12,   284,    13,    14,    15,   250,   251,
      18,    19,     0,     0,     0,     0,     0,    20,   252,   253,
      23,    24,    25,    26,     0,     0,   209,     0,     0,     0,
       0,     0,     0,   282,     0,     0,    32,    33,    34,   400,
      36,    37,    38,   401,    40,     0,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,   403,     0,     0,   212,    54,
       0,    55,    56,     0,     0,     0,     0,    58,    59,    60,
      61,    62,    63,    64,    65,    66,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,     0,     0,     0,    12,   284,    13,    14,    15,
     250,   251,    18,    19,     0,     0,     0,     0,     0,    20,
     252,   253,    23,    24,    25,    26,     0,     0,   209,     0,
       0,     0,     0,     0,     0,   282,     0,     0,    32,    33,
      34,   400,    36,    37,    38,   401,    40,     0,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
     212,    54,     0,    55,    56,     0,     0,     0,     0,    58,
      59,    60,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,     0,     0,     0,    12,   284,    13,
      14,    15,   250,   251,    18,    19,     0,     0,     0,     0,
       0,    20,   252,   253,    23,    24,    25,    26,     0,     0,
     209,     0,     0,     0,     0,     0,     0,   282,     0,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,     0,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   283,
       0,     0,   343,    54,     0,    55,    56,     0,     0,     0,
       0,    58,    59,    60,    61,    62,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,     0,     0,     0,    12,
     284,    13,    14,    15,   250,   251,    18,    19,     0,     0,
       0,     0,     0,    20,   252,   253,    23,    24,    25,    26,
       0,     0,   209,     0,     0,     0,     0,     0,     0,   282,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,     0,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1118,     0,     0,   212,    54,     0,    55,    56,     0,
       0,     0,     0,    58,    59,    60,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,     0,     0,
       0,    12,   284,    13,    14,    15,   250,   251,    18,    19,
       0,     0,     0,     0,     0,    20,   252,   253,    23,    24,
      25,    26,     0,     0,   209,     0,     0,     0,     0,     0,
       0,   282,     0,     0,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     0,    41,    42,    43,    44,    45,    46,
      47,    23,    24,    25,    26,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
     914,     0,     0,     0,   915,     0,     0,    41,    42,    43,
      44,    45,     0,  1207,     0,     0,   212,    54,     0,    55,
      56,     0,     0,     0,     0,    58,    59,    60,    61,    62,
      63,    64,    65,    66,     0,     0,     0,     0,   917,   918,
       0,     0,     0,     0,     0,     0,   919,     0,     0,   920,
       0,     0,   921,   922,   284,  1145,     0,     0,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    23,    24,    25,
      26,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     925,     0,     0,    32,    33,    34,   914,   284,     0,     0,
     915,     0,     0,    41,    42,    43,    44,    45,     0,     0,
      23,    24,    25,    26,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,   914,
       0,     0,     0,   915,   917,   918,    41,    42,    43,    44,
      45,     0,   919,     0,     0,   920,     0,     0,   921,   922,
       0,   923,     0,     0,    58,    59,    60,    61,    62,    63,
      64,    65,    66,     0,     0,     0,     0,   917,   918,     0,
       0,     0,     0,     0,     0,   919,   925,     0,   920,     0,
       0,   921,   922,   284,     0,     0,     0,    58,    59,    60,
      61,    62,    63,    64,    65,    66,   613,   614,     0,     0,
     615,     0,     0,     0,     0,     0,     0,     0,     0,   925,
       0,     0,     0,     0,     0,     0,   284,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   622,   623,     0,     0,
     624,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   672,   614,     0,     0,
     673,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   675,   623,     0,     0,
     676,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   702,   614,     0,     0,
     703,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   705,   623,     0,     0,
     706,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   814,   614,     0,     0,
     815,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   817,   623,     0,     0,
     818,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   823,   614,     0,     0,
     824,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   657,   623,     0,     0,
     658,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,  1039,   614,     0,     0,
    1040,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,  1042,   623,     0,     0,
    1043,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,  1246,   614,     0,     0,
    1247,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,  1249,   623,     0,     0,
    1250,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,  1283,   614,     0,     0,
    1284,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   657,   623,     0,     0,
     658,   202,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,     0,     0,     0,     0,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,     0,     0,     0,     0,
       0,   202
};

static const yytype_int16 yycheck[] =
{
       2,     8,    60,   380,    59,    91,   105,    16,    17,   385,
      71,   324,     2,    28,     4,   328,   324,     8,   104,   331,
     328,    28,    96,   328,   556,   802,   539,   736,    22,    53,
      22,   403,   238,    16,    17,   733,    97,    28,    57,   232,
     444,   302,   303,    85,   740,   276,   101,   809,   452,   280,
     452,     4,    59,    55,    56,   379,   784,   381,    68,    16,
      17,    93,    94,    53,    54,    97,    85,    57,   783,   784,
     382,   448,    55,   736,    76,    77,     2,    97,     4,    98,
      99,   100,    94,   871,   733,    97,    80,    53,    80,    57,
     492,   740,   921,   102,   101,    85,   502,    67,    55,    56,
      77,   869,    71,   586,   428,  1068,    77,    29,    98,    99,
     100,   101,    52,    16,    17,    27,    25,   660,   661,   102,
     783,   784,    66,   447,  1184,   449,  1211,   504,    54,    66,
      25,    13,  1028,    68,    66,    97,   100,    66,    68,   264,
      68,   227,    34,   917,   918,   102,    25,    66,    26,   789,
      27,    99,    29,   477,    25,    25,   144,   797,   216,   586,
      52,    96,    97,    25,    13,   153,    96,    97,     0,    97,
     256,    52,    25,    54,    55,   119,    57,   583,   126,   153,
     561,   505,   563,   145,   121,   122,   148,   119,   212,   153,
      25,   143,   121,   122,  1279,    13,    37,    38,   146,   102,
     119,   123,   121,   122,   290,   266,   146,   720,   721,   241,
     145,   243,    13,   148,  1274,   152,   218,   219,   343,    13,
     101,    26,   212,   152,   226,   249,   235,    13,   237,   238,
     232,   146,  1128,   660,   661,   143,   238,   146,   153,   151,
     264,   153,   219,   285,    13,   247,   266,   218,   219,  1078,
    1079,   146,   235,   148,   237,   238,   517,   247,   519,   249,
    1223,   253,   254,   640,   146,   143,   285,   146,   751,   151,
     463,   153,   262,   650,   264,   146,   146,   649,   235,   683,
     237,   685,    25,   685,   146,   317,   318,   319,   320,   740,
     322,   323,   743,   146,   727,   285,    13,   266,   264,  1073,
    1074,  1051,   151,   736,   153,   317,   318,   319,   320,   395,
     287,   146,   153,  1009,   284,   100,  1014,   641,   109,   343,
    1070,   247,    25,    25,  1112,   344,   316,   651,   389,    56,
     148,   321,   235,   151,   237,   153,   262,   327,   143,   341,
     652,   331,   133,   148,   346,  1113,    13,   148,   152,   881,
     151,    13,   153,   343,   344,   387,   869,   151,   871,   153,
    1009,   146,   386,   316,   388,   151,   772,   153,  1096,   389,
     402,   380,    68,    99,   381,   387,   344,   343,   380,  1028,
      13,  1096,   151,   126,   153,    68,   100,   126,   792,   381,
     316,   152,   382,  1056,   707,   321,   386,    68,   388,   707,
     126,    97,   707,   146,  1181,   282,   149,   146,  1071,    68,
     153,   152,    68,    96,    97,  1078,  1079,   960,   961,  1181,
     733,   428,   965,   966,   126,    96,    97,   740,    25,   431,
     432,    52,   146,  1096,   151,    56,   153,    68,    97,   448,
     442,    97,   449,   146,   146,   148,   448,   149,   450,   451,
     711,   153,    99,    68,   152,   716,   471,   449,    25,   461,
     502,   463,   145,    77,   471,    96,    97,  1217,   699,    28,
     477,   126,  1222,  1223,   145,    68,   478,   683,   671,   126,
     471,    96,    97,   502,   151,   477,   153,   444,    68,   151,
     126,   153,    68,   148,   484,   504,   877,   878,   505,   146,
    1209,   882,   504,   884,    97,   886,  1204,    52,   569,    34,
     822,    56,   502,   505,   145,  1265,  1293,    97,   151,    66,
     153,    97,   136,   137,   138,   783,   784,    52,  1028,   126,
     145,   126,   489,   960,   961,   912,    66,    67,   965,   966,
     853,   583,  1292,   545,  1087,   853,  1209,   919,   853,   146,
     146,   148,   149,    99,   556,  1204,   153,   566,  1009,   126,
    1011,   570,  1211,    99,   583,  1016,   568,    66,   642,   151,
     580,   153,   119,   634,   121,   122,   586,  1028,    68,   146,
     126,    66,   149,   112,    68,    68,   153,   911,  1286,   913,
     126,   121,   122,   583,  1126,  1028,  1000,   580,  1000,  1112,
    1113,    15,  1134,   586,  1217,   981,   630,    97,   632,  1222,
    1037,    25,    96,    97,    97,   150,    68,    99,   153,   621,
     119,   653,   121,   122,   618,   627,   618,    77,    82,    83,
    1279,   640,   146,   627,   641,   627,   121,   122,   640,   124,
     630,   650,   632,    99,   651,    97,  1022,   662,   650,   641,
     660,   661,  1265,   148,   847,   662,   688,   659,   700,   651,
    1087,   145,   652,   921,    68,   659,    56,   659,     9,   671,
     126,   662,    54,   985,    15,   129,   130,    68,   126,    68,
     938,   700,    64,    65,   134,   135,   136,   137,   138,    66,
      68,   693,    96,    97,    99,   781,  1009,    58,  1136,  1137,
     692,   693,   100,  1075,  1204,    96,    97,    96,    97,  1209,
     700,  1211,    15,  1213,    17,  1028,    77,  1093,    96,    97,
    1032,   126,   148,   241,    56,   602,   683,   153,  1260,    68,
     772,   830,  1028,  1114,  1115,  1116,  1117,    68,   146,  1190,
      99,   145,   119,   620,   121,   122,    99,   108,   109,   751,
     752,   269,    66,   772,   145,   273,   145,    96,    97,   820,
    1211,   146,  1213,   146,   148,    96,    97,   145,   751,   148,
     153,  1204,   133,   126,   829,    99,  1209,    99,  1211,  1279,
    1213,  1281,   772,   149,   786,   787,  1286,   153,  1288,   821,
    1048,   793,   794,   783,   784,   819,  1108,   674,   800,   148,
     802,  1177,   126,  1303,   126,   119,   145,   121,   122,   821,
    1068,    37,    38,    77,   145,    14,    15,  1255,  1256,  1196,
    1078,  1079,   829,  1261,   146,  1263,  1264,   704,  1279,   819,
    1281,   149,   822,   126,    68,   792,   144,  1288,  1096,   841,
     842,    99,   844,   845,   146,   847,  1279,    68,  1281,   150,
      99,   153,  1303,  1286,    56,  1288,    68,  1295,  1296,  1297,
    1298,   148,    96,    97,  1245,   146,   924,    99,   126,  1307,
    1303,   895,    77,  1197,   126,    96,    97,   126,   210,   881,
     126,   213,   214,   215,    96,    97,    99,   126,   146,   891,
     106,  1204,    99,    70,   126,   146,    99,   146,  1211,   901,
      56,   903,    99,   912,   151,   895,   913,   909,  1204,    25,
     912,   145,   130,   126,   146,  1211,   906,  1213,   908,   126,
       2,   913,     4,   126,   145,   986,    99,   917,   918,   126,
     126,   921,   126,   145,    16,    17,   454,  1056,   970,   816,
     149,   459,   146,   146,   462,   969,   146,   465,   938,   826,
     960,   961,   144,   126,    58,   965,   966,   144,   970,  1078,
    1079,   146,   964,   481,    88,    89,  1279,   146,   486,   126,
      66,    53,    54,    77,   146,    57,   146,    26,   146,   969,
     906,   146,   908,  1279,    52,  1281,    68,   146,    54,    55,
    1286,    57,  1288,    52,   996,   985,    66,    10,    64,    65,
       8,  1025,  1021,    85,   108,  1029,   144,  1303,   112,    40,
      41,    93,    94,   286,   287,    97,    98,    99,   100,    68,
     102,   146,    13,   119,   146,   121,   122,  1037,   124,   547,
    1068,  1021,    25,  1065,  1120,  1025,   146,    17,   152,  1029,
    1078,  1079,  1032,   152,   921,   922,   146,    96,    97,   119,
     126,   121,   122,  1065,   124,    44,    44,   144,  1048,   126,
     146,  1051,   146,   128,   582,    44,    26,    52,    44,    54,
      55,    56,    57,   150,    15,   146,  1100,  1087,  1068,  1069,
    1070,    52,   146,  1073,  1074,  1109,   146,   131,  1078,  1079,
     146,   146,  1150,   100,   143,   146,   145,    52,   131,   148,
    1124,    40,    41,    42,    43,    44,  1096,    26,    68,   146,
    1100,   443,   444,  1132,   674,    52,   101,   146,  1108,  1109,
     452,   146,   107,  1125,  1126,  1002,    59,    60,    61,    62,
     212,   146,  1134,   100,  1124,   146,    96,    97,    56,   144,
     146,   151,  1132,    52,   704,    54,    55,    56,    57,    68,
     149,  1028,  1176,   235,   146,   237,   238,   489,     9,   241,
     492,   243,   131,  1187,  1041,   247,    52,   249,    54,    55,
      56,    57,   146,  1050,   146,   131,  1053,    96,    97,  1181,
     262,   146,   264,   143,  1203,   145,  1176,  1196,   148,   146,
    1197,   149,   101,   146,  1196,   146,  1198,  1187,   146,  1076,
    1232,  1233,   146,   285,   144,  1197,   724,    52,    56,    54,
      55,    56,    57,  1203,   146,   120,     2,   146,     4,   146,
    1232,  1233,   554,   146,   143,   146,   145,  1217,  1230,   148,
      16,    17,  1222,  1223,   316,   317,   318,   319,   320,   321,
     322,   323,   146,   146,   247,   327,   148,  1271,   580,   331,
     148,  1128,   480,   484,   586,   829,   816,    98,  1260,    89,
     100,   343,   344,   659,   752,  1230,   826,    53,    54,   720,
     869,  1130,  1028,   346,  1151,  1265,   903,  1274,   831,   574,
    1293,  1271,    68,  1071,    52,   339,    54,    55,    56,    57,
      58,  1293,  1069,  1071,  1171,  1172,  1173,  1067,   380,  1198,
     382,  1125,  1292,   108,   386,   387,   388,    93,    94,    77,
     828,    97,   101,   519,   736,  1209,   102,  1204,   733,    -1,
     402,  1151,    -1,    -1,    -1,    -1,    -1,    -1,   846,    -1,
     848,   663,    -1,   101,    -1,    -1,   668,    77,    -1,   107,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   867,
      -1,   683,    -1,   685,    94,    95,    -1,    -1,   431,   432,
      -1,   921,  1239,    -1,    -1,   133,   448,    -1,   136,   442,
      -1,  1248,    -1,    -1,   660,   661,    -1,   450,   451,    -1,
      -1,    -1,   571,    -1,    -1,   153,    -1,   576,    -1,   578,
      -1,   677,   678,   133,   134,   135,   136,   137,   138,   731,
      -1,    -1,   484,    -1,    -1,   478,    -1,    -1,   694,    -1,
      52,    -1,    54,    55,    56,    57,    58,    -1,    -1,   751,
     502,    -1,   504,    -1,    -1,    -1,   212,    -1,    -1,    -1,
      -1,    -1,   764,    -1,    -1,    77,    -1,    -1,    -1,   628,
      -1,    -1,    -1,    -1,   633,    -1,   635,    -1,    -1,   235,
      -1,   237,   238,    77,    -1,   241,    -1,   243,    -1,   101,
     792,   247,    -1,   249,    -1,   107,   108,   109,    77,    -1,
      94,    95,    -1,    -1,    -1,    -1,   262,    -1,   264,    -1,
     812,  1041,    -1,    -1,   566,    94,    95,    -1,   570,    -1,
    1050,   133,    -1,  1053,   136,    -1,    -1,    -1,   580,    -1,
      -1,   583,    -1,    -1,   586,    -1,    -1,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,  1076,    -1,    52,    -1,
      54,    55,    56,    57,    58,   134,   135,   136,   137,   138,
     316,   317,   318,   319,   320,   321,   322,   323,    -1,    -1,
      -1,   327,    -1,    77,    -1,   331,    -1,    -1,   630,    -1,
     632,    -1,    -1,    -1,    -1,    -1,    -1,   343,   640,  1077,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   650,    -1,
     652,   653,    -1,   905,   108,   109,    -1,    -1,   660,   661,
      52,    -1,    54,    55,    56,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   380,    -1,   382,    -1,    -1,   133,
     386,   387,   388,    -1,    -1,    77,   688,    -1,    -1,    -1,
      -1,  1171,  1172,  1173,    -1,    -1,   402,    -1,   700,    91,
      -1,   810,    -1,    -1,   813,  1143,    -1,    -1,    -1,   101,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   827,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   979,    -1,    -1,
    1168,  1169,  1170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   448,    -1,   136,    -1,    -1,    -1,  1000,    -1,
      -1,    -1,    -1,    -1,   960,   961,   148,    -1,    -1,   965,
     966,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1248,    -1,
     772,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   484,    -1,
      -1,  1033,    -1,    -1,    -1,    -1,    -1,    -1,   994,   995,
      -1,   997,   998,   786,   787,    -1,    -1,    -1,   504,    -1,
     793,   794,    -1,    -1,     2,    -1,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   819,    -1,   821,
     822,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      54,    55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1099,   841,   842,
      -1,   844,   845,    77,    -1,    53,    54,    -1,    -1,    57,
     566,    -1,   971,    -1,   570,   974,    -1,    91,   977,    -1,
      -1,    -1,    -1,    -1,   580,   984,    -1,   101,   987,    -1,
     586,  1087,   783,   784,   108,   109,    -1,    85,    -1,    -1,
      -1,    -1,    -1,   895,    -1,    -1,    -1,    -1,    -1,  1105,
      98,    99,   100,    -1,   906,    -1,   908,    -1,    -1,   133,
     912,    -1,    -1,    -1,    -1,    -1,   909,    52,    -1,    54,
      55,    56,    57,    58,   630,    -1,   632,  1179,    -1,    -1,
      -1,    -1,    -1,    -1,   640,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,   650,    -1,   652,   653,    -1,    -1,
      -1,    -1,    -1,    -1,   660,   661,    91,    -1,   960,   961,
      -1,    -1,    -1,   965,   966,    -1,   101,   969,   970,    -1,
      -1,   964,   107,   108,   109,    -1,    -1,     2,    -1,     4,
      -1,    -1,   688,   985,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    16,    17,  1102,    -1,    -1,    -1,    -1,   133,    -1,
      -1,   136,    -1,   996,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,    -1,   917,   918,    -1,  1021,
     921,    -1,    -1,  1025,    -1,    -1,    -1,  1029,    53,    54,
    1032,    -1,    -1,    -1,    -1,  1037,    -1,   938,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,   247,
      -1,   249,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1065,   262,    -1,   264,    -1,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,  1185,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1087,    -1,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1108,  1109,    -1,    -1,
      -1,    -1,    -1,   819,    -1,   821,   822,    -1,   316,    -1,
      -1,    -1,  1124,   321,    -1,    -1,    -1,    -1,    -1,   327,
    1132,    -1,    -1,   331,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   343,   344,  1048,    -1,    -1,
    1051,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1068,  1069,  1070,
      -1,    -1,  1073,  1074,  1176,    -1,    -1,  1078,  1079,    -1,
      -1,    -1,    -1,    -1,   382,  1187,    -1,   212,   386,   895,
     388,    -1,    -1,    -1,  1196,  1096,    -1,    -1,    -1,    -1,
     906,  1203,   908,    -1,    -1,    -1,   912,    -1,    -1,    -1,
     235,    -1,   237,   238,    -1,    -1,   241,    -1,   243,    -1,
      -1,    -1,   247,    -1,   249,    -1,    -1,    -1,    -1,    -1,
    1232,  1233,    -1,    -1,    -1,    -1,    -1,   262,    -1,   264,
      -1,    -1,    -1,    -1,   783,   784,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   960,   961,    -1,    -1,    -1,   965,
     966,    -1,    -1,   969,   970,    -1,    -1,    -1,    -1,  1271,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   985,
      -1,    -1,    -1,    -1,    -1,    -1,   484,    -1,    -1,    -1,
      -1,   316,   317,   318,   319,   320,   321,   322,   323,    -1,
      -1,    -1,   327,    -1,   502,    -1,   331,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1217,    -1,   343,  1025,
      -1,  1222,  1223,  1029,    -1,    -1,  1032,    -1,    -1,    -1,
      -1,  1037,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   380,    -1,   382,    -1,  1065,
      -1,   386,   387,   388,  1265,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   402,   917,   918,
      -1,  1087,   921,    -1,    -1,   583,    -1,    -1,   586,    -1,
      -1,  1292,    -1,    -1,  1100,    -1,    -1,    -1,    -1,   938,
      -1,    -1,  1108,  1109,    -1,    -1,    -1,    -1,    -1,     2,
      -1,     4,    -1,    -1,    -1,    -1,    -1,    -1,  1124,    -1,
      -1,    -1,    -1,   448,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   630,    -1,   632,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   652,    -1,    -1,    -1,    -1,   484,
      53,    54,   660,   661,    57,    -1,    -1,    -1,    -1,    -1,
    1176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   504,
      -1,  1187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1196,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   700,    -1,    -1,    98,    99,   100,    -1,  1048,
      -1,   709,  1051,    -1,    -1,   783,   784,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1232,  1233,    -1,  1068,
    1069,  1070,    -1,    -1,  1073,  1074,    -1,    -1,    -1,  1078,
    1079,   566,    -1,    -1,    -1,   570,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   580,    -1,  1096,    -1,    -1,
      -1,   586,    -1,    -1,    -1,  1271,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   772,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   630,    -1,   632,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   640,    -1,    -1,    -1,   212,
      -1,   819,    -1,    -1,   822,   650,    -1,   652,   653,    -1,
      -1,    -1,    -1,    -1,    -1,   660,   661,    -1,    -1,     2,
      -1,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   917,
     918,    -1,    -1,   921,   247,    -1,   249,    -1,    -1,    -1,
      -1,    -1,    -1,   688,    -1,    -1,    -1,    -1,    -1,   262,
     938,   264,    -1,    -1,    -1,    -1,    -1,    -1,  1217,    -1,
      -1,    -1,    -1,  1222,  1223,    -1,    -1,    -1,    -1,    -1,
      53,    54,   285,    -1,    57,    -1,    -1,   895,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   906,    -1,
     908,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,   316,    -1,    -1,  1265,    -1,   321,    -1,
      -1,    -1,    -1,    -1,   327,    98,    99,   100,   331,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     343,   344,    -1,  1292,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   960,   961,    -1,    -1,    -1,   965,   966,    -1,
      -1,   969,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1048,    -1,    -1,  1051,    -1,    -1,    -1,   985,    -1,   382,
      -1,    -1,    -1,   386,   819,   388,   821,   822,    -1,    -1,
    1068,  1069,  1070,    -1,    -1,  1073,  1074,    -1,    -1,    -1,
    1078,  1079,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1021,    -1,    -1,    -1,  1025,  1096,    -1,
      -1,  1029,    -1,    -1,  1032,    -1,    -1,   783,   784,  1037,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     895,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   906,    -1,   908,   247,    -1,   249,   912,    -1,  1087,
      -1,   484,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   262,
      -1,   264,  1100,    -1,    -1,    -1,    -1,    -1,    -1,   502,
    1108,  1109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   285,    -1,    -1,    -1,  1124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1132,   960,   961,    -1,    -1,    -1,
     965,   966,    -1,    -1,   969,   970,    -1,    -1,    -1,  1217,
      -1,    -1,    -1,   316,  1222,  1223,    -1,    -1,   321,    -1,
     985,    -1,    -1,    -1,   327,    -1,    -1,    -1,   331,    -1,
      -1,   917,   918,    -1,    -1,   921,    -1,    -1,  1176,    -1,
     343,   344,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1187,
     583,    -1,   938,   586,    -1,    -1,    -1,  1265,    -1,    -1,
    1025,    -1,    -1,    -1,  1029,  1203,    -1,  1032,    -1,    -1,
      -1,    -1,  1037,    -1,   783,   784,    -1,    -1,    -1,   382,
      -1,    -1,    -1,   386,  1292,   388,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   630,    -1,   632,
    1065,    -1,    -1,   783,   784,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   652,
      -1,    -1,  1087,    -1,    -1,    -1,    -1,   660,   661,    -1,
      -1,    -1,    -1,  1271,    -1,  1100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1108,  1109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1124,
      -1,    -1,  1048,    -1,     2,  1051,     4,   700,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   484,  1068,  1069,  1070,   783,   784,  1073,  1074,    -1,
      -1,    -1,  1078,  1079,    -1,    -1,    -1,    -1,     2,   502,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   917,   918,
    1096,  1176,   921,    -1,    -1,    53,    54,    -1,    -1,    57,
      -1,    -1,  1187,    -1,    -1,    -1,    -1,    -1,    -1,   938,
      -1,  1196,    -1,    -1,    -1,    -1,    -1,   917,   918,   772,
      -1,   921,    -1,    -1,    -1,    -1,    -1,    85,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   938,    -1,
      98,    99,   100,   101,    -1,    -1,    -1,  1232,  1233,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     583,    -1,    -1,   586,    -1,    -1,   819,    -1,    -1,   822,
      -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1271,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   917,
     918,    -1,    -1,   921,    -1,    -1,    -1,   630,    -1,   632,
      -1,  1217,    -1,    -1,    -1,    -1,  1222,  1223,    -1,  1048,
     938,    -1,  1051,    -1,    -1,    -1,    -1,    -1,    -1,   652,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   660,   661,  1068,
    1069,  1070,   895,    -1,  1073,  1074,    -1,    -1,  1048,  1078,
    1079,  1051,    -1,   906,   212,   908,    -1,    -1,    -1,  1265,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1096,  1068,  1069,
    1070,    -1,    -1,  1073,  1074,    -1,    -1,   700,  1078,  1079,
      -1,    -1,    -1,    -1,    -1,    -1,  1292,    -1,   212,   247,
      -1,   249,    -1,    -1,    -1,    -1,  1096,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   262,    -1,   264,   960,   961,    -1,
      -1,    -1,   965,   966,    -1,    -1,   969,    -1,    -1,     2,
      -1,     4,    -1,   247,    -1,   249,    -1,   285,    -1,    -1,
    1048,    -1,   985,  1051,    -1,    -1,    -1,    -1,   262,    -1,
     264,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   772,
    1068,  1069,  1070,    -1,    -1,  1073,  1074,    -1,   316,    -1,
    1078,  1079,    -1,   321,    -1,    -1,    -1,    -1,  1021,   327,
      53,    54,  1025,   331,    -1,    -1,  1029,    -1,  1096,  1032,
      -1,    -1,    -1,    -1,  1037,   343,   344,    -1,  1217,    -1,
      -1,    -1,   316,  1222,  1223,    -1,   819,   321,    -1,   822,
      -1,    -1,    -1,   327,    -1,    -1,    -1,   331,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1217,    -1,   343,
      -1,    -1,  1222,  1223,   382,    -1,    -1,    -1,   386,    -1,
     388,    -1,    -1,    -1,  1087,    -1,  1265,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1100,   783,   784,
      -1,    -1,    -1,    -1,    -1,  1108,  1109,    -1,   382,    -1,
      -1,    -1,   386,  1292,   388,  1265,    -1,    -1,    -1,    -1,
      -1,  1124,   895,    -1,    -1,    -1,    -1,    -1,    -1,  1132,
      -1,   783,   784,   906,    -1,   908,    -1,    -1,    -1,    -1,
      -1,    -1,  1292,    -1,    -1,    -1,    -1,    -1,    -1,  1217,
      -1,    -1,    -1,    -1,  1222,  1223,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1176,    -1,    -1,   484,    -1,    -1,   212,
      -1,    -1,    -1,    -1,  1187,    -1,    -1,   960,   961,    -1,
      -1,    -1,   965,   966,   502,    -1,   969,  1265,    -1,    -1,
    1203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     484,    -1,   985,    -1,   247,    -1,   249,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1292,    -1,    -1,    -1,    -1,   262,
      -1,   264,   917,   918,    -1,    -1,   921,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1021,    -1,
      -1,    -1,  1025,   938,    -1,    -1,  1029,    -1,    -1,  1032,
      -1,    -1,    -1,    -1,  1037,   917,   918,    -1,  1271,   921,
      -1,    -1,    -1,    -1,    -1,   583,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   316,    -1,    -1,   938,    -1,   321,    -1,
      -1,    -1,    -1,    -1,   327,    -1,    -1,    -1,   331,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     343,    -1,   586,    -1,  1087,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   630,    -1,   632,    -1,    -1,  1100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1108,  1109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   652,    -1,    -1,    -1,    -1,   382,
      -1,  1124,    -1,   386,    -1,   388,   630,    -1,   632,  1132,
      -1,    -1,    -1,  1048,    -1,    -1,  1051,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   652,    -1,
      -1,    -1,    -1,  1068,  1069,  1070,   660,   661,  1073,  1074,
      -1,    -1,   700,  1078,  1079,    -1,  1048,    -1,    -1,  1051,
      -1,    -1,    -1,  1176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1096,    -1,    -1,  1187,    -1,  1068,  1069,  1070,    -1,
      -1,  1073,  1074,    -1,    -1,    -1,  1078,  1079,    -1,    -1,
    1203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1096,    -1,    -1,    -1,    -1,    -1,
      -1,   484,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   772,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,     9,    10,    -1,    -1,    13,    14,    15,  1271,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      -1,   819,    -1,    -1,   822,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,  1217,    -1,    -1,    -1,    -1,  1222,  1223,    -1,
      -1,    -1,    -1,    -1,    -1,   819,    -1,    -1,   822,    -1,
      68,    -1,    -1,   586,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1217,    -1,    -1,    -1,    -1,
    1222,  1223,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
    1265,    -1,    -1,    -1,    -1,    -1,    -1,   895,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,   630,   906,   632,
     908,    -1,   120,    -1,    -1,    -1,    -1,  1292,    -1,    -1,
      -1,    -1,    -1,  1265,    -1,    -1,    -1,    -1,    -1,   652,
      -1,   895,    -1,    -1,    -1,   143,   144,   660,   661,    -1,
     148,   149,   906,   151,   908,   153,    -1,    -1,    -1,    -1,
    1292,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,   969,    -1,    94,    95,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   985,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   960,   961,    -1,    -1,
      -1,   965,   966,    16,    17,   969,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
      -1,   985,    -1,  1021,    -1,    -1,    -1,  1025,    -1,    -1,
      -1,  1029,    -1,    -1,  1032,    48,    49,    50,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,  1021,    -1,    -1,
      -1,  1025,    -1,    -1,    -1,  1029,    -1,    -1,  1032,    -1,
      -1,    -1,    -1,  1037,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,   819,    -1,    -1,   822,
      -1,    -1,  1100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1108,  1109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1087,    -1,    -1,  1124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1132,    -1,  1100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1108,  1109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   895,    -1,    -1,    -1,    -1,    -1,  1176,    -1,
      -1,    -1,    -1,   906,    -1,   908,    -1,    -1,    -1,  1187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,
     213,   214,   215,    -1,   217,  1203,    -1,    -1,    -1,    -1,
      -1,    -1,  1176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,  1187,   237,   238,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   960,   961,    -1,
      -1,    -1,   965,   966,    -1,    -1,   969,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   985,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,  1271,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1025,    -1,    -1,    -1,  1029,  1271,    -1,  1032,
      -1,    -1,    -1,    -1,  1037,    -1,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   348,   349,   350,   351,   352,
      -1,    -1,   355,   356,   357,   358,   359,   360,   361,   362,
      -1,   364,    -1,    -1,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,  1087,    -1,    -1,   380,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1108,  1109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     443,   444,    -1,    -1,    -1,   448,    -1,    -1,    -1,   452,
      -1,    -1,    -1,    -1,    -1,   458,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1176,    -1,    33,    34,    35,    36,    -1,
     473,    -1,    -1,    -1,  1187,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    -1,   489,    -1,    56,   492,
      -1,    59,    60,    61,    62,    63,    -1,    -1,    -1,    -1,
      -1,   504,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   521,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,    -1,    -1,   104,   105,    -1,   107,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   554,    -1,    -1,    -1,    -1,    -1,    -1,  1271,    -1,
      -1,    -1,    -1,   566,   132,    -1,    -1,   570,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,   580,    -1,    -1,
      -1,    -1,    -1,   586,    -1,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    -1,   640,    -1,    -1,
      -1,    -1,    59,    60,    61,    62,    63,   650,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   660,   661,    -1,
     663,   664,   665,   666,    -1,   668,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   677,   678,    -1,    -1,    -1,    -1,
     683,    -1,   685,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   694,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       0,     1,   139,     3,     4,     5,     6,     7,   731,    -1,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,   751,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,   764,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    -1,    -1,    -1,   792,
      -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   812,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,   102,    -1,   104,   105,    -1,   107,    -1,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   887,   888,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   899,   900,    -1,    -1,
      -1,    -1,   905,    33,    34,    35,    36,   910,    -1,   912,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,    52,    -1,    -1,    -1,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   959,   960,   961,    -1,
      90,    91,   965,   966,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,    -1,   104,   105,   979,   107,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,   994,   995,    -1,   997,   998,    -1,  1000,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
    1033,    -1,    -1,    -1,  1037,    -1,    -1,     8,     9,    10,
      -1,    -1,    13,    14,    15,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1087,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1099,    68,    -1,    -1,
      -1,    -1,  1105,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,   123,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,  1179,     0,   149,   150,
     151,    -1,   153,    -1,    -1,     8,     9,    10,    -1,    -1,
      13,    14,    15,  1196,    17,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    25,    -1,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    68,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    99,   100,    -1,    -1,
      -1,    -1,   127,   106,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,   120,    -1,    -1,
     123,   146,    -1,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,     0,    -1,   149,   150,   151,    -1,
     153,    -1,     8,     9,    10,    -1,    -1,    13,    14,    15,
      -1,    17,    -1,    -1,    -1,    -1,    44,    -1,    -1,    25,
      -1,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    68,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    -1,    99,   100,    -1,    -1,    -1,    -1,   127,
     106,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,   120,    -1,    -1,   123,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,     0,    -1,   149,   150,   151,    -1,   153,    -1,     8,
       9,    10,    -1,    -1,    13,    14,    15,    -1,    17,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    68,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    -1,
      -1,   100,    -1,    -1,    -1,    -1,   127,   106,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,     0,    -1,
     149,   150,   151,    -1,   153,    -1,     8,     9,    10,    -1,
      -1,    13,    14,    15,    -1,    17,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    68,    -1,    -1,    -1,
      94,    95,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    -1,    -1,   100,    -1,
      -1,    -1,    -1,   127,   106,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,     0,    -1,   149,   150,   151,
      -1,   153,    -1,     8,     9,    10,    -1,    -1,    13,    14,
      15,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    41,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    68,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    99,   100,    -1,    -1,    -1,    -1,
     127,   106,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,    -1,    -1,   120,    -1,    -1,   123,    -1,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,     0,    -1,   149,   150,   151,    -1,   153,    -1,
       8,     9,    10,    -1,    -1,    13,    14,    15,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      68,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,   106,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,   146,     0,
     148,   149,   150,   151,    -1,   153,    -1,     8,     9,    10,
      -1,    -1,    13,    14,    15,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    86,    87,    -1,    -1,    68,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,   106,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,     0,    -1,   149,   150,
     151,    -1,   153,    -1,     8,     9,    10,    -1,    -1,    13,
      14,    15,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    68,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,   106,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,     0,   148,   149,   150,   151,    -1,   153,
      -1,     8,     9,    10,    -1,    -1,    13,    14,    15,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    41,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
       0,   148,   149,   150,   151,    -1,   153,    -1,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    -1,    -1,    68,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    -1,    99,
     100,    -1,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,   126,   127,   153,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,     0,    -1,   149,
      -1,   151,    -1,   153,    -1,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,     0,    -1,   149,    -1,   151,    -1,
     153,    -1,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,     0,    -1,   149,    -1,   151,    -1,   153,    -1,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
     149,    -1,   151,     1,   153,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,   107,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,     1,   153,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,
     107,    -1,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,     1,   153,     3,     4,     5,
       6,     7,    -1,    -1,    10,    11,    12,    -1,    -1,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,   101,   102,    -1,   104,   105,
      -1,   107,    -1,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,     1,   153,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    -1,
      15,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,   104,
     105,    -1,   107,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,     1,   153,     3,
       4,     5,     6,     7,    -1,    -1,    10,    11,    12,    -1,
      -1,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,
     104,   105,    -1,   107,    -1,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
      -1,     9,    10,    11,    12,   139,   140,   141,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,   151,    -1,   153,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,   107,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,   139,   140,   141,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,   151,    -1,   153,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,
     102,    -1,   104,   105,    -1,   107,    -1,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,   151,
       1,   153,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,   102,    -1,   104,   105,    -1,   107,    -1,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
     151,     1,   153,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,   102,    -1,   104,   105,    -1,   107,    -1,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,   151,     1,   153,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,   102,    -1,   104,   105,    -1,   107,    -1,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,   151,     1,   153,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    -1,    -1,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,   107,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,   139,   140,   141,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,   151,    -1,   153,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,
     102,    -1,   104,   105,    -1,   107,    -1,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,   139,   140,   141,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,   151,
      -1,   153,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,   101,   102,    -1,   104,   105,
      -1,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,   139,   140,   141,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,   153,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,   102,    -1,   104,   105,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,   139,
     140,   141,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,   151,    -1,   153,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,
     104,   105,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,   139,   140,   141,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,   153,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,   107,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,   139,   140,   141,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,   151,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,
     102,    -1,   104,   105,    -1,   107,    -1,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      -1,    -1,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,   140,   141,    -1,
      -1,    -1,    -1,    -1,   147,   148,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,   141,    -1,    -1,    -1,    -1,    -1,
     147,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,   140,   141,
      -1,    -1,    -1,    -1,    -1,   147,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,   141,    -1,    -1,    -1,    -1,    -1,
     147,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,   140,   141,
       3,     4,     5,    -1,     7,   147,    -1,    -1,    11,    12,
      -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,
      -1,   104,   105,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,   139,    11,    12,    -1,
      -1,    -1,    16,   146,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,
     104,   105,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,   139,    11,    12,    -1,    -1,
      -1,    16,   146,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,   104,
     105,    -1,   107,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   139,   140,   141,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
     139,   140,   141,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,
      -1,   104,   105,    -1,   107,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   139,   140,   141,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   139,   140,   141,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   139,   140,
     141,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   139,   140,   141,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,   102,    -1,   104,   105,    -1,   107,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     139,   140,   141,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,
      -1,   104,   105,    -1,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   139,   140,   141,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,
     107,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   139,   140,   141,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,   102,    -1,   104,   105,    -1,    -1,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   139,   140,
     141,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,   104,
     105,    -1,   107,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   139,   140,   141,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,   102,    -1,   104,   105,    -1,   107,    -1,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     139,   140,   141,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,
      -1,   104,   105,    -1,   107,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   139,   140,   141,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,
     107,    -1,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   139,   140,   141,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,   102,    -1,   104,   105,    -1,   107,    -1,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   139,   140,
     141,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,   104,
     105,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   139,   140,   141,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,   102,    -1,   104,   105,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     139,   140,   141,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,
      -1,   104,   105,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   139,   140,   141,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,
     107,    -1,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    16,   139,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,   104,
     105,    -1,   107,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
      -1,    -1,    -1,    16,   139,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,
      -1,   104,   105,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,   139,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,   102,    -1,   104,   105,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,   139,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,   102,    -1,   104,   105,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    16,
     139,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,   102,    -1,   104,   105,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    16,   139,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
      52,    -1,    -1,    -1,    56,    -1,    -1,    59,    60,    61,
      62,    63,    -1,    98,    -1,    -1,   101,   102,    -1,   104,
     105,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,
      -1,    -1,   104,   105,   139,   107,    -1,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    49,    50,    51,    52,   139,    -1,    -1,
      56,    -1,    -1,    59,    60,    61,    62,    63,    -1,    -1,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      -1,    -1,    -1,    56,    90,    91,    59,    60,    61,    62,
      63,    -1,    98,    -1,    -1,   101,    -1,    -1,   104,   105,
      -1,   107,    -1,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    98,   132,    -1,   101,    -1,
      -1,   104,   105,   139,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    52,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    52,    53,    -1,    -1,
      56,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,   141,    -1,    -1,    -1,    -1,
      -1,   147
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   155,   156,     0,     1,     3,     4,     5,     6,     7,
      11,    12,    16,    18,    19,    20,    21,    22,    23,    24,
      30,    31,    32,    33,    34,    35,    36,    39,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    59,    60,    61,    62,    63,    64,    65,    75,    76,
      90,    91,    98,   101,   102,   104,   105,   107,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   139,   140,   141,
     157,   158,   159,   167,   169,   171,   177,   178,   184,   185,
     187,   188,   189,   191,   192,   193,   195,   196,   205,   208,
     224,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   253,   275,   283,   284,   336,   337,   338,   339,
     340,   341,   342,   345,   347,   348,   362,   363,   365,   366,
     367,   369,   370,   371,   372,   373,   411,   425,   159,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    56,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    86,    87,    92,    93,    94,    95,   107,   108,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     140,   141,   147,   199,   200,   201,   203,   204,   362,    39,
      58,    98,   101,   107,   108,   109,   112,   140,   177,   178,
     188,   196,   205,   210,   216,   219,   221,   234,   369,   370,
     372,   373,   409,   410,   216,   148,   217,   218,   148,   213,
     217,   148,   153,   418,    54,   200,   418,   143,   160,   143,
      21,    22,    31,    32,   187,   205,   234,   253,   205,   205,
     205,    56,     1,    47,   101,   163,   164,   165,   167,   190,
     191,   425,   167,   226,   211,   221,   409,   425,   210,   408,
     409,   425,    46,    98,   139,   146,   177,   178,   195,   224,
     234,   369,   370,   373,   276,    54,    55,    57,   199,   351,
     364,   351,   352,   353,   152,   152,   152,   152,   367,   184,
     205,   205,   151,   153,   417,   423,   424,    40,    41,    42,
      43,    44,    37,    38,   148,   376,   377,   378,   379,   425,
     376,   378,    26,   143,   213,   217,   245,   285,    28,   246,
     282,   126,   146,   101,   107,   192,   126,    25,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    94,    95,   100,   127,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   207,   207,    68,    96,    97,
     145,   415,   225,   171,   180,   180,   181,   182,   181,   180,
     417,   424,    98,   189,   196,   234,   258,   369,   370,   373,
      52,    56,    94,    98,   197,   198,   234,   369,   370,   373,
     198,    33,    34,    35,    36,    49,    50,    51,    52,    56,
     148,   176,   199,   371,   406,   216,    97,   415,   416,   285,
     339,    99,    99,   146,   210,    56,   210,   210,   210,   351,
     376,   376,   126,   100,   146,   220,   425,    97,   145,   415,
      99,    99,   146,   220,   216,   418,   419,   216,    91,   215,
     216,   221,   383,   409,   425,   171,   419,   171,    54,    64,
      65,   168,   148,   206,   157,   163,    97,   415,    99,   167,
     166,   190,   149,   417,   424,   419,   227,   419,   150,   146,
     153,   422,   146,   422,   144,   422,   418,    56,   367,   192,
     194,   377,   146,    97,   145,   415,   277,    66,   119,   121,
     122,   354,   119,   119,   354,    67,   354,   343,   349,   346,
     350,    77,   151,   159,   180,   180,   180,   180,   167,   171,
     171,    52,    54,    55,    56,    57,    58,    77,    91,   101,
     107,   108,   109,   133,   136,   263,   380,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   393,   394,   395,   396,
     397,   400,   401,   402,   403,   404,   126,   161,   163,   382,
     126,   161,   286,   287,   106,   186,   290,   291,   290,    70,
     209,   425,   190,   146,   195,   146,   209,   174,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   172,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,    52,    53,    56,   203,   213,   412,   413,
     215,   221,    52,    53,    56,   203,   213,   412,   161,    13,
     254,   423,   254,   163,   180,   163,   417,   230,    56,    97,
     145,   415,    25,   171,    52,    56,   197,   130,   374,    97,
     145,   415,   233,   407,    68,    97,   414,    52,    56,   412,
     209,   209,   202,   124,   126,   126,   209,   210,   107,   210,
     219,   409,    52,    56,   215,    52,    56,   209,   209,   410,
     419,   149,   419,   146,   419,   146,   419,   200,   228,   205,
     144,   144,   412,   412,   209,   160,   419,   165,   419,   409,
     146,   194,    52,    56,   215,    52,    56,   278,   356,   355,
     119,   344,   354,    66,   119,   119,   344,    66,   119,   205,
     101,   107,   259,   260,   261,   262,   385,   146,   405,   425,
     419,   264,   265,   146,   381,   210,   146,   405,    34,    52,
     146,   381,    52,   146,   381,    52,   188,   205,    10,   252,
       8,   247,   332,   425,   423,   188,   205,   252,   144,   288,
     286,   252,   292,   252,   107,   184,   210,   221,   222,   223,
     419,   194,   146,   169,   170,   184,   196,   205,   210,   212,
     223,   234,   373,   175,   173,   418,    99,    99,   213,   217,
     418,   420,   146,    99,    99,   213,   214,   217,   425,   252,
     163,    13,   163,   252,    27,   255,   423,   252,    25,   229,
     297,    17,   249,   295,    52,    56,   215,    52,    56,   181,
     232,   375,   231,    52,    56,   197,   215,   161,   171,   179,
     214,   217,   170,   205,   212,   170,   212,   200,   210,   210,
     220,    99,    99,   420,    99,    99,   383,   409,   171,   212,
     422,   192,   420,   148,   280,   382,   357,    54,    55,    57,
     361,   373,   152,   354,   152,   152,   152,   261,   385,   146,
     419,   146,   404,   210,   126,   380,   387,   400,   402,   390,
     394,   396,   388,   397,   402,   386,   388,    44,    44,   210,
     223,   333,   425,     9,    15,   248,   250,   335,   425,    44,
      44,   289,   144,   293,   210,   146,    44,   194,    44,   126,
      44,    97,   145,   415,    52,    56,    58,    90,    91,    98,
     101,   104,   105,   107,   112,   132,   275,   303,   304,   305,
     306,   309,   314,   315,   316,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   336,   337,
     340,   341,   342,   345,   347,   348,   370,   394,   303,   128,
     209,   209,   186,   150,    99,   209,   209,   186,    14,   250,
     251,   256,   257,   425,   257,   183,   298,   295,   252,   107,
     210,   294,   252,   420,   163,   423,   180,   161,   420,   252,
     419,   176,   285,   282,   209,   209,    99,   209,   209,   419,
     146,   419,   382,   279,   358,   419,   259,   262,   260,   146,
     381,   146,   381,   405,   146,   381,   146,   381,   381,   205,
     205,   100,   334,   425,   163,   162,   205,   205,   131,   270,
     271,   425,   270,   107,   210,   167,   167,   209,   205,    52,
      56,   215,    52,    56,   327,   327,    56,   197,   311,   304,
     312,   313,   314,   315,   318,   420,   310,   418,   421,    52,
     351,    52,    54,    55,    57,   101,   368,   100,   146,   131,
     146,   146,   304,    88,    89,    97,   145,   148,   307,   308,
      34,    52,   205,   170,   212,   170,   212,   209,   170,   212,
     170,   212,   163,   180,   252,   252,   299,   252,   210,   146,
     254,   252,   161,   423,   252,   209,   272,   418,    29,   123,
     281,   359,   146,   146,   388,   402,   388,   388,    98,   196,
     234,   369,   370,   373,   254,   163,   263,   266,   269,   272,
     386,   388,   389,   391,   392,   398,   399,   402,   404,   163,
     161,   210,   420,   304,   420,   107,   304,   318,   420,   146,
     112,   319,   144,   124,   180,   328,   312,   316,   309,   317,
     318,   321,   325,   327,   327,   197,   420,   419,   312,   315,
     319,   312,   315,   319,   170,   212,   254,   302,   303,   107,
     210,   163,   252,   149,   151,   161,   163,   360,   260,   381,
     146,   381,   381,   381,    56,    97,   145,   415,   163,   335,
     405,   272,   131,   264,   146,   267,   268,    98,   234,   146,
     405,   146,   267,   146,   267,   419,    52,   146,   146,   351,
     421,   149,   146,   146,   419,   419,   419,   420,   420,   420,
     163,   254,    40,    41,   210,   257,   295,   296,    52,   273,
     274,   384,   252,   144,   163,   388,    52,    56,   215,    52,
      56,   332,   131,   234,   266,   399,   402,    56,    97,   391,
     396,   388,   398,   402,   388,   146,   317,   317,   316,   318,
     256,   300,   180,   180,   146,   418,   120,   381,   420,   146,
     267,   146,   267,    52,    56,   405,   146,   267,   146,   267,
     267,   317,   146,   163,   274,   388,   402,   388,   388,   257,
     297,   301,   267,   146,   267,   267,   267,   388,   267
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   154,   156,   155,   157,   158,   158,   158,   158,   159,
     159,   160,   162,   161,   161,   163,   164,   164,   164,   164,
     165,   166,   165,   168,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   170,   170,   170,   171,   171,   171,   171,
     171,   172,   173,   171,   174,   175,   171,   171,   176,   177,
     179,   178,   180,   182,   183,   181,   184,   184,   185,   185,
     186,   187,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   189,   189,   190,   190,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   192,   192,   193,
     193,   194,   194,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   197,   197,   198,   198,   198,   199,   199,   199,   199,
     199,   200,   200,   201,   202,   201,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   206,   205,   205,
     205,   205,   205,   205,   205,   207,   207,   207,   207,   208,
     208,   209,   209,   210,   211,   211,   211,   211,   212,   212,
     213,   213,   213,   214,   214,   215,   215,   215,   215,   215,
     216,   216,   216,   216,   216,   218,   217,   219,   219,   220,
     220,   221,   221,   221,   221,   222,   222,   223,   223,   223,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   225,   224,   226,   224,   227,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   228,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   229,   224,
     230,   224,   224,   224,   231,   224,   232,   224,   233,   224,
     224,   224,   224,   224,   224,   224,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   254,   254,   255,
     255,   256,   256,   257,   257,   258,   258,   259,   259,   260,
     260,   261,   261,   261,   261,   261,   262,   262,   263,   263,
     265,   264,   266,   266,   266,   266,   267,   267,   268,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   270,   270,   271,   271,   272,   272,
     273,   273,   274,   274,   276,   277,   278,   279,   275,   280,
     280,   281,   281,   282,   283,   283,   283,   283,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   285,   285,   287,
     288,   289,   286,   291,   292,   293,   290,   294,   294,   294,
     294,   295,   296,   296,   298,   299,   300,   297,   301,   301,
     302,   302,   302,   303,   303,   303,   303,   303,   303,   304,
     305,   305,   306,   306,   307,   308,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   310,
     309,   309,   311,   309,   312,   312,   312,   312,   312,   312,
     312,   312,   313,   313,   314,   314,   315,   316,   316,   317,
     317,   318,   319,   319,   319,   319,   320,   320,   321,   321,
     322,   322,   323,   323,   324,   325,   325,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   328,   329,   329,
     330,   331,   331,   331,   332,   332,   333,   333,   333,   334,
     334,   335,   335,   336,   336,   337,   338,   338,   338,   339,
     340,   341,   342,   343,   343,   344,   344,   345,   346,   346,
     347,   348,   349,   349,   350,   350,   351,   351,   352,   352,
     353,   353,   354,   355,   354,   356,   357,   358,   359,   360,
     354,   361,   361,   361,   361,   362,   362,   363,   364,   364,
     364,   364,   365,   366,   366,   367,   367,   367,   367,   368,
     368,   368,   369,   369,   369,   369,   369,   370,   370,   370,
     370,   370,   370,   370,   371,   371,   372,   372,   373,   373,
     375,   374,   374,   376,   376,   377,   378,   379,   378,   380,
     380,   380,   380,   380,   381,   381,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   383,   384,   384,   384,   384,   385,   385,   386,   387,
     387,   388,   388,   389,   390,   390,   391,   391,   392,   392,
     393,   393,   394,   394,   395,   396,   396,   397,   398,   399,
     399,   400,   400,   401,   401,   402,   402,   403,   403,   404,
     404,   405,   405,   406,   407,   406,   408,   408,   409,   409,
     410,   410,   410,   410,   410,   411,   411,   411,   412,   412,
     412,   412,   413,   413,   413,   414,   414,   415,   415,   416,
     416,   417,   417,   418,   418,   419,   420,   421,   422,   422,
     422,   423,   423,   424,   424,   425
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     3,     2,     1,
       2,     3,     0,     6,     3,     2,     1,     1,     3,     2,
       1,     0,     3,     0,     4,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     4,     1,     4,     4,     6,     4,
       1,     4,     4,     7,     6,     6,     6,     6,     4,     6,
       4,     6,     4,     1,     3,     1,     1,     3,     3,     3,
       2,     0,     0,     5,     0,     0,     5,     1,     1,     2,
       0,     5,     1,     0,     0,     4,     1,     1,     1,     4,
       3,     1,     2,     3,     4,     5,     4,     5,     2,     2,
       2,     2,     2,     1,     3,     1,     3,     1,     2,     3,
       5,     2,     4,     2,     4,     1,     3,     1,     3,     2,
       3,     1,     3,     1,     1,     4,     3,     3,     3,     3,
       2,     1,     1,     1,     4,     3,     3,     3,     3,     2,
       1,     1,     1,     2,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     7,
       6,     6,     6,     6,     5,     4,     3,     3,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     4,     2,
       2,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     3,     3,     0,     4,     6,
       4,     6,     4,     6,     1,     1,     1,     1,     1,     3,
       3,     1,     1,     1,     1,     2,     4,     2,     1,     3,
       3,     5,     3,     1,     1,     1,     1,     2,     4,     2,
       1,     2,     2,     4,     1,     0,     2,     2,     1,     2,
       1,     1,     2,     3,     4,     1,     1,     3,     4,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     0,     3,     0,     4,     3,     3,     2,
       3,     3,     1,     4,     3,     1,     0,     6,     4,     3,
       2,     1,     2,     1,     6,     6,     4,     4,     0,     6,
       0,     5,     5,     6,     0,     6,     0,     7,     0,     5,
       4,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     5,     1,     2,     1,     1,     1,     3,     1,
       3,     1,     3,     5,     1,     3,     2,     1,     1,     1,
       0,     2,     4,     2,     2,     1,     2,     0,     1,     6,
       8,     4,     6,     4,     2,     6,     2,     4,     6,     2,
       4,     2,     4,     1,     1,     1,     3,     4,     1,     4,
       1,     3,     1,     1,     0,     0,     0,     0,     7,     4,
       1,     3,     3,     3,     2,     4,     5,     5,     2,     4,
       4,     3,     3,     3,     2,     1,     4,     3,     3,     0,
       0,     0,     5,     0,     0,     0,     5,     1,     2,     3,
       4,     5,     1,     1,     0,     0,     0,     8,     1,     1,
       1,     3,     3,     1,     2,     3,     1,     1,     1,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     4,     4,
       4,     3,     4,     4,     4,     3,     3,     3,     2,     0,
       4,     2,     0,     4,     1,     1,     2,     3,     5,     2,
       4,     1,     2,     3,     1,     3,     5,     2,     1,     1,
       3,     1,     3,     1,     2,     1,     1,     3,     2,     1,
       1,     3,     2,     1,     2,     1,     1,     1,     3,     3,
       2,     2,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       4,     2,     3,     1,     6,     1,     1,     1,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     2,     3,
       3,     3,     4,     0,     3,     1,     2,     4,     0,     3,
       4,     4,     0,     3,     0,     3,     0,     2,     0,     2,
       0,     2,     1,     0,     3,     0,     0,     0,     0,     0,
       8,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     4,     0,     1,     1,     3,     1,     0,     3,     4,
       2,     2,     1,     1,     2,     0,     6,     8,     4,     6,
       4,     6,     2,     4,     6,     2,     4,     2,     4,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     2,     1,     2,     1,     1,     3,
       1,     3,     1,     1,     2,     2,     1,     3,     3,     1,
       3,     1,     3,     1,     1,     2,     1,     1,     1,     2,
       1,     2,     1,     1,     0,     4,     1,     2,     1,     3,
       3,     2,     1,     4,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     0,     1,     2,     2,     2,     0,     1,
       1,     1,     1,     1,     2,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, p, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (p, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (p, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (p, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (p, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (p, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (p, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, p); \
      YYFPRINTF (p, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, struct parser_params *p)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (p);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_tIDENTIFIER: /* "local variable or method"  */
#line 1090 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%"PRIsVALUE, rb_id2str(((*yyvaluep).val)));
#else
    rb_parser_printf(p, "%"PRIsVALUE, RNODE(((*yyvaluep).val))->nd_rval);
#endif
}
#line 6241 "ripper.c"
        break;

    case YYSYMBOL_tFID: /* "method"  */
#line 1090 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%"PRIsVALUE, rb_id2str(((*yyvaluep).val)));
#else
    rb_parser_printf(p, "%"PRIsVALUE, RNODE(((*yyvaluep).val))->nd_rval);
#endif
}
#line 6253 "ripper.c"
        break;

    case YYSYMBOL_tGVAR: /* "global variable"  */
#line 1090 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%"PRIsVALUE, rb_id2str(((*yyvaluep).val)));
#else
    rb_parser_printf(p, "%"PRIsVALUE, RNODE(((*yyvaluep).val))->nd_rval);
#endif
}
#line 6265 "ripper.c"
        break;

    case YYSYMBOL_tIVAR: /* "instance variable"  */
#line 1090 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%"PRIsVALUE, rb_id2str(((*yyvaluep).val)));
#else
    rb_parser_printf(p, "%"PRIsVALUE, RNODE(((*yyvaluep).val))->nd_rval);
#endif
}
#line 6277 "ripper.c"
        break;

    case YYSYMBOL_tCONSTANT: /* "constant"  */
#line 1090 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%"PRIsVALUE, rb_id2str(((*yyvaluep).val)));
#else
    rb_parser_printf(p, "%"PRIsVALUE, RNODE(((*yyvaluep).val))->nd_rval);
#endif
}
#line 6289 "ripper.c"
        break;

    case YYSYMBOL_tCVAR: /* "class variable"  */
#line 1090 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%"PRIsVALUE, rb_id2str(((*yyvaluep).val)));
#else
    rb_parser_printf(p, "%"PRIsVALUE, RNODE(((*yyvaluep).val))->nd_rval);
#endif
}
#line 6301 "ripper.c"
        break;

    case YYSYMBOL_tLABEL: /* "label"  */
#line 1090 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%"PRIsVALUE, rb_id2str(((*yyvaluep).val)));
#else
    rb_parser_printf(p, "%"PRIsVALUE, RNODE(((*yyvaluep).val))->nd_rval);
#endif
}
#line 6313 "ripper.c"
        break;

    case YYSYMBOL_tINTEGER: /* "integer literal"  */
#line 1097 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%+"PRIsVALUE, ((*yyvaluep).val)->nd_lit);
#else
    rb_parser_printf(p, "%+"PRIsVALUE, get_value(((*yyvaluep).val)));
#endif
}
#line 6325 "ripper.c"
        break;

    case YYSYMBOL_tFLOAT: /* "float literal"  */
#line 1097 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%+"PRIsVALUE, ((*yyvaluep).val)->nd_lit);
#else
    rb_parser_printf(p, "%+"PRIsVALUE, get_value(((*yyvaluep).val)));
#endif
}
#line 6337 "ripper.c"
        break;

    case YYSYMBOL_tRATIONAL: /* "rational literal"  */
#line 1097 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%+"PRIsVALUE, ((*yyvaluep).val)->nd_lit);
#else
    rb_parser_printf(p, "%+"PRIsVALUE, get_value(((*yyvaluep).val)));
#endif
}
#line 6349 "ripper.c"
        break;

    case YYSYMBOL_tIMAGINARY: /* "imaginary literal"  */
#line 1097 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%+"PRIsVALUE, ((*yyvaluep).val)->nd_lit);
#else
    rb_parser_printf(p, "%+"PRIsVALUE, get_value(((*yyvaluep).val)));
#endif
}
#line 6361 "ripper.c"
        break;

    case YYSYMBOL_tCHAR: /* "char literal"  */
#line 1097 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%+"PRIsVALUE, ((*yyvaluep).val)->nd_lit);
#else
    rb_parser_printf(p, "%+"PRIsVALUE, get_value(((*yyvaluep).val)));
#endif
}
#line 6373 "ripper.c"
        break;

    case YYSYMBOL_tNTH_REF: /* "numbered reference"  */
#line 1104 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "$%ld", ((*yyvaluep).val)->nd_nth);
#else
    rb_parser_printf(p, "%"PRIsVALUE, ((*yyvaluep).val));
#endif
}
#line 6385 "ripper.c"
        break;

    case YYSYMBOL_tBACK_REF: /* "back reference"  */
#line 1111 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "$%c", (int)((*yyvaluep).val)->nd_nth);
#else
    rb_parser_printf(p, "%"PRIsVALUE, ((*yyvaluep).val));
#endif
}
#line 6397 "ripper.c"
        break;

    case YYSYMBOL_tSTRING_CONTENT: /* "literal content"  */
#line 1097 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%+"PRIsVALUE, ((*yyvaluep).val)->nd_lit);
#else
    rb_parser_printf(p, "%+"PRIsVALUE, get_value(((*yyvaluep).val)));
#endif
}
#line 6409 "ripper.c"
        break;

    case YYSYMBOL_tOP_ASGN: /* "operator-assignment"  */
#line 1090 "ripper.y"
         {
#ifndef RIPPER
    rb_parser_printf(p, "%"PRIsVALUE, rb_id2str(((*yyvaluep).val)));
#else
    rb_parser_printf(p, "%"PRIsVALUE, RNODE(((*yyvaluep).val))->nd_rval);
#endif
}
#line 6421 "ripper.c"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, struct parser_params *p)
{
  YYFPRINTF (p, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (p, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, p);
  YYFPRINTF (p, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
ruby_parser_yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop, struct parser_params *p)
#define yy_stack_print(b, t) ruby_parser_yy_stack_print(b, t, p)
{
  YYFPRINTF (p, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (p, " %d", yybot);
    }
  YYFPRINTF (p, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, struct parser_params *p)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (p, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (p, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), p);
      YYFPRINTF (p, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, p); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#ifndef yydebug
int yydebug;
#endif
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (struct parser_params *p, YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, struct parser_params *p)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (p);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (struct parser_params *p)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((p, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */


/* User initialization code.  */
#line 1122 "ripper.y"
{
    RUBY_SET_YYLLOC_OF_NONE(yylloc);
}

#line 6916 "ripper.c"

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((p, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((p, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((p, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, p);
    }

  if (yychar <= END_OF_INPUT)
    {
      yychar = END_OF_INPUT;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((p, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 1327 "ripper.y"
                   {
			SET_LEX_STATE(EXPR_BEG);
			local_push(p, ifndef_ripper(1)+0);
		    }
#line 7132 "ripper.c"
    break;

  case 3: /* program: $@1 top_compstmt  */
#line 1332 "ripper.y"
                    {
#if 0
			if ((yyvsp[0].val) && !compile_for_eval) {
			    NODE *node = (yyvsp[0].val);
			    /* last expression should not be void */
			    if (nd_type_p(node, NODE_BLOCK)) {
				while (node->nd_next) {
				    node = node->nd_next;
				}
				node = node->nd_head;
			    }
			    node = remove_begin(node);
			    void_expr(p, node);
			}
			p->eval_tree = NEW_SCOPE(0, block_append(p, p->eval_tree, (yyvsp[0].val)), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(program,v1);p->result=v2;}
			local_pop(p);
		    }
#line 7156 "ripper.c"
    break;

  case 4: /* top_compstmt: top_stmts opt_terms  */
#line 1354 "ripper.y"
                    {
			(yyval.val) = void_stmts(p, (yyvsp[-1].val));
		    }
#line 7164 "ripper.c"
    break;

  case 5: /* top_stmts: none  */
#line 1360 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=dispatch0(stmts_new);v2=dispatch0(void_stmt);v3=v1;v4=v2;v5=dispatch2(stmts_add,v3,v4);(yyval.val)=v5;}
		    }
#line 7175 "ripper.c"
    break;

  case 6: /* top_stmts: top_stmt  */
#line 1367 "ripper.y"
                    {
#if 0
			(yyval.val) = newline_node((yyvsp[0].val));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(stmts_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(stmts_add,v2,v3);(yyval.val)=v4;}
		    }
#line 7186 "ripper.c"
    break;

  case 7: /* top_stmts: top_stmts terms top_stmt  */
#line 1374 "ripper.y"
                    {
#if 0
			(yyval.val) = block_append(p, (yyvsp[-2].val), newline_node((yyvsp[0].val)));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(stmts_add,v1,v2);(yyval.val)=v3;}
		    }
#line 7197 "ripper.c"
    break;

  case 8: /* top_stmts: error top_stmt  */
#line 1381 "ripper.y"
                    {
			(yyval.val) = remove_begin((yyvsp[0].val));
		    }
#line 7205 "ripper.c"
    break;

  case 10: /* top_stmt: "`BEGIN'" begin_block  */
#line 1388 "ripper.y"
                    {
			(yyval.val) = (yyvsp[0].val);
		    }
#line 7213 "ripper.c"
    break;

  case 11: /* begin_block: '{' top_compstmt '}'  */
#line 1394 "ripper.y"
                    {
#if 0
			p->eval_tree_begin = block_append(p, p->eval_tree_begin,
							  NEW_BEGIN((yyvsp[-1].val), &(yyloc)));
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(BEGIN,v1);(yyval.val)=v2;}
		    }
#line 7226 "ripper.c"
    break;

  case 12: /* $@2: %empty  */
#line 1406 "ripper.y"
                         {if (!(yyvsp[-1].val)) {yyerror1(&(yylsp[0]), "else without rescue is useless");}}
#line 7232 "ripper.c"
    break;

  case 13: /* bodystmt: compstmt opt_rescue k_else $@2 compstmt opt_ensure  */
#line 1409 "ripper.y"
                    {
#if 0
			(yyval.val) = new_bodystmt(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=escape_Qundef((yyvsp[-5].val));v2=escape_Qundef((yyvsp[-4].val));v3=escape_Qundef((yyvsp[-1].val));v4=escape_Qundef((yyvsp[0].val));v5=dispatch4(bodystmt,v1,v2,v3,v4);(yyval.val)=v5;}
		    }
#line 7243 "ripper.c"
    break;

  case 14: /* bodystmt: compstmt opt_rescue opt_ensure  */
#line 1418 "ripper.y"
                    {
#if 0
			(yyval.val) = new_bodystmt(p, (yyvsp[-2].val), (yyvsp[-1].val), 0, (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=escape_Qundef((yyvsp[-2].val));v2=escape_Qundef((yyvsp[-1].val));v3=Qnil;v4=escape_Qundef((yyvsp[0].val));v5=dispatch4(bodystmt,v1,v2,v3,v4);(yyval.val)=v5;}
		    }
#line 7254 "ripper.c"
    break;

  case 15: /* compstmt: stmts opt_terms  */
#line 1427 "ripper.y"
                    {
			(yyval.val) = void_stmts(p, (yyvsp[-1].val));
		    }
#line 7262 "ripper.c"
    break;

  case 16: /* stmts: none  */
#line 1433 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=dispatch0(stmts_new);v2=dispatch0(void_stmt);v3=v1;v4=v2;v5=dispatch2(stmts_add,v3,v4);(yyval.val)=v5;}
		    }
#line 7273 "ripper.c"
    break;

  case 17: /* stmts: stmt_or_begin  */
#line 1440 "ripper.y"
                    {
#if 0
			(yyval.val) = newline_node((yyvsp[0].val));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(stmts_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(stmts_add,v2,v3);(yyval.val)=v4;}
		    }
#line 7284 "ripper.c"
    break;

  case 18: /* stmts: stmts terms stmt_or_begin  */
#line 1447 "ripper.y"
                    {
#if 0
			(yyval.val) = block_append(p, (yyvsp[-2].val), newline_node((yyvsp[0].val)));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(stmts_add,v1,v2);(yyval.val)=v3;}
		    }
#line 7295 "ripper.c"
    break;

  case 19: /* stmts: error stmt  */
#line 1454 "ripper.y"
                    {
			(yyval.val) = remove_begin((yyvsp[0].val));
		    }
#line 7303 "ripper.c"
    break;

  case 20: /* stmt_or_begin: stmt  */
#line 1460 "ripper.y"
                    {
			(yyval.val) = (yyvsp[0].val);
		    }
#line 7311 "ripper.c"
    break;

  case 21: /* $@3: %empty  */
#line 1464 "ripper.y"
                    {
			yyerror1(&(yylsp[0]), "BEGIN is permitted only at toplevel");
		    }
#line 7319 "ripper.c"
    break;

  case 22: /* stmt_or_begin: "`BEGIN'" $@3 begin_block  */
#line 1468 "ripper.y"
                    {
			(yyval.val) = (yyvsp[0].val);
		    }
#line 7327 "ripper.c"
    break;

  case 23: /* $@4: %empty  */
#line 1473 "ripper.y"
                                      {SET_LEX_STATE(EXPR_FNAME|EXPR_FITEM);}
#line 7333 "ripper.c"
    break;

  case 24: /* stmt: "`alias'" fitem $@4 fitem  */
#line 1474 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_ALIAS((yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(alias,v1,v2);(yyval.val)=v3;}
		    }
#line 7344 "ripper.c"
    break;

  case 25: /* stmt: "`alias'" "global variable" "global variable"  */
#line 1481 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_VALIAS((yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=(yyvsp[0].val);v3=dispatch2(var_alias,v1,v2);(yyval.val)=v3;}
		    }
#line 7355 "ripper.c"
    break;

  case 26: /* stmt: "`alias'" "global variable" "back reference"  */
#line 1488 "ripper.y"
                    {
#if 0
			char buf[2];
			buf[0] = '$';
			buf[1] = (char)(yyvsp[0].val)->nd_nth;
			(yyval.val) = NEW_VALIAS((yyvsp[-1].val), rb_intern2(buf, 2), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=(yyvsp[0].val);v3=dispatch2(var_alias,v1,v2);(yyval.val)=v3;}
		    }
#line 7369 "ripper.c"
    break;

  case 27: /* stmt: "`alias'" "global variable" "numbered reference"  */
#line 1498 "ripper.y"
                    {
			static const char mesg[] = "can't make alias for the number variables";
#if 0
			yyerror1(&(yylsp[0]), mesg);
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=ERR_MESG();v2=(yyvsp[0].val);v3=dispatch2(alias_error,v1,v2);(yyval.val)=v3;}ripper_error(p);
		    }
#line 7382 "ripper.c"
    break;

  case 28: /* stmt: "`undef'" undef_list  */
#line 1507 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[0].val);
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(undef,v1);(yyval.val)=v2;}
		    }
#line 7393 "ripper.c"
    break;

  case 29: /* stmt: stmt "`if' modifier" expr_value  */
#line 1514 "ripper.y"
                    {
#if 0
			(yyval.val) = new_if(p, (yyvsp[0].val), remove_begin((yyvsp[-2].val)), 0, &(yyloc));
			fixpos((yyval.val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[0].val);v2=(yyvsp[-2].val);v3=dispatch2(if_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 7405 "ripper.c"
    break;

  case 30: /* stmt: stmt "`unless' modifier" expr_value  */
#line 1522 "ripper.y"
                    {
#if 0
			(yyval.val) = new_unless(p, (yyvsp[0].val), remove_begin((yyvsp[-2].val)), 0, &(yyloc));
			fixpos((yyval.val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[0].val);v2=(yyvsp[-2].val);v3=dispatch2(unless_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 7417 "ripper.c"
    break;

  case 31: /* stmt: stmt "`while' modifier" expr_value  */
#line 1530 "ripper.y"
                    {
#if 0
			if ((yyvsp[-2].val) && nd_type_p((yyvsp[-2].val), NODE_BEGIN)) {
			    (yyval.val) = NEW_WHILE(cond(p, (yyvsp[0].val), &(yylsp[0])), (yyvsp[-2].val)->nd_body, 0, &(yyloc));
			}
			else {
			    (yyval.val) = NEW_WHILE(cond(p, (yyvsp[0].val), &(yylsp[0])), (yyvsp[-2].val), 1, &(yyloc));
			}
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[0].val);v2=(yyvsp[-2].val);v3=dispatch2(while_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 7433 "ripper.c"
    break;

  case 32: /* stmt: stmt "`until' modifier" expr_value  */
#line 1542 "ripper.y"
                    {
#if 0
			if ((yyvsp[-2].val) && nd_type_p((yyvsp[-2].val), NODE_BEGIN)) {
			    (yyval.val) = NEW_UNTIL(cond(p, (yyvsp[0].val), &(yylsp[0])), (yyvsp[-2].val)->nd_body, 0, &(yyloc));
			}
			else {
			    (yyval.val) = NEW_UNTIL(cond(p, (yyvsp[0].val), &(yylsp[0])), (yyvsp[-2].val), 1, &(yyloc));
			}
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[0].val);v2=(yyvsp[-2].val);v3=dispatch2(until_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 7449 "ripper.c"
    break;

  case 33: /* stmt: stmt "`rescue' modifier" stmt  */
#line 1554 "ripper.y"
                    {
#if 0
			NODE *resq;
			YYLTYPE loc = code_loc_gen(&(yylsp[-1]), &(yylsp[0]));
			resq = NEW_RESBODY(0, remove_begin((yyvsp[0].val)), 0, &loc);
			(yyval.val) = NEW_RESCUE(remove_begin((yyvsp[-2].val)), resq, 0, &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(rescue_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 7463 "ripper.c"
    break;

  case 34: /* stmt: "`END'" '{' compstmt '}'  */
#line 1564 "ripper.y"
                    {
			if (p->ctxt.in_def) {
			    rb_warn0("END in method; use at_exit");
			}
#if 0
			{
			    NODE *scope = NEW_NODE(
				NODE_SCOPE, 0 /* tbl */, (yyvsp[-1].val) /* body */, 0 /* args */, &(yyloc));
			    (yyval.val) = NEW_POSTEXE(scope, &(yyloc));
			}
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(END,v1);(yyval.val)=v2;}
		    }
#line 7481 "ripper.c"
    break;

  case 36: /* stmt: mlhs '=' lex_ctxt command_call  */
#line 1579 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[0].val));
			(yyval.val) = node_assign(p, (yyvsp[-3].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[0].val);v3=dispatch2(massign,v1,v2);(yyval.val)=v3;}
		    }
#line 7493 "ripper.c"
    break;

  case 37: /* stmt: lhs '=' lex_ctxt mrhs  */
#line 1587 "ripper.y"
                    {
#if 0
			(yyval.val) = node_assign(p, (yyvsp[-3].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[0].val);v3=dispatch2(assign,v1,v2);(yyval.val)=v3;}
		    }
#line 7504 "ripper.c"
    break;

  case 38: /* stmt: mlhs '=' lex_ctxt mrhs_arg "`rescue' modifier" stmt  */
#line 1594 "ripper.y"
                    {
#if 0
                        YYLTYPE loc = code_loc_gen(&(yylsp[-1]), &(yylsp[0]));
			(yyval.val) = node_assign(p, (yyvsp[-5].val), NEW_RESCUE((yyvsp[-2].val), NEW_RESBODY(0, remove_begin((yyvsp[0].val)), 0, &loc), 0, &(yyloc)), (yyvsp[-3].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(rescue_mod,v1,v2);v4=(yyvsp[-5].val);v5=v3;v6=dispatch2(massign,v4,v5);(yyval.val)=v6;}
                    }
#line 7516 "ripper.c"
    break;

  case 39: /* stmt: mlhs '=' lex_ctxt mrhs_arg  */
#line 1602 "ripper.y"
                    {
#if 0
			(yyval.val) = node_assign(p, (yyvsp[-3].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[0].val);v3=dispatch2(massign,v1,v2);(yyval.val)=v3;}
		    }
#line 7527 "ripper.c"
    break;

  case 41: /* command_asgn: lhs '=' lex_ctxt command_rhs  */
#line 1612 "ripper.y"
                    {
#if 0
			(yyval.val) = node_assign(p, (yyvsp[-3].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[0].val);v3=dispatch2(assign,v1,v2);(yyval.val)=v3;}
		    }
#line 7538 "ripper.c"
    break;

  case 42: /* command_asgn: var_lhs "operator-assignment" lex_ctxt command_rhs  */
#line 1619 "ripper.y"
                    {
#if 0
			(yyval.val) = new_op_assign(p, (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-3].val);v2=(yyvsp[-2].val);v3=(yyvsp[0].val);v4=dispatch3(opassign,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 7549 "ripper.c"
    break;

  case 43: /* command_asgn: primary_value '[' opt_call_args rbracket "operator-assignment" lex_ctxt command_rhs  */
#line 1626 "ripper.y"
                    {
#if 0
			(yyval.val) = new_ary_op_assign(p, (yyvsp[-6].val), (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-4]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-6].val);v2=escape_Qundef((yyvsp[-4].val));v3=dispatch2(aref_field,v1,v2);v4=v3;v5=(yyvsp[-2].val);v6=(yyvsp[0].val);v7=dispatch3(opassign,v4,v5,v6);(yyval.val)=v7;}

		    }
#line 7561 "ripper.c"
    break;

  case 44: /* command_asgn: primary_value call_op "local variable or method" "operator-assignment" lex_ctxt command_rhs  */
#line 1634 "ripper.y"
                    {
#if 0
			(yyval.val) = new_attr_op_assign(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-5].val);v2=(yyvsp[-4].val);v3=(yyvsp[-3].val);v4=dispatch3(field,v1,v2,v3);v5=v4;v6=(yyvsp[-2].val);v7=(yyvsp[0].val);v8=dispatch3(opassign,v5,v6,v7);(yyval.val)=v8;}
		    }
#line 7572 "ripper.c"
    break;

  case 45: /* command_asgn: primary_value call_op "constant" "operator-assignment" lex_ctxt command_rhs  */
#line 1641 "ripper.y"
                    {
#if 0
			(yyval.val) = new_attr_op_assign(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-5].val);v2=(yyvsp[-4].val);v3=(yyvsp[-3].val);v4=dispatch3(field,v1,v2,v3);v5=v4;v6=(yyvsp[-2].val);v7=(yyvsp[0].val);v8=dispatch3(opassign,v5,v6,v7);(yyval.val)=v8;}
		    }
#line 7583 "ripper.c"
    break;

  case 46: /* command_asgn: primary_value "::" "constant" "operator-assignment" lex_ctxt command_rhs  */
#line 1648 "ripper.y"
                    {
#if 0
			YYLTYPE loc = code_loc_gen(&(yylsp[-5]), &(yylsp[-3]));
			(yyval.val) = new_const_op_assign(p, NEW_COLON2((yyvsp[-5].val), (yyvsp[-3].val), &loc), (yyvsp[-2].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-5].val);v2=(yyvsp[-3].val);v3=dispatch2(const_path_field,v1,v2);v4=v3;v5=(yyvsp[-2].val);v6=(yyvsp[0].val);v7=dispatch3(opassign,v4,v5,v6);(yyval.val)=v7;}
		    }
#line 7595 "ripper.c"
    break;

  case 47: /* command_asgn: primary_value "::" "local variable or method" "operator-assignment" lex_ctxt command_rhs  */
#line 1656 "ripper.y"
                    {
#if 0
			(yyval.val) = new_attr_op_assign(p, (yyvsp[-5].val), ID2VAL(idCOLON2), (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-5].val);v2=ID2VAL(idCOLON2);v3=(yyvsp[-3].val);v4=dispatch3(field,v1,v2,v3);v5=v4;v6=(yyvsp[-2].val);v7=(yyvsp[0].val);v8=dispatch3(opassign,v5,v6,v7);(yyval.val)=v8;}
		    }
#line 7606 "ripper.c"
    break;

  case 48: /* command_asgn: defn_head f_opt_paren_args '=' command  */
#line 1663 "ripper.y"
                    {
			endless_method_name(p, (yyvsp[-3].node), &(yylsp[-3]));
			restore_defun(p, (yyvsp[-3].node)->nd_defn);
#if 0
			(yyval.val) = set_defun_body(p, (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[0].val);v2=Qnil;v3=Qnil;v4=Qnil;v5=dispatch4(bodystmt,v1,v2,v3,v4);(yyvsp[0].val)=v5;}
			{VALUE v1,v2,v3,v4;v1=get_value((yyvsp[-3].val));v2=(yyvsp[-2].val);v3=(yyvsp[0].val);v4=dispatch3(def,v1,v2,v3);(yyval.val)=v4;}
			local_pop(p);
		    }
#line 7621 "ripper.c"
    break;

  case 49: /* command_asgn: defn_head f_opt_paren_args '=' command "`rescue' modifier" arg  */
#line 1674 "ripper.y"
                    {
			endless_method_name(p, (yyvsp[-5].node), &(yylsp[-5]));
			restore_defun(p, (yyvsp[-5].node)->nd_defn);
#if 0
			(yyvsp[-2].val) = rescued_expr(p, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-2]), &(yylsp[-1]), &(yylsp[0]));
			(yyval.val) = set_defun_body(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-2].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(rescue_mod,v1,v2);v4=v3;v5=Qnil;v6=Qnil;v7=Qnil;v8=dispatch4(bodystmt,v4,v5,v6,v7);(yyvsp[-2].val)=v8;}
			{VALUE v1,v2,v3,v4;v1=get_value((yyvsp[-5].val));v2=(yyvsp[-4].val);v3=(yyvsp[-2].val);v4=dispatch3(def,v1,v2,v3);(yyval.val)=v4;}
			local_pop(p);
		    }
#line 7637 "ripper.c"
    break;

  case 50: /* command_asgn: defs_head f_opt_paren_args '=' command  */
#line 1686 "ripper.y"
                    {
			endless_method_name(p, (yyvsp[-3].node), &(yylsp[-3]));
			restore_defun(p, (yyvsp[-3].node)->nd_defn);
#if 0
			(yyval.val) = set_defun_body(p, (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			(yyvsp[-3].val) = get_value((yyvsp[-3].val));

			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[0].val);v2=Qnil;v3=Qnil;v4=Qnil;v5=dispatch4(bodystmt,v1,v2,v3,v4);(yyvsp[0].val)=v5;}
			{VALUE v1,v2,v3,v4,v5,v6;v1=AREF((yyvsp[-3].val), 0);v2=AREF((yyvsp[-3].val), 1);v3=AREF((yyvsp[-3].val), 2);v4=(yyvsp[-2].val);v5=(yyvsp[0].val);v6=dispatch5(defs,v1,v2,v3,v4,v5);(yyval.val)=v6;}
			local_pop(p);
		    }
#line 7654 "ripper.c"
    break;

  case 51: /* command_asgn: defs_head f_opt_paren_args '=' command "`rescue' modifier" arg  */
#line 1699 "ripper.y"
                    {
			endless_method_name(p, (yyvsp[-5].node), &(yylsp[-5]));
			restore_defun(p, (yyvsp[-5].node)->nd_defn);
#if 0
			(yyvsp[-2].val) = rescued_expr(p, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-2]), &(yylsp[-1]), &(yylsp[0]));
			(yyval.val) = set_defun_body(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-2].val), &(yyloc));
#endif
			(yyvsp[-5].val) = get_value((yyvsp[-5].val));

			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(rescue_mod,v1,v2);v4=v3;v5=Qnil;v6=Qnil;v7=Qnil;v8=dispatch4(bodystmt,v4,v5,v6,v7);(yyvsp[-2].val)=v8;}
			{VALUE v1,v2,v3,v4,v5,v6;v1=AREF((yyvsp[-5].val), 0);v2=AREF((yyvsp[-5].val), 1);v3=AREF((yyvsp[-5].val), 2);v4=(yyvsp[-4].val);v5=(yyvsp[-2].val);v6=dispatch5(defs,v1,v2,v3,v4,v5);(yyval.val)=v6;}
			local_pop(p);
		    }
#line 7672 "ripper.c"
    break;

  case 52: /* command_asgn: backref "operator-assignment" lex_ctxt command_rhs  */
#line 1713 "ripper.y"
                    {
#if 0
			rb_backref_error(p, (yyvsp[-3].val));
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=var_field(p, (yyvsp[-3].val));v2=(yyvsp[0].val);v3=dispatch2(assign,v1,v2);(yyval.val)=backref_error(p, RNODE((yyvsp[-3].val)), v3);}ripper_error(p);
		    }
#line 7684 "ripper.c"
    break;

  case 53: /* command_rhs: command_call  */
#line 1723 "ripper.y"
                    {
			value_expr((yyvsp[0].val));
			(yyval.val) = (yyvsp[0].val);
		    }
#line 7693 "ripper.c"
    break;

  case 54: /* command_rhs: command_call "`rescue' modifier" stmt  */
#line 1728 "ripper.y"
                    {
#if 0
			YYLTYPE loc = code_loc_gen(&(yylsp[-1]), &(yylsp[0]));
			value_expr((yyvsp[-2].val));
			(yyval.val) = NEW_RESCUE((yyvsp[-2].val), NEW_RESBODY(0, remove_begin((yyvsp[0].val)), 0, &loc), 0, &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(rescue_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 7706 "ripper.c"
    break;

  case 57: /* expr: expr "`and'" expr  */
#line 1741 "ripper.y"
                    {
			(yyval.val) = logop(p, idAND, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 7714 "ripper.c"
    break;

  case 58: /* expr: expr "`or'" expr  */
#line 1745 "ripper.y"
                    {
			(yyval.val) = logop(p, idOR, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 7722 "ripper.c"
    break;

  case 59: /* expr: "`not'" opt_nl expr  */
#line 1749 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, method_cond(p, (yyvsp[0].val), &(yylsp[0])), METHOD_NOT, &(yylsp[-2]), &(yyloc));
		    }
#line 7730 "ripper.c"
    break;

  case 60: /* expr: '!' command_call  */
#line 1753 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, method_cond(p, (yyvsp[0].val), &(yylsp[0])), '!', &(yylsp[-1]), &(yyloc));
		    }
#line 7738 "ripper.c"
    break;

  case 61: /* @5: %empty  */
#line 1757 "ripper.y"
                    {
			value_expr((yyvsp[-1].val));
			SET_LEX_STATE(EXPR_BEG|EXPR_LABEL);
			p->command_start = FALSE;
			(yyvsp[0].ctxt) = p->ctxt;
			p->ctxt.in_kwarg = 1;
			(yyval.tbl) = push_pvtbl(p);
		    }
#line 7751 "ripper.c"
    break;

  case 62: /* @6: %empty  */
#line 1765 "ripper.y"
                    {
			(yyval.tbl) = push_pktbl(p);
		    }
#line 7759 "ripper.c"
    break;

  case 63: /* expr: arg "=>" @5 @6 p_top_expr_body  */
#line 1769 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-1].tbl));
			pop_pvtbl(p, (yyvsp[-2].tbl));
			p->ctxt.in_kwarg = (yyvsp[-3].ctxt).in_kwarg;
#if 0
			(yyval.val) = NEW_CASE3((yyvsp[-4].val), NEW_IN((yyvsp[0].val), 0, 0, &(yylsp[0])), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[0].val);v2=Qnil;v3=Qnil;v4=dispatch3(in,v1,v2,v3);v5=(yyvsp[-4].val);v6=v4;v7=dispatch2(case,v5,v6);(yyval.val)=v7;}
		    }
#line 7773 "ripper.c"
    break;

  case 64: /* @7: %empty  */
#line 1779 "ripper.y"
                    {
			value_expr((yyvsp[-1].val));
			SET_LEX_STATE(EXPR_BEG|EXPR_LABEL);
			p->command_start = FALSE;
			(yyvsp[0].ctxt) = p->ctxt;
			p->ctxt.in_kwarg = 1;
			(yyval.tbl) = push_pvtbl(p);
		    }
#line 7786 "ripper.c"
    break;

  case 65: /* @8: %empty  */
#line 1787 "ripper.y"
                    {
			(yyval.tbl) = push_pktbl(p);
		    }
#line 7794 "ripper.c"
    break;

  case 66: /* expr: arg "`in'" @7 @8 p_top_expr_body  */
#line 1791 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-1].tbl));
			pop_pvtbl(p, (yyvsp[-2].tbl));
			p->ctxt.in_kwarg = (yyvsp[-3].ctxt).in_kwarg;
#if 0
			(yyval.val) = NEW_CASE3((yyvsp[-4].val), NEW_IN((yyvsp[0].val), NEW_TRUE(&(yylsp[0])), NEW_FALSE(&(yylsp[0])), &(yylsp[0])), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[0].val);v2=Qnil;v3=Qnil;v4=dispatch3(in,v1,v2,v3);v5=(yyvsp[-4].val);v6=v4;v7=dispatch2(case,v5,v6);(yyval.val)=v7;}
		    }
#line 7808 "ripper.c"
    break;

  case 68: /* def_name: fname  */
#line 1804 "ripper.y"
                    {
			ID fname = get_id((yyvsp[0].val));
			ID cur_arg = p->cur_arg;
			YYSTYPE c = {.ctxt = p->ctxt};
			numparam_name(p, fname);
			local_push(p, 0);
			p->cur_arg = 0;
			p->ctxt.in_def = 1;
			(yyval.node) = NEW_NODE(NODE_SELF, /*vid*/cur_arg, /*mid*/fname, /*cval*/c.val, &(yyloc));
#if 0
#endif
			(yyval.val) = NEW_RIPPER(fname, get_value((yyvsp[0].val)), (yyval.val), &NULL_LOC);

		    }
#line 7827 "ripper.c"
    break;

  case 69: /* defn_head: k_def def_name  */
#line 1821 "ripper.y"
                    {
			(yyval.val) = (yyvsp[0].val);
#if 0
			(yyval.val) = NEW_NODE(NODE_DEFN, 0, (yyval.val)->nd_mid, (yyval.val), &(yyloc));
#endif
		    }
#line 7838 "ripper.c"
    break;

  case 70: /* $@9: %empty  */
#line 1830 "ripper.y"
                    {
			SET_LEX_STATE(EXPR_FNAME);
			p->ctxt.in_argdef = 1;
		    }
#line 7847 "ripper.c"
    break;

  case 71: /* defs_head: k_def singleton dot_or_colon $@9 def_name  */
#line 1835 "ripper.y"
                    {
			SET_LEX_STATE(EXPR_ENDFN|EXPR_LABEL); /* force for args */
			(yyval.val) = (yyvsp[0].val);
#if 0
			(yyval.val) = NEW_NODE(NODE_DEFS, (yyvsp[-3].val), (yyval.val)->nd_mid, (yyval.val), &(yyloc));
#endif
			VALUE ary = rb_ary_new_from_args(3, (yyvsp[-3].val), (yyvsp[-2].val), get_value((yyval.val)));
			add_mark_object(p, ary);
			(yyval.node)->nd_rval = ary;

		    }
#line 7863 "ripper.c"
    break;

  case 72: /* expr_value: expr  */
#line 1849 "ripper.y"
                    {
			value_expr((yyvsp[0].val));
			(yyval.val) = (yyvsp[0].val);
		    }
#line 7872 "ripper.c"
    break;

  case 73: /* $@10: %empty  */
#line 1855 "ripper.y"
                  {COND_PUSH(1);}
#line 7878 "ripper.c"
    break;

  case 74: /* $@11: %empty  */
#line 1855 "ripper.y"
                                                {COND_POP();}
#line 7884 "ripper.c"
    break;

  case 75: /* expr_value_do: $@10 expr_value do $@11  */
#line 1856 "ripper.y"
                    {
			(yyval.val) = (yyvsp[-2].val);
		    }
#line 7892 "ripper.c"
    break;

  case 79: /* block_command: block_call call_op2 operation2 command_args  */
#line 1867 "ripper.y"
                    {
#if 0
			(yyval.val) = new_qcall(p, (yyvsp[-2].val), (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-3].val);v2=(yyvsp[-2].val);v3=(yyvsp[-1].val);v4=dispatch3(call,v1,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=dispatch2(method_add_arg,v5,v6);(yyval.val)=v7;}
		    }
#line 7903 "ripper.c"
    break;

  case 80: /* cmd_brace_block: "{ arg" brace_body '}'  */
#line 1876 "ripper.y"
                    {
			(yyval.val) = (yyvsp[-1].val);
#if 0
			(yyval.val)->nd_body->nd_loc = code_loc_gen(&(yylsp[-2]), &(yylsp[0]));
			nd_set_line((yyval.val), (yylsp[-2]).end_pos.lineno);
#endif
		    }
#line 7915 "ripper.c"
    break;

  case 81: /* fcall: operation  */
#line 1886 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_FCALL((yyvsp[0].val), 0, &(yyloc));
			nd_set_line((yyval.val), p->tokline);
#endif
			(yyval.val)=(yyvsp[0].val);
		    }
#line 7927 "ripper.c"
    break;

  case 82: /* command: fcall command_args  */
#line 1896 "ripper.y"
                    {
#if 0
			(yyvsp[-1].val)->nd_args = (yyvsp[0].val);
			nd_set_last_loc((yyvsp[-1].val), (yylsp[0]).end_pos);
			(yyval.val) = (yyvsp[-1].val);
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=(yyvsp[0].val);v3=dispatch2(command,v1,v2);(yyval.val)=v3;}
		    }
#line 7940 "ripper.c"
    break;

  case 83: /* command: fcall command_args cmd_brace_block  */
#line 1905 "ripper.y"
                    {
#if 0
			block_dup_check(p, (yyvsp[-1].val), (yyvsp[0].val));
			(yyvsp[-2].val)->nd_args = (yyvsp[-1].val);
			(yyval.val) = method_add_block(p, (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-2].val));
			nd_set_last_loc((yyvsp[-2].val), (yylsp[-1]).end_pos);
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=dispatch2(command,v1,v2);v4=v3;v5=(yyvsp[0].val);v6=dispatch2(method_add_block,v4,v5);(yyval.val)=v6;}
		    }
#line 7955 "ripper.c"
    break;

  case 84: /* command: primary_value call_op operation2 command_args  */
#line 1916 "ripper.y"
                    {
#if 0
			(yyval.val) = new_command_qcall(p, (yyvsp[-2].val), (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), Qnull, &(yylsp[-1]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[-3].val);v2=(yyvsp[-2].val);v3=(yyvsp[-1].val);v4=(yyvsp[0].val);v5=dispatch4(command_call,v1,v2,v3,v4);(yyval.val)=v5;}
		    }
#line 7966 "ripper.c"
    break;

  case 85: /* command: primary_value call_op operation2 command_args cmd_brace_block  */
#line 1923 "ripper.y"
                    {
#if 0
			(yyval.val) = new_command_qcall(p, (yyvsp[-3].val), (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-2]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-4].val);v2=(yyvsp[-3].val);v3=(yyvsp[-2].val);v4=(yyvsp[-1].val);v5=dispatch4(command_call,v1,v2,v3,v4);v6=v5;v7=(yyvsp[0].val);v8=dispatch2(method_add_block,v6,v7);(yyval.val)=v8;}
		    }
#line 7977 "ripper.c"
    break;

  case 86: /* command: primary_value "::" operation2 command_args  */
#line 1930 "ripper.y"
                    {
#if 0
			(yyval.val) = new_command_qcall(p, ID2VAL(idCOLON2), (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), Qnull, &(yylsp[-1]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[-3].val);v2=ID2VAL(idCOLON2);v3=(yyvsp[-1].val);v4=(yyvsp[0].val);v5=dispatch4(command_call,v1,v2,v3,v4);(yyval.val)=v5;}
		    }
#line 7988 "ripper.c"
    break;

  case 87: /* command: primary_value "::" operation2 command_args cmd_brace_block  */
#line 1937 "ripper.y"
                    {
#if 0
			(yyval.val) = new_command_qcall(p, ID2VAL(idCOLON2), (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-2]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-4].val);v2=ID2VAL(idCOLON2);v3=(yyvsp[-2].val);v4=(yyvsp[-1].val);v5=dispatch4(command_call,v1,v2,v3,v4);v6=v5;v7=(yyvsp[0].val);v8=dispatch2(method_add_block,v6,v7);(yyval.val)=v8;}
		   }
#line 7999 "ripper.c"
    break;

  case 88: /* command: "`super'" command_args  */
#line 1944 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_SUPER((yyvsp[0].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[0].val));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(super,v1);(yyval.val)=v2;}
		    }
#line 8011 "ripper.c"
    break;

  case 89: /* command: "`yield'" command_args  */
#line 1952 "ripper.y"
                    {
#if 0
			(yyval.val) = new_yield(p, (yyvsp[0].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[0].val));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(yield,v1);(yyval.val)=v2;}
		    }
#line 8023 "ripper.c"
    break;

  case 90: /* command: k_return call_args  */
#line 1960 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_RETURN(ret_args(p, (yyvsp[0].val)), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(return,v1);(yyval.val)=v2;}
		    }
#line 8034 "ripper.c"
    break;

  case 91: /* command: "`break'" call_args  */
#line 1967 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_BREAK(ret_args(p, (yyvsp[0].val)), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(break,v1);(yyval.val)=v2;}
		    }
#line 8045 "ripper.c"
    break;

  case 92: /* command: "`next'" call_args  */
#line 1974 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_NEXT(ret_args(p, (yyvsp[0].val)), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(next,v1);(yyval.val)=v2;}
		    }
#line 8056 "ripper.c"
    break;

  case 94: /* mlhs: "(" mlhs_inner rparen  */
#line 1984 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val);
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(mlhs_paren,v1);(yyval.val)=v2;}
		    }
#line 8067 "ripper.c"
    break;

  case 96: /* mlhs_inner: "(" mlhs_inner rparen  */
#line 1994 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN(NEW_LIST((yyvsp[-1].val), &(yyloc)), 0, &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(mlhs_paren,v1);(yyval.val)=v2;}
		    }
#line 8078 "ripper.c"
    break;

  case 97: /* mlhs_basic: mlhs_head  */
#line 2003 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN((yyvsp[0].val), 0, &(yyloc));
#endif
			(yyval.val)=(yyvsp[0].val);
		    }
#line 8089 "ripper.c"
    break;

  case 98: /* mlhs_basic: mlhs_head mlhs_item  */
#line 2010 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN(list_append(p, (yyvsp[-1].val),(yyvsp[0].val)), 0, &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=(yyvsp[0].val);v3=dispatch2(mlhs_add,v1,v2);(yyval.val)=v3;}
		    }
#line 8100 "ripper.c"
    break;

  case 99: /* mlhs_basic: mlhs_head "*" mlhs_node  */
#line 2017 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN((yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(mlhs_add_star,v1,v2);(yyval.val)=v3;}
		    }
#line 8111 "ripper.c"
    break;

  case 100: /* mlhs_basic: mlhs_head "*" mlhs_node ',' mlhs_post  */
#line 2024 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN((yyvsp[-4].val), NEW_POSTARG((yyvsp[-2].val),(yyvsp[0].val),&(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=(yyvsp[-4].val);v2=(yyvsp[-2].val);v3=dispatch2(mlhs_add_star,v1,v2);v4=v3;v5=(yyvsp[0].val);v6=dispatch2(mlhs_add_post,v4,v5);(yyval.val)=v6;}
		    }
#line 8122 "ripper.c"
    break;

  case 101: /* mlhs_basic: mlhs_head "*"  */
#line 2031 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN((yyvsp[-1].val), NODE_SPECIAL_NO_NAME_REST, &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=Qnil;v3=dispatch2(mlhs_add_star,v1,v2);(yyval.val)=v3;}
		    }
#line 8133 "ripper.c"
    break;

  case 102: /* mlhs_basic: mlhs_head "*" ',' mlhs_post  */
#line 2038 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN((yyvsp[-3].val), NEW_POSTARG(NODE_SPECIAL_NO_NAME_REST, (yyvsp[0].val), &(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=(yyvsp[-3].val);v2=Qnil;v3=dispatch2(mlhs_add_star,v1,v2);v4=v3;v5=(yyvsp[0].val);v6=dispatch2(mlhs_add_post,v4,v5);(yyval.val)=v6;}
		    }
#line 8144 "ripper.c"
    break;

  case 103: /* mlhs_basic: "*" mlhs_node  */
#line 2045 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN(0, (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(mlhs_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(mlhs_add_star,v2,v3);(yyval.val)=v4;}
		    }
#line 8155 "ripper.c"
    break;

  case 104: /* mlhs_basic: "*" mlhs_node ',' mlhs_post  */
#line 2052 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN(0, NEW_POSTARG((yyvsp[-2].val),(yyvsp[0].val),&(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=dispatch0(mlhs_new);v2=v1;v3=(yyvsp[-2].val);v4=dispatch2(mlhs_add_star,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=dispatch2(mlhs_add_post,v5,v6);(yyval.val)=v7;}
		    }
#line 8166 "ripper.c"
    break;

  case 105: /* mlhs_basic: "*"  */
#line 2059 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN(0, NODE_SPECIAL_NO_NAME_REST, &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(mlhs_new);v2=v1;v3=Qnil;v4=dispatch2(mlhs_add_star,v2,v3);(yyval.val)=v4;}
		    }
#line 8177 "ripper.c"
    break;

  case 106: /* mlhs_basic: "*" ',' mlhs_post  */
#line 2066 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN(0, NEW_POSTARG(NODE_SPECIAL_NO_NAME_REST, (yyvsp[0].val), &(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=dispatch0(mlhs_new);v2=v1;v3=Qnil;v4=dispatch2(mlhs_add_star,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=dispatch2(mlhs_add_post,v5,v6);(yyval.val)=v7;}
		    }
#line 8188 "ripper.c"
    break;

  case 108: /* mlhs_item: "(" mlhs_inner rparen  */
#line 2076 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val);
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(mlhs_paren,v1);(yyval.val)=v2;}
		    }
#line 8199 "ripper.c"
    break;

  case 109: /* mlhs_head: mlhs_item ','  */
#line 2085 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_LIST((yyvsp[-1].val), &(yylsp[-1]));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(mlhs_new);v2=v1;v3=(yyvsp[-1].val);v4=dispatch2(mlhs_add,v2,v3);(yyval.val)=v4;}
		    }
#line 8210 "ripper.c"
    break;

  case 110: /* mlhs_head: mlhs_head mlhs_item ','  */
#line 2092 "ripper.y"
                    {
#if 0
			(yyval.val) = list_append(p, (yyvsp[-2].val), (yyvsp[-1].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=dispatch2(mlhs_add,v1,v2);(yyval.val)=v3;}
		    }
#line 8221 "ripper.c"
    break;

  case 111: /* mlhs_post: mlhs_item  */
#line 2101 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_LIST((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(mlhs_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(mlhs_add,v2,v3);(yyval.val)=v4;}
		    }
#line 8232 "ripper.c"
    break;

  case 112: /* mlhs_post: mlhs_post ',' mlhs_item  */
#line 2108 "ripper.y"
                    {
#if 0
			(yyval.val) = list_append(p, (yyvsp[-2].val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(mlhs_add,v1,v2);(yyval.val)=v3;}
		    }
#line 8243 "ripper.c"
    break;

  case 113: /* mlhs_node: user_variable  */
#line 2117 "ripper.y"
                    {
#if 0
			(yyval.val) = assignable(p, (yyvsp[0].val), 0, &(yyloc));
#endif
			(yyval.val)=assignable(p, var_field(p, (yyvsp[0].val)));
		    }
#line 8254 "ripper.c"
    break;

  case 114: /* mlhs_node: keyword_variable  */
#line 2124 "ripper.y"
                    {
#if 0
			(yyval.val) = assignable(p, (yyvsp[0].val), 0, &(yyloc));
#endif
			(yyval.val)=assignable(p, var_field(p, (yyvsp[0].val)));
		    }
#line 8265 "ripper.c"
    break;

  case 115: /* mlhs_node: primary_value '[' opt_call_args rbracket  */
#line 2131 "ripper.y"
                    {
#if 0
			(yyval.val) = aryset(p, (yyvsp[-3].val), (yyvsp[-1].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=escape_Qundef((yyvsp[-1].val));v3=dispatch2(aref_field,v1,v2);(yyval.val)=v3;}
		    }
#line 8276 "ripper.c"
    break;

  case 116: /* mlhs_node: primary_value call_op "local variable or method"  */
#line 2138 "ripper.y"
                    {
			if ((yyvsp[-1].val) == tANDDOT) {
			    yyerror1(&(yylsp[-1]), "&. inside multiple assignment destination");
			}
#if 0
			(yyval.val) = attrset(p, (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=(yyvsp[0].val);v4=dispatch3(field,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 8290 "ripper.c"
    break;

  case 117: /* mlhs_node: primary_value "::" "local variable or method"  */
#line 2148 "ripper.y"
                    {
#if 0
			(yyval.val) = attrset(p, (yyvsp[-2].val), idCOLON2, (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(const_path_field,v1,v2);(yyval.val)=v3;}
		    }
#line 8301 "ripper.c"
    break;

  case 118: /* mlhs_node: primary_value call_op "constant"  */
#line 2155 "ripper.y"
                    {
			if ((yyvsp[-1].val) == tANDDOT) {
			    yyerror1(&(yylsp[-1]), "&. inside multiple assignment destination");
			}
#if 0
			(yyval.val) = attrset(p, (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=(yyvsp[0].val);v4=dispatch3(field,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 8315 "ripper.c"
    break;

  case 119: /* mlhs_node: primary_value "::" "constant"  */
#line 2165 "ripper.y"
                    {
#if 0
			(yyval.val) = const_decl(p, NEW_COLON2((yyvsp[-2].val), (yyvsp[0].val), &(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(const_path_field,v1,v2);(yyval.val)=const_decl(p, v3);}
		    }
#line 8326 "ripper.c"
    break;

  case 120: /* mlhs_node: ":: at EXPR_BEG" "constant"  */
#line 2172 "ripper.y"
                    {
#if 0
			(yyval.val) = const_decl(p, NEW_COLON3((yyvsp[0].val), &(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(top_const_field,v1);(yyval.val)=const_decl(p, v2);}
		    }
#line 8337 "ripper.c"
    break;

  case 121: /* mlhs_node: backref  */
#line 2179 "ripper.y"
                    {
#if 0
			rb_backref_error(p, (yyvsp[0].val));
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			(yyval.val)=backref_error(p, RNODE((yyvsp[0].val)), var_field(p, (yyvsp[0].val)));ripper_error(p);
		    }
#line 8349 "ripper.c"
    break;

  case 122: /* lhs: user_variable  */
#line 2189 "ripper.y"
                    {
#if 0
			(yyval.val) = assignable(p, (yyvsp[0].val), 0, &(yyloc));
#endif
			(yyval.val)=assignable(p, var_field(p, (yyvsp[0].val)));
		    }
#line 8360 "ripper.c"
    break;

  case 123: /* lhs: keyword_variable  */
#line 2196 "ripper.y"
                    {
#if 0
			(yyval.val) = assignable(p, (yyvsp[0].val), 0, &(yyloc));
#endif
			(yyval.val)=assignable(p, var_field(p, (yyvsp[0].val)));
		    }
#line 8371 "ripper.c"
    break;

  case 124: /* lhs: primary_value '[' opt_call_args rbracket  */
#line 2203 "ripper.y"
                    {
#if 0
			(yyval.val) = aryset(p, (yyvsp[-3].val), (yyvsp[-1].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=escape_Qundef((yyvsp[-1].val));v3=dispatch2(aref_field,v1,v2);(yyval.val)=v3;}
		    }
#line 8382 "ripper.c"
    break;

  case 125: /* lhs: primary_value call_op "local variable or method"  */
#line 2210 "ripper.y"
                    {
#if 0
			(yyval.val) = attrset(p, (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=(yyvsp[0].val);v4=dispatch3(field,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 8393 "ripper.c"
    break;

  case 126: /* lhs: primary_value "::" "local variable or method"  */
#line 2217 "ripper.y"
                    {
#if 0
			(yyval.val) = attrset(p, (yyvsp[-2].val), idCOLON2, (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-2].val);v2=ID2VAL(idCOLON2);v3=(yyvsp[0].val);v4=dispatch3(field,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 8404 "ripper.c"
    break;

  case 127: /* lhs: primary_value call_op "constant"  */
#line 2224 "ripper.y"
                    {
#if 0
			(yyval.val) = attrset(p, (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=(yyvsp[0].val);v4=dispatch3(field,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 8415 "ripper.c"
    break;

  case 128: /* lhs: primary_value "::" "constant"  */
#line 2231 "ripper.y"
                    {
#if 0
			(yyval.val) = const_decl(p, NEW_COLON2((yyvsp[-2].val), (yyvsp[0].val), &(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(const_path_field,v1,v2);(yyval.val)=const_decl(p, v3);}
		    }
#line 8426 "ripper.c"
    break;

  case 129: /* lhs: ":: at EXPR_BEG" "constant"  */
#line 2238 "ripper.y"
                    {
#if 0
			(yyval.val) = const_decl(p, NEW_COLON3((yyvsp[0].val), &(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(top_const_field,v1);(yyval.val)=const_decl(p, v2);}
		    }
#line 8437 "ripper.c"
    break;

  case 130: /* lhs: backref  */
#line 2245 "ripper.y"
                    {
#if 0
			rb_backref_error(p, (yyvsp[0].val));
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			(yyval.val)=backref_error(p, RNODE((yyvsp[0].val)), var_field(p, (yyvsp[0].val)));ripper_error(p);
		    }
#line 8449 "ripper.c"
    break;

  case 131: /* cname: "local variable or method"  */
#line 2255 "ripper.y"
                    {
			static const char mesg[] = "class/module name must be CONSTANT";
#if 0
			yyerror1(&(yylsp[0]), mesg);
#endif
			{VALUE v1,v2,v3;v1=ERR_MESG();v2=(yyvsp[0].val);v3=dispatch2(class_name_error,v1,v2);(yyval.val)=v3;}ripper_error(p);
		    }
#line 8461 "ripper.c"
    break;

  case 133: /* cpath: ":: at EXPR_BEG" cname  */
#line 2266 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_COLON3((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(top_const_ref,v1);(yyval.val)=v2;}
		    }
#line 8472 "ripper.c"
    break;

  case 134: /* cpath: cname  */
#line 2273 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_COLON2(0, (yyval.val), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(const_ref,v1);(yyval.val)=v2;}
		    }
#line 8483 "ripper.c"
    break;

  case 135: /* cpath: primary_value "::" cname  */
#line 2280 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_COLON2((yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(const_path_ref,v1,v2);(yyval.val)=v3;}
		    }
#line 8494 "ripper.c"
    break;

  case 139: /* fname: op  */
#line 2292 "ripper.y"
                    {
			SET_LEX_STATE(EXPR_ENDFN);
			(yyval.val) = (yyvsp[0].val);
		    }
#line 8503 "ripper.c"
    break;

  case 141: /* fitem: fname  */
#line 2300 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_LIT(ID2SYM((yyvsp[0].val)), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(symbol_literal,v1);(yyval.val)=v2;}
		    }
#line 8514 "ripper.c"
    break;

  case 143: /* undef_list: fitem  */
#line 2310 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_UNDEF((yyvsp[0].val), &(yyloc));
#endif
			(yyval.val)=rb_ary_new3(1, get_value((yyvsp[0].val)));
		    }
#line 8525 "ripper.c"
    break;

  case 144: /* $@12: %empty  */
#line 2316 "ripper.y"
                                 {SET_LEX_STATE(EXPR_FNAME|EXPR_FITEM);}
#line 8531 "ripper.c"
    break;

  case 145: /* undef_list: undef_list ',' $@12 fitem  */
#line 2317 "ripper.y"
                    {
#if 0
			NODE *undef = NEW_UNDEF((yyvsp[0].val), &(yylsp[0]));
			(yyval.val) = block_append(p, (yyvsp[-3].val), undef);
#endif
			(yyval.val)=rb_ary_push((yyvsp[-3].val), get_value((yyvsp[0].val)));
		    }
#line 8543 "ripper.c"
    break;

  case 146: /* op: '|'  */
#line 2326 "ripper.y"
                                { ifndef_ripper((yyval.val) = '|'); }
#line 8549 "ripper.c"
    break;

  case 147: /* op: '^'  */
#line 2327 "ripper.y"
                                { ifndef_ripper((yyval.val) = '^'); }
#line 8555 "ripper.c"
    break;

  case 148: /* op: '&'  */
#line 2328 "ripper.y"
                                { ifndef_ripper((yyval.val) = '&'); }
#line 8561 "ripper.c"
    break;

  case 149: /* op: "<=>"  */
#line 2329 "ripper.y"
                                { ifndef_ripper((yyval.val) = tCMP); }
#line 8567 "ripper.c"
    break;

  case 150: /* op: "=="  */
#line 2330 "ripper.y"
                                { ifndef_ripper((yyval.val) = tEQ); }
#line 8573 "ripper.c"
    break;

  case 151: /* op: "==="  */
#line 2331 "ripper.y"
                                { ifndef_ripper((yyval.val) = tEQQ); }
#line 8579 "ripper.c"
    break;

  case 152: /* op: "=~"  */
#line 2332 "ripper.y"
                                { ifndef_ripper((yyval.val) = tMATCH); }
#line 8585 "ripper.c"
    break;

  case 153: /* op: "!~"  */
#line 2333 "ripper.y"
                                { ifndef_ripper((yyval.val) = tNMATCH); }
#line 8591 "ripper.c"
    break;

  case 154: /* op: '>'  */
#line 2334 "ripper.y"
                                { ifndef_ripper((yyval.val) = '>'); }
#line 8597 "ripper.c"
    break;

  case 155: /* op: ">="  */
#line 2335 "ripper.y"
                                { ifndef_ripper((yyval.val) = tGEQ); }
#line 8603 "ripper.c"
    break;

  case 156: /* op: '<'  */
#line 2336 "ripper.y"
                                { ifndef_ripper((yyval.val) = '<'); }
#line 8609 "ripper.c"
    break;

  case 157: /* op: "<="  */
#line 2337 "ripper.y"
                                { ifndef_ripper((yyval.val) = tLEQ); }
#line 8615 "ripper.c"
    break;

  case 158: /* op: "!="  */
#line 2338 "ripper.y"
                                { ifndef_ripper((yyval.val) = tNEQ); }
#line 8621 "ripper.c"
    break;

  case 159: /* op: "<<"  */
#line 2339 "ripper.y"
                                { ifndef_ripper((yyval.val) = tLSHFT); }
#line 8627 "ripper.c"
    break;

  case 160: /* op: ">>"  */
#line 2340 "ripper.y"
                                { ifndef_ripper((yyval.val) = tRSHFT); }
#line 8633 "ripper.c"
    break;

  case 161: /* op: '+'  */
#line 2341 "ripper.y"
                                { ifndef_ripper((yyval.val) = '+'); }
#line 8639 "ripper.c"
    break;

  case 162: /* op: '-'  */
#line 2342 "ripper.y"
                                { ifndef_ripper((yyval.val) = '-'); }
#line 8645 "ripper.c"
    break;

  case 163: /* op: '*'  */
#line 2343 "ripper.y"
                                { ifndef_ripper((yyval.val) = '*'); }
#line 8651 "ripper.c"
    break;

  case 164: /* op: "*"  */
#line 2344 "ripper.y"
                                { ifndef_ripper((yyval.val) = '*'); }
#line 8657 "ripper.c"
    break;

  case 165: /* op: '/'  */
#line 2345 "ripper.y"
                                { ifndef_ripper((yyval.val) = '/'); }
#line 8663 "ripper.c"
    break;

  case 166: /* op: '%'  */
#line 2346 "ripper.y"
                                { ifndef_ripper((yyval.val) = '%'); }
#line 8669 "ripper.c"
    break;

  case 167: /* op: "**"  */
#line 2347 "ripper.y"
                                { ifndef_ripper((yyval.val) = tPOW); }
#line 8675 "ripper.c"
    break;

  case 168: /* op: "**arg"  */
#line 2348 "ripper.y"
                                { ifndef_ripper((yyval.val) = tDSTAR); }
#line 8681 "ripper.c"
    break;

  case 169: /* op: '!'  */
#line 2349 "ripper.y"
                                { ifndef_ripper((yyval.val) = '!'); }
#line 8687 "ripper.c"
    break;

  case 170: /* op: '~'  */
#line 2350 "ripper.y"
                                { ifndef_ripper((yyval.val) = '~'); }
#line 8693 "ripper.c"
    break;

  case 171: /* op: "unary+"  */
#line 2351 "ripper.y"
                                { ifndef_ripper((yyval.val) = tUPLUS); }
#line 8699 "ripper.c"
    break;

  case 172: /* op: "unary-"  */
#line 2352 "ripper.y"
                                { ifndef_ripper((yyval.val) = tUMINUS); }
#line 8705 "ripper.c"
    break;

  case 173: /* op: "[]"  */
#line 2353 "ripper.y"
                                { ifndef_ripper((yyval.val) = tAREF); }
#line 8711 "ripper.c"
    break;

  case 174: /* op: "[]="  */
#line 2354 "ripper.y"
                                { ifndef_ripper((yyval.val) = tASET); }
#line 8717 "ripper.c"
    break;

  case 175: /* op: '`'  */
#line 2355 "ripper.y"
                                { ifndef_ripper((yyval.val) = '`'); }
#line 8723 "ripper.c"
    break;

  case 217: /* arg: lhs '=' lex_ctxt arg_rhs  */
#line 2373 "ripper.y"
                    {
#if 0
			(yyval.val) = node_assign(p, (yyvsp[-3].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[0].val);v3=dispatch2(assign,v1,v2);(yyval.val)=v3;}
		    }
#line 8734 "ripper.c"
    break;

  case 218: /* arg: var_lhs "operator-assignment" lex_ctxt arg_rhs  */
#line 2380 "ripper.y"
                    {
#if 0
			(yyval.val) = new_op_assign(p, (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-3].val);v2=(yyvsp[-2].val);v3=(yyvsp[0].val);v4=dispatch3(opassign,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 8745 "ripper.c"
    break;

  case 219: /* arg: primary_value '[' opt_call_args rbracket "operator-assignment" lex_ctxt arg_rhs  */
#line 2387 "ripper.y"
                    {
#if 0
			(yyval.val) = new_ary_op_assign(p, (yyvsp[-6].val), (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-4]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-6].val);v2=escape_Qundef((yyvsp[-4].val));v3=dispatch2(aref_field,v1,v2);v4=v3;v5=(yyvsp[-2].val);v6=(yyvsp[0].val);v7=dispatch3(opassign,v4,v5,v6);(yyval.val)=v7;}
		    }
#line 8756 "ripper.c"
    break;

  case 220: /* arg: primary_value call_op "local variable or method" "operator-assignment" lex_ctxt arg_rhs  */
#line 2394 "ripper.y"
                    {
#if 0
			(yyval.val) = new_attr_op_assign(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-5].val);v2=(yyvsp[-4].val);v3=(yyvsp[-3].val);v4=dispatch3(field,v1,v2,v3);v5=v4;v6=(yyvsp[-2].val);v7=(yyvsp[0].val);v8=dispatch3(opassign,v5,v6,v7);(yyval.val)=v8;}
		    }
#line 8767 "ripper.c"
    break;

  case 221: /* arg: primary_value call_op "constant" "operator-assignment" lex_ctxt arg_rhs  */
#line 2401 "ripper.y"
                    {
#if 0
			(yyval.val) = new_attr_op_assign(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-5].val);v2=(yyvsp[-4].val);v3=(yyvsp[-3].val);v4=dispatch3(field,v1,v2,v3);v5=v4;v6=(yyvsp[-2].val);v7=(yyvsp[0].val);v8=dispatch3(opassign,v5,v6,v7);(yyval.val)=v8;}
		    }
#line 8778 "ripper.c"
    break;

  case 222: /* arg: primary_value "::" "local variable or method" "operator-assignment" lex_ctxt arg_rhs  */
#line 2408 "ripper.y"
                    {
#if 0
			(yyval.val) = new_attr_op_assign(p, (yyvsp[-5].val), ID2VAL(idCOLON2), (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-5].val);v2=ID2VAL(idCOLON2);v3=(yyvsp[-3].val);v4=dispatch3(field,v1,v2,v3);v5=v4;v6=(yyvsp[-2].val);v7=(yyvsp[0].val);v8=dispatch3(opassign,v5,v6,v7);(yyval.val)=v8;}
		    }
#line 8789 "ripper.c"
    break;

  case 223: /* arg: primary_value "::" "constant" "operator-assignment" lex_ctxt arg_rhs  */
#line 2415 "ripper.y"
                    {
#if 0
			YYLTYPE loc = code_loc_gen(&(yylsp[-5]), &(yylsp[-3]));
			(yyval.val) = new_const_op_assign(p, NEW_COLON2((yyvsp[-5].val), (yyvsp[-3].val), &loc), (yyvsp[-2].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-5].val);v2=(yyvsp[-3].val);v3=dispatch2(const_path_field,v1,v2);v4=v3;v5=(yyvsp[-2].val);v6=(yyvsp[0].val);v7=dispatch3(opassign,v4,v5,v6);(yyval.val)=v7;}
		    }
#line 8801 "ripper.c"
    break;

  case 224: /* arg: ":: at EXPR_BEG" "constant" "operator-assignment" lex_ctxt arg_rhs  */
#line 2423 "ripper.y"
                    {
#if 0
			YYLTYPE loc = code_loc_gen(&(yylsp[-4]), &(yylsp[-3]));
			(yyval.val) = new_const_op_assign(p, NEW_COLON3((yyvsp[-3].val), &loc), (yyvsp[-2].val), (yyvsp[0].val), (yyvsp[-1].ctxt), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=(yyvsp[-3].val);v2=dispatch1(top_const_field,v1);v3=v2;v4=(yyvsp[-2].val);v5=(yyvsp[0].val);v6=dispatch3(opassign,v3,v4,v5);(yyval.val)=v6;}
		    }
#line 8813 "ripper.c"
    break;

  case 225: /* arg: backref "operator-assignment" lex_ctxt arg_rhs  */
#line 2431 "ripper.y"
                    {
#if 0
			rb_backref_error(p, (yyvsp[-3].val));
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=var_field(p, (yyvsp[-3].val));v2=(yyvsp[-2].val);v3=(yyvsp[0].val);v4=dispatch3(opassign,v1,v2,v3);(yyval.val)=backref_error(p, RNODE((yyvsp[-3].val)), v4);}ripper_error(p);
		    }
#line 8825 "ripper.c"
    break;

  case 226: /* arg: arg ".." arg  */
#line 2439 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[-2].val));
			value_expr((yyvsp[0].val));
			(yyval.val) = NEW_DOT2((yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(dot2,v1,v2);(yyval.val)=v3;}
		    }
#line 8838 "ripper.c"
    break;

  case 227: /* arg: arg "..." arg  */
#line 2448 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[-2].val));
			value_expr((yyvsp[0].val));
			(yyval.val) = NEW_DOT3((yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(dot3,v1,v2);(yyval.val)=v3;}
		    }
#line 8851 "ripper.c"
    break;

  case 228: /* arg: arg ".."  */
#line 2457 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[-1].val));
			(yyval.val) = NEW_DOT2((yyvsp[-1].val), new_nil_at(p, &(yylsp[0]).end_pos), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=Qnil;v3=dispatch2(dot2,v1,v2);(yyval.val)=v3;}
		    }
#line 8863 "ripper.c"
    break;

  case 229: /* arg: arg "..."  */
#line 2465 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[-1].val));
			(yyval.val) = NEW_DOT3((yyvsp[-1].val), new_nil_at(p, &(yylsp[0]).end_pos), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=Qnil;v3=dispatch2(dot3,v1,v2);(yyval.val)=v3;}
		    }
#line 8875 "ripper.c"
    break;

  case 230: /* arg: "(.." arg  */
#line 2473 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[0].val));
			(yyval.val) = NEW_DOT2(new_nil_at(p, &(yylsp[-1]).beg_pos), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=Qnil;v2=(yyvsp[0].val);v3=dispatch2(dot2,v1,v2);(yyval.val)=v3;}
		    }
#line 8887 "ripper.c"
    break;

  case 231: /* arg: "(..." arg  */
#line 2481 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[0].val));
			(yyval.val) = NEW_DOT3(new_nil_at(p, &(yylsp[-1]).beg_pos), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=Qnil;v2=(yyvsp[0].val);v3=dispatch2(dot3,v1,v2);(yyval.val)=v3;}
		    }
#line 8899 "ripper.c"
    break;

  case 232: /* arg: arg '+' arg  */
#line 2489 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), '+', (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8907 "ripper.c"
    break;

  case 233: /* arg: arg '-' arg  */
#line 2493 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), '-', (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8915 "ripper.c"
    break;

  case 234: /* arg: arg '*' arg  */
#line 2497 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), '*', (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8923 "ripper.c"
    break;

  case 235: /* arg: arg '/' arg  */
#line 2501 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), '/', (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8931 "ripper.c"
    break;

  case 236: /* arg: arg '%' arg  */
#line 2505 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), '%', (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8939 "ripper.c"
    break;

  case 237: /* arg: arg "**" arg  */
#line 2509 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), idPow, (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8947 "ripper.c"
    break;

  case 238: /* arg: tUMINUS_NUM simple_numeric "**" arg  */
#line 2513 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, call_bin_op(p, (yyvsp[-2].val), idPow, (yyvsp[0].val), &(yylsp[-2]), &(yyloc)), idUMinus, &(yylsp[-3]), &(yyloc));
		    }
#line 8955 "ripper.c"
    break;

  case 239: /* arg: "unary+" arg  */
#line 2517 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, (yyvsp[0].val), idUPlus, &(yylsp[-1]), &(yyloc));
		    }
#line 8963 "ripper.c"
    break;

  case 240: /* arg: "unary-" arg  */
#line 2521 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, (yyvsp[0].val), idUMinus, &(yylsp[-1]), &(yyloc));
		    }
#line 8971 "ripper.c"
    break;

  case 241: /* arg: arg '|' arg  */
#line 2525 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), '|', (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8979 "ripper.c"
    break;

  case 242: /* arg: arg '^' arg  */
#line 2529 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), '^', (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8987 "ripper.c"
    break;

  case 243: /* arg: arg '&' arg  */
#line 2533 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), '&', (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 8995 "ripper.c"
    break;

  case 244: /* arg: arg "<=>" arg  */
#line 2537 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), idCmp, (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9003 "ripper.c"
    break;

  case 246: /* arg: arg "==" arg  */
#line 2542 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), idEq, (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9011 "ripper.c"
    break;

  case 247: /* arg: arg "===" arg  */
#line 2546 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), idEqq, (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9019 "ripper.c"
    break;

  case 248: /* arg: arg "!=" arg  */
#line 2550 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), idNeq, (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9027 "ripper.c"
    break;

  case 249: /* arg: arg "=~" arg  */
#line 2554 "ripper.y"
                    {
			(yyval.val) = match_op(p, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9035 "ripper.c"
    break;

  case 250: /* arg: arg "!~" arg  */
#line 2558 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), idNeqTilde, (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9043 "ripper.c"
    break;

  case 251: /* arg: '!' arg  */
#line 2562 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, method_cond(p, (yyvsp[0].val), &(yylsp[0])), '!', &(yylsp[-1]), &(yyloc));
		    }
#line 9051 "ripper.c"
    break;

  case 252: /* arg: '~' arg  */
#line 2566 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, (yyvsp[0].val), '~', &(yylsp[-1]), &(yyloc));
		    }
#line 9059 "ripper.c"
    break;

  case 253: /* arg: arg "<<" arg  */
#line 2570 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), idLTLT, (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9067 "ripper.c"
    break;

  case 254: /* arg: arg ">>" arg  */
#line 2574 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), idGTGT, (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9075 "ripper.c"
    break;

  case 255: /* arg: arg "&&" arg  */
#line 2578 "ripper.y"
                    {
			(yyval.val) = logop(p, idANDOP, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9083 "ripper.c"
    break;

  case 256: /* arg: arg "||" arg  */
#line 2582 "ripper.y"
                    {
			(yyval.val) = logop(p, idOROP, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9091 "ripper.c"
    break;

  case 257: /* $@13: %empty  */
#line 2585 "ripper.y"
                                         {p->ctxt.in_defined = 1;}
#line 9097 "ripper.c"
    break;

  case 258: /* arg: "`defined?'" opt_nl $@13 arg  */
#line 2586 "ripper.y"
                    {
			p->ctxt.in_defined = 0;
			(yyval.val) = new_defined(p, (yyvsp[0].val), &(yyloc));
		    }
#line 9106 "ripper.c"
    break;

  case 259: /* arg: arg '?' arg opt_nl ':' arg  */
#line 2591 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[-5].val));
			(yyval.val) = new_if(p, (yyvsp[-5].val), (yyvsp[-3].val), (yyvsp[0].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-5].val));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-5].val);v2=(yyvsp[-3].val);v3=(yyvsp[0].val);v4=dispatch3(ifop,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 9119 "ripper.c"
    break;

  case 260: /* arg: defn_head f_opt_paren_args '=' arg  */
#line 2600 "ripper.y"
                    {
			endless_method_name(p, (yyvsp[-3].node), &(yylsp[-3]));
			restore_defun(p, (yyvsp[-3].node)->nd_defn);
#if 0
			(yyval.val) = set_defun_body(p, (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[0].val);v2=Qnil;v3=Qnil;v4=Qnil;v5=dispatch4(bodystmt,v1,v2,v3,v4);(yyvsp[0].val)=v5;}
			{VALUE v1,v2,v3,v4;v1=get_value((yyvsp[-3].val));v2=(yyvsp[-2].val);v3=(yyvsp[0].val);v4=dispatch3(def,v1,v2,v3);(yyval.val)=v4;}
			local_pop(p);
		    }
#line 9134 "ripper.c"
    break;

  case 261: /* arg: defn_head f_opt_paren_args '=' arg "`rescue' modifier" arg  */
#line 2611 "ripper.y"
                    {
			endless_method_name(p, (yyvsp[-5].node), &(yylsp[-5]));
			restore_defun(p, (yyvsp[-5].node)->nd_defn);
#if 0
			(yyvsp[-2].val) = rescued_expr(p, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-2]), &(yylsp[-1]), &(yylsp[0]));
			(yyval.val) = set_defun_body(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-2].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(rescue_mod,v1,v2);v4=v3;v5=Qnil;v6=Qnil;v7=Qnil;v8=dispatch4(bodystmt,v4,v5,v6,v7);(yyvsp[-2].val)=v8;}
			{VALUE v1,v2,v3,v4;v1=get_value((yyvsp[-5].val));v2=(yyvsp[-4].val);v3=(yyvsp[-2].val);v4=dispatch3(def,v1,v2,v3);(yyval.val)=v4;}
			local_pop(p);
		    }
#line 9150 "ripper.c"
    break;

  case 262: /* arg: defs_head f_opt_paren_args '=' arg  */
#line 2623 "ripper.y"
                    {
			endless_method_name(p, (yyvsp[-3].node), &(yylsp[-3]));
			restore_defun(p, (yyvsp[-3].node)->nd_defn);
#if 0
			(yyval.val) = set_defun_body(p, (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			(yyvsp[-3].val) = get_value((yyvsp[-3].val));

			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[0].val);v2=Qnil;v3=Qnil;v4=Qnil;v5=dispatch4(bodystmt,v1,v2,v3,v4);(yyvsp[0].val)=v5;}
			{VALUE v1,v2,v3,v4,v5,v6;v1=AREF((yyvsp[-3].val), 0);v2=AREF((yyvsp[-3].val), 1);v3=AREF((yyvsp[-3].val), 2);v4=(yyvsp[-2].val);v5=(yyvsp[0].val);v6=dispatch5(defs,v1,v2,v3,v4,v5);(yyval.val)=v6;}
			local_pop(p);
		    }
#line 9167 "ripper.c"
    break;

  case 263: /* arg: defs_head f_opt_paren_args '=' arg "`rescue' modifier" arg  */
#line 2636 "ripper.y"
                    {
			endless_method_name(p, (yyvsp[-5].node), &(yylsp[-5]));
			restore_defun(p, (yyvsp[-5].node)->nd_defn);
#if 0
			(yyvsp[-2].val) = rescued_expr(p, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-2]), &(yylsp[-1]), &(yylsp[0]));
			(yyval.val) = set_defun_body(p, (yyvsp[-5].val), (yyvsp[-4].val), (yyvsp[-2].val), &(yyloc));
#endif
			(yyvsp[-5].val) = get_value((yyvsp[-5].val));

			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(rescue_mod,v1,v2);v4=v3;v5=Qnil;v6=Qnil;v7=Qnil;v8=dispatch4(bodystmt,v4,v5,v6,v7);(yyvsp[-2].val)=v8;}
			{VALUE v1,v2,v3,v4,v5,v6;v1=AREF((yyvsp[-5].val), 0);v2=AREF((yyvsp[-5].val), 1);v3=AREF((yyvsp[-5].val), 2);v4=(yyvsp[-4].val);v5=(yyvsp[-2].val);v6=dispatch5(defs,v1,v2,v3,v4,v5);(yyval.val)=v6;}
			local_pop(p);
		    }
#line 9185 "ripper.c"
    break;

  case 264: /* arg: primary  */
#line 2650 "ripper.y"
                    {
			(yyval.val) = (yyvsp[0].val);
		    }
#line 9193 "ripper.c"
    break;

  case 265: /* relop: '>'  */
#line 2655 "ripper.y"
                       {(yyval.val) = '>';}
#line 9199 "ripper.c"
    break;

  case 266: /* relop: '<'  */
#line 2656 "ripper.y"
                       {(yyval.val) = '<';}
#line 9205 "ripper.c"
    break;

  case 267: /* relop: ">="  */
#line 2657 "ripper.y"
                       {(yyval.val) = idGE;}
#line 9211 "ripper.c"
    break;

  case 268: /* relop: "<="  */
#line 2658 "ripper.y"
                       {(yyval.val) = idLE;}
#line 9217 "ripper.c"
    break;

  case 269: /* rel_expr: arg relop arg  */
#line 2662 "ripper.y"
                    {
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9225 "ripper.c"
    break;

  case 270: /* rel_expr: rel_expr relop arg  */
#line 2666 "ripper.y"
                    {
			rb_warning1("comparison '%s' after comparison", WARN_ID((yyvsp[-1].val)));
			(yyval.val) = call_bin_op(p, (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
		    }
#line 9234 "ripper.c"
    break;

  case 271: /* lex_ctxt: "escaped space"  */
#line 2673 "ripper.y"
                    {
			(yyval.ctxt) = p->ctxt;
		    }
#line 9242 "ripper.c"
    break;

  case 272: /* lex_ctxt: none  */
#line 2677 "ripper.y"
                    {
			(yyval.ctxt) = p->ctxt;
		    }
#line 9250 "ripper.c"
    break;

  case 273: /* arg_value: arg  */
#line 2683 "ripper.y"
                    {
			value_expr((yyvsp[0].val));
			(yyval.val) = (yyvsp[0].val);
		    }
#line 9259 "ripper.c"
    break;

  case 275: /* aref_args: args trailer  */
#line 2691 "ripper.y"
                    {
			(yyval.val) = (yyvsp[-1].val);
		    }
#line 9267 "ripper.c"
    break;

  case 276: /* aref_args: args ',' assocs trailer  */
#line 2695 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val) ? arg_append(p, (yyvsp[-3].val), new_hash(p, (yyvsp[-1].val), &(yylsp[-1])), &(yyloc)) : (yyvsp[-3].val);
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[-1].val);v2=dispatch1(bare_assoc_hash,v1);v3=(yyvsp[-3].val);v4=v2;v5=dispatch2(args_add,v3,v4);(yyval.val)=v5;}
		    }
#line 9278 "ripper.c"
    break;

  case 277: /* aref_args: assocs trailer  */
#line 2702 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val) ? NEW_LIST(new_hash(p, (yyvsp[-1].val), &(yylsp[-1])), &(yyloc)) : 0;
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=dispatch0(args_new);v2=(yyvsp[-1].val);v3=dispatch1(bare_assoc_hash,v2);v4=v1;v5=v3;v6=dispatch2(args_add,v4,v5);(yyval.val)=v6;}
		    }
#line 9289 "ripper.c"
    break;

  case 278: /* arg_rhs: arg  */
#line 2711 "ripper.y"
                    {
			value_expr((yyvsp[0].val));
			(yyval.val) = (yyvsp[0].val);
		    }
#line 9298 "ripper.c"
    break;

  case 279: /* arg_rhs: arg "`rescue' modifier" arg  */
#line 2716 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[-2].val));
			(yyval.val) = rescued_expr(p, (yyvsp[-2].val), (yyvsp[0].val), &(yylsp[-2]), &(yylsp[-1]), &(yylsp[0]));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(rescue_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 9310 "ripper.c"
    break;

  case 280: /* paren_args: '(' opt_call_args rparen  */
#line 2726 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val);
#endif
			{VALUE v1,v2;v1=escape_Qundef((yyvsp[-1].val));v2=dispatch1(arg_paren,v1);(yyval.val)=v2;}
		    }
#line 9321 "ripper.c"
    break;

  case 281: /* paren_args: '(' args ',' args_forward rparen  */
#line 2733 "ripper.y"
                    {
			if (!check_forwarding_args(p)) {
			    (yyval.val) = Qnone;
			}
			else {
#if 0
			    (yyval.val) = new_args_forward_call(p, (yyvsp[-3].val), &(yylsp[-1]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[-3].val);v2=(yyvsp[-1].val);v3=dispatch2(args_add,v1,v2);v4=v3;v5=dispatch1(arg_paren,v4);(yyval.val)=v5;}
			}
		    }
#line 9337 "ripper.c"
    break;

  case 282: /* paren_args: '(' args_forward rparen  */
#line 2745 "ripper.y"
                    {
			if (!check_forwarding_args(p)) {
			    (yyval.val) = Qnone;
			}
			else {
#if 0
			    (yyval.val) = new_args_forward_call(p, 0, &(yylsp[-1]), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(arg_paren,v1);(yyval.val)=v2;}
			}
		    }
#line 9353 "ripper.c"
    break;

  case 287: /* opt_call_args: args ','  */
#line 2765 "ripper.y"
                    {
		      (yyval.val) = (yyvsp[-1].val);
		    }
#line 9361 "ripper.c"
    break;

  case 288: /* opt_call_args: args ',' assocs ','  */
#line 2769 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val) ? arg_append(p, (yyvsp[-3].val), new_hash(p, (yyvsp[-1].val), &(yylsp[-1])), &(yyloc)) : (yyvsp[-3].val);
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[-1].val);v2=dispatch1(bare_assoc_hash,v1);v3=(yyvsp[-3].val);v4=v2;v5=dispatch2(args_add,v3,v4);(yyval.val)=v5;}
		    }
#line 9372 "ripper.c"
    break;

  case 289: /* opt_call_args: assocs ','  */
#line 2776 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val) ? NEW_LIST(new_hash(p, (yyvsp[-1].val), &(yylsp[-1])), &(yylsp[-1])) : 0;
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=dispatch0(args_new);v2=(yyvsp[-1].val);v3=dispatch1(bare_assoc_hash,v2);v4=v1;v5=v3;v6=dispatch2(args_add,v4,v5);(yyval.val)=v6;}
		    }
#line 9383 "ripper.c"
    break;

  case 290: /* call_args: command  */
#line 2785 "ripper.y"
                    {
#if 0
			value_expr((yyvsp[0].val));
			(yyval.val) = NEW_LIST((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(args_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(args_add,v2,v3);(yyval.val)=v4;}
		    }
#line 9395 "ripper.c"
    break;

  case 291: /* call_args: args opt_block_arg  */
#line 2793 "ripper.y"
                    {
#if 0
			(yyval.val) = arg_blk_pass((yyvsp[-1].val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=(yyvsp[0].val);v3=dispatch2(args_add_block,v1,v2);(yyval.val)=v3;}
		    }
#line 9406 "ripper.c"
    break;

  case 292: /* call_args: assocs opt_block_arg  */
#line 2800 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val) ? NEW_LIST(new_hash(p, (yyvsp[-1].val), &(yylsp[-1])), &(yylsp[-1])) : 0;
			(yyval.val) = arg_blk_pass((yyval.val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8,v9;v1=dispatch0(args_new);v2=(yyvsp[-1].val);v3=dispatch1(bare_assoc_hash,v2);v4=v1;v5=v3;v6=dispatch2(args_add,v4,v5);v7=v6;v8=(yyvsp[0].val);v9=dispatch2(args_add_block,v7,v8);(yyval.val)=v9;}
		    }
#line 9418 "ripper.c"
    break;

  case 293: /* call_args: args ',' assocs opt_block_arg  */
#line 2808 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val) ? arg_append(p, (yyvsp[-3].val), new_hash(p, (yyvsp[-1].val), &(yylsp[-1])), &(yyloc)) : (yyvsp[-3].val);
			(yyval.val) = arg_blk_pass((yyval.val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-1].val);v2=dispatch1(bare_assoc_hash,v1);v3=(yyvsp[-3].val);v4=v2;v5=dispatch2(args_add,v3,v4);v6=v5;v7=(yyvsp[0].val);v8=dispatch2(args_add_block,v6,v7);(yyval.val)=v8;}
		    }
#line 9430 "ripper.c"
    break;

  case 294: /* call_args: block_arg  */
#line 2816 "ripper.y"
                        {{VALUE v1,v2,v3,v4;v1=dispatch0(args_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(args_add_block,v2,v3);(yyval.val)=v4;}}
#line 9436 "ripper.c"
    break;

  case 295: /* $@14: %empty  */
#line 2819 "ripper.y"
                    {
			/* If call_args starts with a open paren '(' or '[',
			 * look-ahead reading of the letters calls CMDARG_PUSH(0),
			 * but the push must be done after CMDARG_PUSH(1).
			 * So this code makes them consistent by first cancelling
			 * the premature CMDARG_PUSH(0), doing CMDARG_PUSH(1),
			 * and finally redoing CMDARG_PUSH(0).
			 */
			int lookahead = 0;
			switch (yychar) {
			  case '(': case tLPAREN: case tLPAREN_ARG: case '[': case tLBRACK:
			    lookahead = 1;
			}
			if (lookahead) CMDARG_POP();
			CMDARG_PUSH(1);
			if (lookahead) CMDARG_PUSH(0);
		    }
#line 9458 "ripper.c"
    break;

  case 296: /* command_args: $@14 call_args  */
#line 2837 "ripper.y"
                    {
			/* call_args can be followed by tLBRACE_ARG (that does CMDARG_PUSH(0) in the lexer)
			 * but the push must be done after CMDARG_POP() in the parser.
			 * So this code does CMDARG_POP() to pop 0 pushed by tLBRACE_ARG,
			 * CMDARG_POP() to pop 1 pushed by command_args,
			 * and CMDARG_PUSH(0) to restore back the flag set by tLBRACE_ARG.
			 */
			int lookahead = 0;
			switch (yychar) {
			  case tLBRACE_ARG:
			    lookahead = 1;
			}
			if (lookahead) CMDARG_POP();
			CMDARG_POP();
			if (lookahead) CMDARG_PUSH(0);
			(yyval.val) = (yyvsp[0].val);
		    }
#line 9480 "ripper.c"
    break;

  case 297: /* block_arg: "&" arg_value  */
#line 2857 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_BLOCK_PASS((yyvsp[0].val), &(yyloc));
#endif
			(yyval.val)=(yyvsp[0].val);
		    }
#line 9491 "ripper.c"
    break;

  case 298: /* block_arg: "&"  */
#line 2864 "ripper.y"
                    {
#if 0
                        if (!local_id(p, ANON_BLOCK_ID)) {
                            compile_error(p, "no anonymous block parameter");
                        }
                        (yyval.val) = NEW_BLOCK_PASS(NEW_LVAR(ANON_BLOCK_ID, &(yylsp[0])), &(yyloc));
#endif
                    (yyval.val) = Qnil;

                    }
#line 9506 "ripper.c"
    break;

  case 299: /* opt_block_arg: ',' block_arg  */
#line 2877 "ripper.y"
                    {
			(yyval.val) = (yyvsp[0].val);
		    }
#line 9514 "ripper.c"
    break;

  case 300: /* opt_block_arg: none  */
#line 2881 "ripper.y"
                    {
			(yyval.val) = 0;
		    }
#line 9522 "ripper.c"
    break;

  case 301: /* args: arg_value  */
#line 2888 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_LIST((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(args_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(args_add,v2,v3);(yyval.val)=v4;}
		    }
#line 9533 "ripper.c"
    break;

  case 302: /* args: "*" arg_value  */
#line 2895 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_SPLAT((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(args_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(args_add_star,v2,v3);(yyval.val)=v4;}
		    }
#line 9544 "ripper.c"
    break;

  case 303: /* args: args ',' arg_value  */
#line 2902 "ripper.y"
                    {
#if 0
			(yyval.val) = last_arg_append(p, (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(args_add,v1,v2);(yyval.val)=v3;}
		    }
#line 9555 "ripper.c"
    break;

  case 304: /* args: args ',' "*" arg_value  */
#line 2909 "ripper.y"
                    {
#if 0
			(yyval.val) = rest_arg_append(p, (yyvsp[-3].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[0].val);v3=dispatch2(args_add_star,v1,v2);(yyval.val)=v3;}
		    }
#line 9566 "ripper.c"
    break;

  case 307: /* mrhs: args ',' arg_value  */
#line 2924 "ripper.y"
                    {
#if 0
			(yyval.val) = last_arg_append(p, (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[-2].val);v2=dispatch1(mrhs_new_from_args,v1);v3=v2;v4=(yyvsp[0].val);v5=dispatch2(mrhs_add,v3,v4);(yyval.val)=v5;}
		    }
#line 9577 "ripper.c"
    break;

  case 308: /* mrhs: args ',' "*" arg_value  */
#line 2931 "ripper.y"
                    {
#if 0
			(yyval.val) = rest_arg_append(p, (yyvsp[-3].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[-3].val);v2=dispatch1(mrhs_new_from_args,v1);v3=v2;v4=(yyvsp[0].val);v5=dispatch2(mrhs_add_star,v3,v4);(yyval.val)=v5;}
		    }
#line 9588 "ripper.c"
    break;

  case 309: /* mrhs: "*" arg_value  */
#line 2938 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_SPLAT((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(mrhs_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(mrhs_add_star,v2,v3);(yyval.val)=v4;}
		    }
#line 9599 "ripper.c"
    break;

  case 320: /* primary: "method"  */
#line 2957 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_FCALL((yyvsp[0].val), 0, &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=(yyvsp[0].val);v2=dispatch1(fcall,v1);v3=dispatch0(args_new);v4=v2;v5=v3;v6=dispatch2(method_add_arg,v4,v5);(yyval.val)=v6;}
		    }
#line 9610 "ripper.c"
    break;

  case 321: /* $@15: %empty  */
#line 2964 "ripper.y"
                    {
			CMDARG_PUSH(0);
		    }
#line 9618 "ripper.c"
    break;

  case 322: /* primary: k_begin $@15 bodystmt k_end  */
#line 2969 "ripper.y"
                    {
			CMDARG_POP();
#if 0
			set_line_body((yyvsp[-1].val), (yylsp[-3]).end_pos.lineno);
			(yyval.val) = NEW_BEGIN((yyvsp[-1].val), &(yyloc));
			nd_set_line((yyval.val), (yylsp[-3]).end_pos.lineno);
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(begin,v1);(yyval.val)=v2;}
		    }
#line 9632 "ripper.c"
    break;

  case 323: /* $@16: %empty  */
#line 2978 "ripper.y"
                              {SET_LEX_STATE(EXPR_ENDARG);}
#line 9638 "ripper.c"
    break;

  case 324: /* primary: "( arg" $@16 rparen  */
#line 2979 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_BEGIN(0, &(yyloc));
#endif
			{VALUE v1,v2;v1=0;v2=dispatch1(paren,v1);(yyval.val)=v2;}
		    }
#line 9649 "ripper.c"
    break;

  case 325: /* $@17: %empty  */
#line 2985 "ripper.y"
                                   {SET_LEX_STATE(EXPR_ENDARG);}
#line 9655 "ripper.c"
    break;

  case 326: /* primary: "( arg" stmt $@17 rparen  */
#line 2986 "ripper.y"
                    {
#if 0
			if (nd_type_p((yyvsp[-2].val), NODE_SELF)) (yyvsp[-2].val)->nd_state = 0;
			(yyval.val) = (yyvsp[-2].val);
#endif
			{VALUE v1,v2;v1=(yyvsp[-2].val);v2=dispatch1(paren,v1);(yyval.val)=v2;}
		    }
#line 9667 "ripper.c"
    break;

  case 327: /* primary: "(" compstmt ')'  */
#line 2994 "ripper.y"
                    {
#if 0
			if (nd_type_p((yyvsp[-1].val), NODE_SELF)) (yyvsp[-1].val)->nd_state = 0;
			(yyval.val) = (yyvsp[-1].val);
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(paren,v1);(yyval.val)=v2;}
		    }
#line 9679 "ripper.c"
    break;

  case 328: /* primary: primary_value "::" "constant"  */
#line 3002 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_COLON2((yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(const_path_ref,v1,v2);(yyval.val)=v3;}
		    }
#line 9690 "ripper.c"
    break;

  case 329: /* primary: ":: at EXPR_BEG" "constant"  */
#line 3009 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_COLON3((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(top_const_ref,v1);(yyval.val)=v2;}
		    }
#line 9701 "ripper.c"
    break;

  case 330: /* primary: "[" aref_args ']'  */
#line 3016 "ripper.y"
                    {
#if 0
			(yyval.val) = make_list((yyvsp[-1].val), &(yyloc));
#endif
			{VALUE v1,v2;v1=escape_Qundef((yyvsp[-1].val));v2=dispatch1(array,v1);(yyval.val)=v2;}
		    }
#line 9712 "ripper.c"
    break;

  case 331: /* primary: "{" assoc_list '}'  */
#line 3023 "ripper.y"
                    {
#if 0
			(yyval.val) = new_hash(p, (yyvsp[-1].val), &(yyloc));
			(yyval.val)->nd_brace = TRUE;
#endif
			{VALUE v1,v2;v1=escape_Qundef((yyvsp[-1].val));v2=dispatch1(hash,v1);(yyval.val)=v2;}
		    }
#line 9724 "ripper.c"
    break;

  case 332: /* primary: k_return  */
#line 3031 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_RETURN(0, &(yyloc));
#endif
			{VALUE v1;v1=dispatch0(return0);(yyval.val)=v1;}
		    }
#line 9735 "ripper.c"
    break;

  case 333: /* primary: "`yield'" '(' call_args rparen  */
#line 3038 "ripper.y"
                    {
#if 0
			(yyval.val) = new_yield(p, (yyvsp[-1].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-1].val);v2=dispatch1(paren,v1);v3=v2;v4=dispatch1(yield,v3);(yyval.val)=v4;}
		    }
#line 9746 "ripper.c"
    break;

  case 334: /* primary: "`yield'" '(' rparen  */
#line 3045 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_YIELD(0, &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5;v1=dispatch0(args_new);v2=v1;v3=dispatch1(paren,v2);v4=v3;v5=dispatch1(yield,v4);(yyval.val)=v5;}
		    }
#line 9757 "ripper.c"
    break;

  case 335: /* primary: "`yield'"  */
#line 3052 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_YIELD(0, &(yyloc));
#endif
			{VALUE v1;v1=dispatch0(yield0);(yyval.val)=v1;}
		    }
#line 9768 "ripper.c"
    break;

  case 336: /* $@18: %empty  */
#line 3058 "ripper.y"
                                             {p->ctxt.in_defined = 1;}
#line 9774 "ripper.c"
    break;

  case 337: /* primary: "`defined?'" opt_nl '(' $@18 expr rparen  */
#line 3059 "ripper.y"
                    {
			p->ctxt.in_defined = 0;
			(yyval.val) = new_defined(p, (yyvsp[-1].val), &(yyloc));
		    }
#line 9783 "ripper.c"
    break;

  case 338: /* primary: "`not'" '(' expr rparen  */
#line 3064 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, method_cond(p, (yyvsp[-1].val), &(yylsp[-1])), METHOD_NOT, &(yylsp[-3]), &(yyloc));
		    }
#line 9791 "ripper.c"
    break;

  case 339: /* primary: "`not'" '(' rparen  */
#line 3068 "ripper.y"
                    {
			(yyval.val) = call_uni_op(p, method_cond(p, new_nil(&(yylsp[-1])), &(yylsp[-1])), METHOD_NOT, &(yylsp[-2]), &(yyloc));
		    }
#line 9799 "ripper.c"
    break;

  case 340: /* primary: fcall brace_block  */
#line 3072 "ripper.y"
                    {
#if 0
			(yyval.val) = method_add_block(p, (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8,v9;v1=(yyvsp[-1].val);v2=dispatch1(fcall,v1);v3=dispatch0(args_new);v4=v2;v5=v3;v6=dispatch2(method_add_arg,v4,v5);v7=v6;v8=(yyvsp[0].val);v9=dispatch2(method_add_block,v7,v8);(yyval.val)=v9;}
		    }
#line 9810 "ripper.c"
    break;

  case 342: /* primary: method_call brace_block  */
#line 3080 "ripper.y"
                    {
#if 0
			block_dup_check(p, (yyvsp[-1].val)->nd_args, (yyvsp[0].val));
			(yyval.val) = method_add_block(p, (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=(yyvsp[0].val);v3=dispatch2(method_add_block,v1,v2);(yyval.val)=v3;}
		    }
#line 9822 "ripper.c"
    break;

  case 344: /* primary: k_if expr_value then compstmt if_tail k_end  */
#line 3092 "ripper.y"
                    {
#if 0
			(yyval.val) = new_if(p, (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[-1].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-4].val));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-4].val);v2=(yyvsp[-2].val);v3=escape_Qundef((yyvsp[-1].val));v4=dispatch3(if,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 9834 "ripper.c"
    break;

  case 345: /* primary: k_unless expr_value then compstmt opt_else k_end  */
#line 3103 "ripper.y"
                    {
#if 0
			(yyval.val) = new_unless(p, (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[-1].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-4].val));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-4].val);v2=(yyvsp[-2].val);v3=escape_Qundef((yyvsp[-1].val));v4=dispatch3(unless,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 9846 "ripper.c"
    break;

  case 346: /* primary: k_while expr_value_do compstmt k_end  */
#line 3113 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_WHILE(cond(p, (yyvsp[-2].val), &(yylsp[-2])), (yyvsp[-1].val), 1, &(yyloc));
			fixpos((yyval.val), (yyvsp[-2].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=dispatch2(while,v1,v2);(yyval.val)=v3;}
		    }
#line 9858 "ripper.c"
    break;

  case 347: /* primary: k_until expr_value_do compstmt k_end  */
#line 3123 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_UNTIL(cond(p, (yyvsp[-2].val), &(yylsp[-2])), (yyvsp[-1].val), 1, &(yyloc));
			fixpos((yyval.val), (yyvsp[-2].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=dispatch2(until,v1,v2);(yyval.val)=v3;}
		    }
#line 9870 "ripper.c"
    break;

  case 348: /* @19: %empty  */
#line 3131 "ripper.y"
                    {
			(yyval.val) = p->case_labels;
			p->case_labels = Qnil;
		    }
#line 9879 "ripper.c"
    break;

  case 349: /* primary: k_case expr_value opt_terms @19 case_body k_end  */
#line 3137 "ripper.y"
                    {
			if (RTEST(p->case_labels)) rb_hash_clear(p->case_labels);
			p->case_labels = (yyvsp[-2].val);
#if 0
			(yyval.val) = NEW_CASE((yyvsp[-4].val), (yyvsp[-1].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-4].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-4].val);v2=(yyvsp[-1].val);v3=dispatch2(case,v1,v2);(yyval.val)=v3;}
		    }
#line 9893 "ripper.c"
    break;

  case 350: /* @20: %empty  */
#line 3147 "ripper.y"
                    {
			(yyval.val) = p->case_labels;
			p->case_labels = 0;
		    }
#line 9902 "ripper.c"
    break;

  case 351: /* primary: k_case opt_terms @20 case_body k_end  */
#line 3153 "ripper.y"
                    {
			if (RTEST(p->case_labels)) rb_hash_clear(p->case_labels);
			p->case_labels = (yyvsp[-2].val);
#if 0
			(yyval.val) = NEW_CASE2((yyvsp[-1].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=Qnil;v2=(yyvsp[-1].val);v3=dispatch2(case,v1,v2);(yyval.val)=v3;}
		    }
#line 9915 "ripper.c"
    break;

  case 352: /* primary: k_case expr_value opt_terms p_case_body k_end  */
#line 3164 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_CASE3((yyvsp[-3].val), (yyvsp[-1].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[-1].val);v3=dispatch2(case,v1,v2);(yyval.val)=v3;}
		    }
#line 9926 "ripper.c"
    break;

  case 353: /* primary: k_for for_var "`in'" expr_value_do compstmt k_end  */
#line 3173 "ripper.y"
                    {
#if 0
			/*
			 *  for a, b, c in e
			 *  #=>
			 *  e.each{|*x| a, b, c = x}
			 *
			 *  for a in e
			 *  #=>
			 *  e.each{|x| a, = x}
			 */
			ID id = internal_id(p);
			NODE *m = NEW_ARGS_AUX(0, 0, &NULL_LOC);
			NODE *args, *scope, *internal_var = NEW_DVAR(id, &(yylsp[-4]));
                        rb_ast_id_table_t *tbl = rb_ast_new_local_table(p->ast, 1);
			tbl->ids[0] = id; /* internal id */

			switch (nd_type((yyvsp[-4].val))) {
			  case NODE_LASGN:
			  case NODE_DASGN: /* e.each {|internal_var| a = internal_var; ... } */
			    (yyvsp[-4].val)->nd_value = internal_var;
			    id = 0;
			    m->nd_plen = 1;
			    m->nd_next = (yyvsp[-4].val);
			    break;
			  case NODE_MASGN: /* e.each {|*internal_var| a, b, c = (internal_var.length == 1 && Array === (tmp = internal_var[0]) ? tmp : internal_var); ... } */
			    m->nd_next = node_assign(p, (yyvsp[-4].val), NEW_FOR_MASGN(internal_var, &(yylsp[-4])), NO_LEX_CTXT, &(yylsp[-4]));
			    break;
			  default: /* e.each {|*internal_var| @a, B, c[1], d.attr = internal_val; ... } */
			    m->nd_next = node_assign(p, NEW_MASGN(NEW_LIST((yyvsp[-4].val), &(yylsp[-4])), 0, &(yylsp[-4])), internal_var, NO_LEX_CTXT, &(yylsp[-4]));
			}
			/* {|*internal_id| <m> = internal_id; ... } */
			args = new_args(p, m, 0, id, 0, new_args_tail(p, 0, 0, 0, &(yylsp[-4])), &(yylsp[-4]));
			scope = NEW_NODE(NODE_SCOPE, tbl, (yyvsp[-1].val), args, &(yyloc));
			(yyval.val) = NEW_FOR((yyvsp[-2].val), scope, &(yyloc));
			fixpos((yyval.val), (yyvsp[-4].val));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-4].val);v2=(yyvsp[-2].val);v3=(yyvsp[-1].val);v4=dispatch3(for,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 9970 "ripper.c"
    break;

  case 354: /* $@21: %empty  */
#line 3213 "ripper.y"
                    {
			if (p->ctxt.in_def) {
			    YYLTYPE loc = code_loc_gen(&(yylsp[-2]), &(yylsp[-1]));
			    yyerror1(&loc, "class definition in method body");
			}
			p->ctxt.in_class = 1;
			local_push(p, 0);
		    }
#line 9983 "ripper.c"
    break;

  case 355: /* primary: k_class cpath superclass $@21 bodystmt k_end  */
#line 3223 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_CLASS((yyvsp[-4].val), (yyvsp[-1].val), (yyvsp[-3].val), &(yyloc));
			nd_set_line((yyval.val)->nd_body, (yylsp[0]).end_pos.lineno);
			set_line_body((yyvsp[-1].val), (yylsp[-3]).end_pos.lineno);
			nd_set_line((yyval.val), (yylsp[-3]).end_pos.lineno);
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-4].val);v2=(yyvsp[-3].val);v3=(yyvsp[-1].val);v4=dispatch3(class,v1,v2,v3);(yyval.val)=v4;}
			local_pop(p);
			p->ctxt.in_class = (yyvsp[-5].ctxt).in_class;
			p->ctxt.shareable_constant_value = (yyvsp[-5].ctxt).shareable_constant_value;
		    }
#line 10000 "ripper.c"
    break;

  case 356: /* $@22: %empty  */
#line 3236 "ripper.y"
                    {
			p->ctxt.in_def = 0;
			p->ctxt.in_class = 0;
			local_push(p, 0);
		    }
#line 10010 "ripper.c"
    break;

  case 357: /* primary: k_class "<<" expr $@22 term bodystmt k_end  */
#line 3244 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_SCLASS((yyvsp[-4].val), (yyvsp[-1].val), &(yyloc));
			nd_set_line((yyval.val)->nd_body, (yylsp[0]).end_pos.lineno);
			set_line_body((yyvsp[-1].val), nd_line((yyvsp[-4].val)));
			fixpos((yyval.val), (yyvsp[-4].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-4].val);v2=(yyvsp[-1].val);v3=dispatch2(sclass,v1,v2);(yyval.val)=v3;}
			local_pop(p);
			p->ctxt.in_def = (yyvsp[-6].ctxt).in_def;
			p->ctxt.in_class = (yyvsp[-6].ctxt).in_class;
			p->ctxt.shareable_constant_value = (yyvsp[-6].ctxt).shareable_constant_value;
		    }
#line 10028 "ripper.c"
    break;

  case 358: /* $@23: %empty  */
#line 3258 "ripper.y"
                    {
			if (p->ctxt.in_def) {
			    YYLTYPE loc = code_loc_gen(&(yylsp[-1]), &(yylsp[0]));
			    yyerror1(&loc, "module definition in method body");
			}
			p->ctxt.in_class = 1;
			local_push(p, 0);
		    }
#line 10041 "ripper.c"
    break;

  case 359: /* primary: k_module cpath $@23 bodystmt k_end  */
#line 3268 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MODULE((yyvsp[-3].val), (yyvsp[-1].val), &(yyloc));
			nd_set_line((yyval.val)->nd_body, (yylsp[0]).end_pos.lineno);
			set_line_body((yyvsp[-1].val), (yylsp[-3]).end_pos.lineno);
			nd_set_line((yyval.val), (yylsp[-3]).end_pos.lineno);
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[-1].val);v3=dispatch2(module,v1,v2);(yyval.val)=v3;}
			local_pop(p);
			p->ctxt.in_class = (yyvsp[-4].ctxt).in_class;
			p->ctxt.shareable_constant_value = (yyvsp[-4].ctxt).shareable_constant_value;
		    }
#line 10058 "ripper.c"
    break;

  case 360: /* primary: defn_head f_arglist bodystmt k_end  */
#line 3284 "ripper.y"
                    {
			restore_defun(p, (yyvsp[-3].node)->nd_defn);
#if 0
			(yyval.val) = set_defun_body(p, (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[-1].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=get_value((yyvsp[-3].val));v2=(yyvsp[-2].val);v3=(yyvsp[-1].val);v4=dispatch3(def,v1,v2,v3);(yyval.val)=v4;}
			local_pop(p);
		    }
#line 10071 "ripper.c"
    break;

  case 361: /* primary: defs_head f_arglist bodystmt k_end  */
#line 3296 "ripper.y"
                    {
			restore_defun(p, (yyvsp[-3].node)->nd_defn);
#if 0
			(yyval.val) = set_defun_body(p, (yyvsp[-3].val), (yyvsp[-2].val), (yyvsp[-1].val), &(yyloc));
#endif
			(yyvsp[-3].val) = get_value((yyvsp[-3].val));

			{VALUE v1,v2,v3,v4,v5,v6;v1=AREF((yyvsp[-3].val), 0);v2=AREF((yyvsp[-3].val), 1);v3=AREF((yyvsp[-3].val), 2);v4=(yyvsp[-2].val);v5=(yyvsp[-1].val);v6=dispatch5(defs,v1,v2,v3,v4,v5);(yyval.val)=v6;}
			local_pop(p);
		    }
#line 10086 "ripper.c"
    break;

  case 362: /* primary: "`break'"  */
#line 3307 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_BREAK(0, &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=dispatch0(args_new);v2=v1;v3=dispatch1(break,v2);(yyval.val)=v3;}
		    }
#line 10097 "ripper.c"
    break;

  case 363: /* primary: "`next'"  */
#line 3314 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_NEXT(0, &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=dispatch0(args_new);v2=v1;v3=dispatch1(next,v2);(yyval.val)=v3;}
		    }
#line 10108 "ripper.c"
    break;

  case 364: /* primary: "`redo'"  */
#line 3321 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_REDO(&(yyloc));
#endif
			{VALUE v1;v1=dispatch0(redo);(yyval.val)=v1;}
		    }
#line 10119 "ripper.c"
    break;

  case 365: /* primary: "`retry'"  */
#line 3328 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_RETRY(&(yyloc));
#endif
			{VALUE v1;v1=dispatch0(retry);(yyval.val)=v1;}
		    }
#line 10130 "ripper.c"
    break;

  case 366: /* primary_value: primary  */
#line 3337 "ripper.y"
                    {
			value_expr((yyvsp[0].val));
			(yyval.val) = (yyvsp[0].val);
		    }
#line 10139 "ripper.c"
    break;

  case 367: /* k_begin: "`begin'"  */
#line 3344 "ripper.y"
                    {
			token_info_push(p, "begin", &(yyloc));
		    }
#line 10147 "ripper.c"
    break;

  case 368: /* k_if: "`if'"  */
#line 3350 "ripper.y"
                    {
			WARN_EOL("if");
			token_info_push(p, "if", &(yyloc));
			if (p->token_info && p->token_info->nonspc &&
			    p->token_info->next && !strcmp(p->token_info->next->token, "else")) {
			    const char *tok = p->lex.ptok;
			    const char *beg = p->lex.pbeg + p->token_info->next->beg.column;
			    beg += rb_strlen_lit("else");
			    while (beg < tok && ISSPACE(*beg)) beg++;
			    if (beg == tok) {
				p->token_info->nonspc = 0;
			    }
			}
		    }
#line 10166 "ripper.c"
    break;

  case 369: /* k_unless: "`unless'"  */
#line 3367 "ripper.y"
                    {
			token_info_push(p, "unless", &(yyloc));
		    }
#line 10174 "ripper.c"
    break;

  case 370: /* k_while: "`while'"  */
#line 3373 "ripper.y"
                    {
			token_info_push(p, "while", &(yyloc));
		    }
#line 10182 "ripper.c"
    break;

  case 371: /* k_until: "`until'"  */
#line 3379 "ripper.y"
                    {
			token_info_push(p, "until", &(yyloc));
		    }
#line 10190 "ripper.c"
    break;

  case 372: /* k_case: "`case'"  */
#line 3385 "ripper.y"
                    {
			token_info_push(p, "case", &(yyloc));
		    }
#line 10198 "ripper.c"
    break;

  case 373: /* k_for: "`for'"  */
#line 3391 "ripper.y"
                    {
			token_info_push(p, "for", &(yyloc));
		    }
#line 10206 "ripper.c"
    break;

  case 374: /* k_class: "`class'"  */
#line 3397 "ripper.y"
                    {
			token_info_push(p, "class", &(yyloc));
			(yyval.ctxt) = p->ctxt;
		    }
#line 10215 "ripper.c"
    break;

  case 375: /* k_module: "`module'"  */
#line 3404 "ripper.y"
                    {
			token_info_push(p, "module", &(yyloc));
			(yyval.ctxt) = p->ctxt;
		    }
#line 10224 "ripper.c"
    break;

  case 376: /* k_def: "`def'"  */
#line 3411 "ripper.y"
                    {
			token_info_push(p, "def", &(yyloc));
			p->ctxt.in_argdef = 1;
		    }
#line 10233 "ripper.c"
    break;

  case 377: /* k_do: "`do'"  */
#line 3418 "ripper.y"
                    {
			token_info_push(p, "do", &(yyloc));
		    }
#line 10241 "ripper.c"
    break;

  case 378: /* k_do_block: "`do' for block"  */
#line 3424 "ripper.y"
                    {
			token_info_push(p, "do", &(yyloc));
		    }
#line 10249 "ripper.c"
    break;

  case 379: /* k_rescue: "`rescue'"  */
#line 3430 "ripper.y"
                    {
			token_info_warn(p, "rescue", p->token_info, 1, &(yyloc));
		    }
#line 10257 "ripper.c"
    break;

  case 380: /* k_ensure: "`ensure'"  */
#line 3436 "ripper.y"
                    {
			token_info_warn(p, "ensure", p->token_info, 1, &(yyloc));
		    }
#line 10265 "ripper.c"
    break;

  case 381: /* k_when: "`when'"  */
#line 3442 "ripper.y"
                    {
			token_info_warn(p, "when", p->token_info, 0, &(yyloc));
		    }
#line 10273 "ripper.c"
    break;

  case 382: /* k_else: "`else'"  */
#line 3448 "ripper.y"
                    {
			token_info *ptinfo_beg = p->token_info;
			int same = ptinfo_beg && strcmp(ptinfo_beg->token, "case") != 0;
			token_info_warn(p, "else", p->token_info, same, &(yyloc));
			if (same) {
			    token_info e;
			    e.next = ptinfo_beg->next;
			    e.token = "else";
			    token_info_setup(&e, p->lex.pbeg, &(yyloc));
			    if (!e.nonspc) *ptinfo_beg = e;
			}
		    }
#line 10290 "ripper.c"
    break;

  case 383: /* k_elsif: "`elsif'"  */
#line 3463 "ripper.y"
                    {
			WARN_EOL("elsif");
			token_info_warn(p, "elsif", p->token_info, 1, &(yyloc));
		    }
#line 10299 "ripper.c"
    break;

  case 384: /* k_end: "`end'"  */
#line 3470 "ripper.y"
                    {
			token_info_pop(p, "end", &(yyloc));
		    }
#line 10307 "ripper.c"
    break;

  case 385: /* k_return: "`return'"  */
#line 3476 "ripper.y"
                    {
			if (p->ctxt.in_class && !p->ctxt.in_def && !dyna_in_block(p))
			    yyerror1(&(yylsp[0]), "Invalid return in class/module body");
		    }
#line 10316 "ripper.c"
    break;

  case 392: /* if_tail: k_elsif expr_value then compstmt if_tail  */
#line 3495 "ripper.y"
                    {
#if 0
			(yyval.val) = new_if(p, (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-3].val));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-3].val);v2=(yyvsp[-1].val);v3=escape_Qundef((yyvsp[0].val));v4=dispatch3(elsif,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 10328 "ripper.c"
    break;

  case 394: /* opt_else: k_else compstmt  */
#line 3506 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[0].val);
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(else,v1);(yyval.val)=v2;}
		    }
#line 10339 "ripper.c"
    break;

  case 397: /* f_marg: f_norm_arg  */
#line 3519 "ripper.y"
                    {
#if 0
			(yyval.val) = assignable(p, (yyvsp[0].val), 0, &(yyloc));
			mark_lvar_used(p, (yyval.val));
#endif
			(yyval.val)=assignable(p, (yyvsp[0].val));
		    }
#line 10351 "ripper.c"
    break;

  case 398: /* f_marg: "(" f_margs rparen  */
#line 3527 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val);
#endif
			{VALUE v1,v2;v1=(yyvsp[-1].val);v2=dispatch1(mlhs_paren,v1);(yyval.val)=v2;}
		    }
#line 10362 "ripper.c"
    break;

  case 399: /* f_marg_list: f_marg  */
#line 3536 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_LIST((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(mlhs_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(mlhs_add,v2,v3);(yyval.val)=v4;}
		    }
#line 10373 "ripper.c"
    break;

  case 400: /* f_marg_list: f_marg_list ',' f_marg  */
#line 3543 "ripper.y"
                    {
#if 0
			(yyval.val) = list_append(p, (yyvsp[-2].val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(mlhs_add,v1,v2);(yyval.val)=v3;}
		    }
#line 10384 "ripper.c"
    break;

  case 401: /* f_margs: f_marg_list  */
#line 3552 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN((yyvsp[0].val), 0, &(yyloc));
#endif
			(yyval.val)=(yyvsp[0].val);
		    }
#line 10395 "ripper.c"
    break;

  case 402: /* f_margs: f_marg_list ',' f_rest_marg  */
#line 3559 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN((yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(mlhs_add_star,v1,v2);(yyval.val)=v3;}
		    }
#line 10406 "ripper.c"
    break;

  case 403: /* f_margs: f_marg_list ',' f_rest_marg ',' f_marg_list  */
#line 3566 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN((yyvsp[-4].val), NEW_POSTARG((yyvsp[-2].val), (yyvsp[0].val), &(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6;v1=(yyvsp[-4].val);v2=(yyvsp[-2].val);v3=dispatch2(mlhs_add_star,v1,v2);v4=v3;v5=(yyvsp[0].val);v6=dispatch2(mlhs_add_post,v4,v5);(yyval.val)=v6;}
		    }
#line 10417 "ripper.c"
    break;

  case 404: /* f_margs: f_rest_marg  */
#line 3573 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN(0, (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(mlhs_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(mlhs_add_star,v2,v3);(yyval.val)=v4;}
		    }
#line 10428 "ripper.c"
    break;

  case 405: /* f_margs: f_rest_marg ',' f_marg_list  */
#line 3580 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_MASGN(0, NEW_POSTARG((yyvsp[-2].val), (yyvsp[0].val), &(yyloc)), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=dispatch0(mlhs_new);v2=v1;v3=(yyvsp[-2].val);v4=dispatch2(mlhs_add_star,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=dispatch2(mlhs_add_post,v5,v6);(yyval.val)=v7;}
		    }
#line 10439 "ripper.c"
    break;

  case 406: /* f_rest_marg: "*" f_norm_arg  */
#line 3589 "ripper.y"
                    {
#if 0
			(yyval.val) = assignable(p, (yyvsp[0].val), 0, &(yyloc));
			mark_lvar_used(p, (yyval.val));
#endif
			(yyval.val)=assignable(p, (yyvsp[0].val));
		    }
#line 10451 "ripper.c"
    break;

  case 407: /* f_rest_marg: "*"  */
#line 3597 "ripper.y"
                    {
#if 0
			(yyval.val) = NODE_SPECIAL_NO_NAME_REST;
#endif
			(yyval.val)=Qnil;
		    }
#line 10462 "ripper.c"
    break;

  case 409: /* f_any_kwrest: f_no_kwarg  */
#line 3606 "ripper.y"
                             {(yyval.val) = ID2VAL(idNil);}
#line 10468 "ripper.c"
    break;

  case 410: /* $@24: %empty  */
#line 3609 "ripper.y"
                  {p->ctxt.in_argdef = 0;}
#line 10474 "ripper.c"
    break;

  case 412: /* block_args_tail: f_block_kwarg ',' f_kwrest opt_f_block_arg  */
#line 3612 "ripper.y"
                    {
			(yyval.val) = new_args_tail(p, (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-1]));
		    }
#line 10482 "ripper.c"
    break;

  case 413: /* block_args_tail: f_block_kwarg opt_f_block_arg  */
#line 3616 "ripper.y"
                    {
			(yyval.val) = new_args_tail(p, (yyvsp[-1].val), Qnone, (yyvsp[0].val), &(yylsp[-1]));
		    }
#line 10490 "ripper.c"
    break;

  case 414: /* block_args_tail: f_any_kwrest opt_f_block_arg  */
#line 3620 "ripper.y"
                    {
			(yyval.val) = new_args_tail(p, Qnone, (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-1]));
		    }
#line 10498 "ripper.c"
    break;

  case 415: /* block_args_tail: f_block_arg  */
#line 3624 "ripper.y"
                    {
			(yyval.val) = new_args_tail(p, Qnone, Qnone, (yyvsp[0].val), &(yylsp[0]));
		    }
#line 10506 "ripper.c"
    break;

  case 416: /* opt_block_args_tail: ',' block_args_tail  */
#line 3630 "ripper.y"
                    {
			(yyval.val) = (yyvsp[0].val);
		    }
#line 10514 "ripper.c"
    break;

  case 417: /* opt_block_args_tail: %empty  */
#line 3634 "ripper.y"
                    {
			(yyval.val) = new_args_tail(p, Qnone, Qnone, Qnone, &(yylsp[0]));
		    }
#line 10522 "ripper.c"
    break;

  case 418: /* excessed_comma: ','  */
#line 3640 "ripper.y"
                    {
			/* magic number for rest_id in iseq_set_arguments() */
#if 0
			(yyval.val) = NODE_SPECIAL_EXCESSIVE_COMMA;
#endif
			{VALUE v1;v1=dispatch0(excessed_comma);(yyval.val)=v1;}
		    }
#line 10534 "ripper.c"
    break;

  case 419: /* block_param: f_arg ',' f_block_optarg ',' f_rest_arg opt_block_args_tail  */
#line 3650 "ripper.y"
                    {
			(yyval.val) = new_args(p, (yyvsp[-5].val), (yyvsp[-3].val), (yyvsp[-1].val), Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 10542 "ripper.c"
    break;

  case 420: /* block_param: f_arg ',' f_block_optarg ',' f_rest_arg ',' f_arg opt_block_args_tail  */
#line 3654 "ripper.y"
                    {
			(yyval.val) = new_args(p, (yyvsp[-7].val), (yyvsp[-5].val), (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
		    }
#line 10550 "ripper.c"
    break;

  case 421: /* block_param: f_arg ',' f_block_optarg opt_block_args_tail  */
#line 3658 "ripper.y"
                    {
			(yyval.val) = new_args(p, (yyvsp[-3].val), (yyvsp[-1].val), Qnone, Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 10558 "ripper.c"
    break;

  case 422: /* block_param: f_arg ',' f_block_optarg ',' f_arg opt_block_args_tail  */
#line 3662 "ripper.y"
                    {
			(yyval.val) = new_args(p, (yyvsp[-5].val), (yyvsp[-3].val), Qnone, (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
		    }
#line 10566 "ripper.c"
    break;

  case 423: /* block_param: f_arg ',' f_rest_arg opt_block_args_tail  */
#line 3666 "ripper.y"
                    {
			(yyval.val) = new_args(p, (yyvsp[-3].val), Qnone, (yyvsp[-1].val), Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 10574 "ripper.c"
    break;

  case 424: /* block_param: f_arg excessed_comma  */
#line 3670 "ripper.y"
                    {
			(yyval.val) = new_args_tail(p, Qnone, Qnone, Qnone, &(yylsp[0]));
			(yyval.val) = new_args(p, (yyvsp[-1].val), Qnone, (yyvsp[0].val), Qnone, (yyval.val), &(yyloc));
		    }
#line 10583 "ripper.c"
    break;

  case 425: /* block_param: f_arg ',' f_rest_arg ',' f_arg opt_block_args_tail  */
#line 3675 "ripper.y"
                    {
			(yyval.val) = new_args(p, (yyvsp[-5].val), Qnone, (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
		    }
#line 10591 "ripper.c"
    break;

  case 426: /* block_param: f_arg opt_block_args_tail  */
#line 3679 "ripper.y"
                    {
			(yyval.val) = new_args(p, (yyvsp[-1].val), Qnone, Qnone, Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 10599 "ripper.c"
    break;

  case 427: /* block_param: f_block_optarg ',' f_rest_arg opt_block_args_tail  */
#line 3683 "ripper.y"
                    {
			(yyval.val) = new_args(p, Qnone, (yyvsp[-3].val), (yyvsp[-1].val), Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 10607 "ripper.c"
    break;

  case 428: /* block_param: f_block_optarg ',' f_rest_arg ',' f_arg opt_block_args_tail  */
#line 3687 "ripper.y"
                    {
			(yyval.val) = new_args(p, Qnone, (yyvsp[-5].val), (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
		    }
#line 10615 "ripper.c"
    break;

  case 429: /* block_param: f_block_optarg opt_block_args_tail  */
#line 3691 "ripper.y"
                    {
			(yyval.val) = new_args(p, Qnone, (yyvsp[-1].val), Qnone, Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 10623 "ripper.c"
    break;

  case 430: /* block_param: f_block_optarg ',' f_arg opt_block_args_tail  */
#line 3695 "ripper.y"
                    {
			(yyval.val) = new_args(p, Qnone, (yyvsp[-3].val), Qnone, (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
		    }
#line 10631 "ripper.c"
    break;

  case 431: /* block_param: f_rest_arg opt_block_args_tail  */
#line 3699 "ripper.y"
                    {
			(yyval.val) = new_args(p, Qnone, Qnone, (yyvsp[-1].val), Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 10639 "ripper.c"
    break;

  case 432: /* block_param: f_rest_arg ',' f_arg opt_block_args_tail  */
#line 3703 "ripper.y"
                    {
			(yyval.val) = new_args(p, Qnone, Qnone, (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
		    }
#line 10647 "ripper.c"
    break;

  case 433: /* block_param: block_args_tail  */
#line 3707 "ripper.y"
                    {
			(yyval.val) = new_args(p, Qnone, Qnone, Qnone, Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 10655 "ripper.c"
    break;

  case 435: /* opt_block_param: block_param_def  */
#line 3714 "ripper.y"
                    {
			p->command_start = TRUE;
		    }
#line 10663 "ripper.c"
    break;

  case 436: /* block_param_def: '|' opt_bv_decl '|'  */
#line 3720 "ripper.y"
                    {
			p->cur_arg = 0;
			p->max_numparam = ORDINAL_PARAM;
			p->ctxt.in_argdef = 0;
#if 0
			(yyval.val) = 0;
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11;v1=Qnil;v2=Qnil;v3=Qnil;v4=Qnil;v5=Qnil;v6=Qnil;v7=Qnil;v8=dispatch7(params,v1,v2,v3,v4,v5,v6,v7);v9=v8;v10=escape_Qundef((yyvsp[-1].val));v11=dispatch2(block_var,v9,v10);(yyval.val)=v11;}
		    }
#line 10677 "ripper.c"
    break;

  case 437: /* block_param_def: '|' block_param opt_bv_decl '|'  */
#line 3730 "ripper.y"
                    {
			p->cur_arg = 0;
			p->max_numparam = ORDINAL_PARAM;
			p->ctxt.in_argdef = 0;
#if 0
			(yyval.val) = (yyvsp[-2].val);
#endif
			{VALUE v1,v2,v3;v1=escape_Qundef((yyvsp[-2].val));v2=escape_Qundef((yyvsp[-1].val));v3=dispatch2(block_var,v1,v2);(yyval.val)=v3;}
		    }
#line 10691 "ripper.c"
    break;

  case 438: /* opt_bv_decl: opt_nl  */
#line 3743 "ripper.y"
                    {
			(yyval.val) = 0;
		    }
#line 10699 "ripper.c"
    break;

  case 439: /* opt_bv_decl: opt_nl ';' bv_decls opt_nl  */
#line 3747 "ripper.y"
                    {
#if 0
			(yyval.val) = 0;
#endif
			(yyval.val)=(yyvsp[-1].val);
		    }
#line 10710 "ripper.c"
    break;

  case 440: /* bv_decls: bvar  */
#line 3756 "ripper.y"
                        {(yyval.val)=rb_ary_new3(1, get_value((yyvsp[0].val)));}
#line 10716 "ripper.c"
    break;

  case 441: /* bv_decls: bv_decls ',' bvar  */
#line 3758 "ripper.y"
                        {(yyval.val)=rb_ary_push((yyvsp[-2].val), get_value((yyvsp[0].val)));}
#line 10722 "ripper.c"
    break;

  case 442: /* bvar: "local variable or method"  */
#line 3762 "ripper.y"
                    {
			new_bv(p, get_id((yyvsp[0].val)));
			(yyval.val)=get_value((yyvsp[0].val));
		    }
#line 10731 "ripper.c"
    break;

  case 443: /* bvar: f_bad_arg  */
#line 3767 "ripper.y"
                    {
			(yyval.val) = 0;
		    }
#line 10739 "ripper.c"
    break;

  case 444: /* @25: %empty  */
#line 3773 "ripper.y"
                    {
			token_info_push(p, "->", &(yylsp[0]));
			(yyvsp[0].vars) = dyna_push(p);
			(yyval.num) = p->lex.lpar_beg;
			p->lex.lpar_beg = p->lex.paren_nest;
		    }
#line 10750 "ripper.c"
    break;

  case 445: /* @26: %empty  */
#line 3779 "ripper.y"
                    {
			(yyval.num) = p->max_numparam;
			p->max_numparam = 0;
		    }
#line 10759 "ripper.c"
    break;

  case 446: /* @27: %empty  */
#line 3783 "ripper.y"
                    {
			(yyval.node) = numparam_push(p);
		    }
#line 10767 "ripper.c"
    break;

  case 447: /* $@28: %empty  */
#line 3787 "ripper.y"
                    {
			CMDARG_PUSH(0);
		    }
#line 10775 "ripper.c"
    break;

  case 448: /* lambda: "->" @25 @26 @27 f_larglist $@28 lambda_body  */
#line 3791 "ripper.y"
                    {
			int max_numparam = p->max_numparam;
			p->lex.lpar_beg = (yyvsp[-5].num);
			p->max_numparam = (yyvsp[-4].num);
			CMDARG_POP();
			(yyvsp[-2].val) = args_with_numbered(p, (yyvsp[-2].val), max_numparam);
#if 0
                        {
                            YYLTYPE loc = code_loc_gen(&(yylsp[-2]), &(yylsp[0]));
                            (yyval.val) = NEW_LAMBDA((yyvsp[-2].val), (yyvsp[0].val), &loc);
                            nd_set_line((yyval.val)->nd_body, (yylsp[0]).end_pos.lineno);
                            nd_set_line((yyval.val), (yylsp[-2]).end_pos.lineno);
			    nd_set_first_loc((yyval.val), (yylsp[-6]).beg_pos);
                        }
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(lambda,v1,v2);(yyval.val)=v3;}
			numparam_pop(p, (yyvsp[-3].node));
			dyna_pop(p, (yyvsp[-6].vars));
		    }
#line 10799 "ripper.c"
    break;

  case 449: /* f_larglist: '(' f_args opt_bv_decl ')'  */
#line 3813 "ripper.y"
                    {
			p->ctxt.in_argdef = 0;
#if 0
			(yyval.val) = (yyvsp[-2].val);
			p->max_numparam = ORDINAL_PARAM;
#endif
			{VALUE v1,v2;v1=(yyvsp[-2].val);v2=dispatch1(paren,v1);(yyval.val)=v2;}
		    }
#line 10812 "ripper.c"
    break;

  case 450: /* f_larglist: f_args  */
#line 3822 "ripper.y"
                    {
			p->ctxt.in_argdef = 0;
#if 0
			if (!args_info_empty_p((yyvsp[0].val)->nd_ainfo))
			    p->max_numparam = ORDINAL_PARAM;
#endif
			(yyval.val) = (yyvsp[0].val);
		    }
#line 10825 "ripper.c"
    break;

  case 451: /* lambda_body: tLAMBEG compstmt '}'  */
#line 3833 "ripper.y"
                    {
			token_info_pop(p, "}", &(yylsp[0]));
			(yyval.val) = (yyvsp[-1].val);
		    }
#line 10834 "ripper.c"
    break;

  case 452: /* lambda_body: "`do' for lambda" bodystmt k_end  */
#line 3838 "ripper.y"
                    {
			(yyval.val) = (yyvsp[-1].val);
		    }
#line 10842 "ripper.c"
    break;

  case 453: /* do_block: k_do_block do_body k_end  */
#line 3844 "ripper.y"
                    {
			(yyval.val) = (yyvsp[-1].val);
#if 0
			(yyval.val)->nd_body->nd_loc = code_loc_gen(&(yylsp[-2]), &(yylsp[0]));
			nd_set_line((yyval.val), (yylsp[-2]).end_pos.lineno);
#endif
		    }
#line 10854 "ripper.c"
    break;

  case 454: /* block_call: command do_block  */
#line 3854 "ripper.y"
                    {
#if 0
			if (nd_type_p((yyvsp[-1].val), NODE_YIELD)) {
			    compile_error(p, "block given to yield");
			}
			else {
			    block_dup_check(p, (yyvsp[-1].val)->nd_args, (yyvsp[0].val));
			}
			(yyval.val) = method_add_block(p, (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-1].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-1].val);v2=(yyvsp[0].val);v3=dispatch2(method_add_block,v1,v2);(yyval.val)=v3;}
		    }
#line 10872 "ripper.c"
    break;

  case 455: /* block_call: block_call call_op2 operation2 opt_paren_args  */
#line 3868 "ripper.y"
                    {
#if 0
			(yyval.val) = new_qcall(p, (yyvsp[-2].val), (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-3].val);v2=(yyvsp[-2].val);v3=(yyvsp[-1].val);v4=dispatch3(call,v1,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=v6==Qundef ? v5 : dispatch2(method_add_arg,v5,v6);(yyval.val)=v7;}
		    }
#line 10883 "ripper.c"
    break;

  case 456: /* block_call: block_call call_op2 operation2 opt_paren_args brace_block  */
#line 3875 "ripper.y"
                    {
#if 0
			(yyval.val) = new_command_qcall(p, (yyvsp[-3].val), (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-2]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-4].val);v2=(yyvsp[-3].val);v3=(yyvsp[-2].val);v4=(yyvsp[-1].val);v5=dispatch4(command_call,v1,v2,v3,v4);v6=v5;v7=(yyvsp[0].val);v8=v7==Qundef ? v6 : dispatch2(method_add_block,v6,v7);(yyval.val)=v8;}
		    }
#line 10894 "ripper.c"
    break;

  case 457: /* block_call: block_call call_op2 operation2 command_args do_block  */
#line 3882 "ripper.y"
                    {
#if 0
			(yyval.val) = new_command_qcall(p, (yyvsp[-3].val), (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-2]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7,v8;v1=(yyvsp[-4].val);v2=(yyvsp[-3].val);v3=(yyvsp[-2].val);v4=(yyvsp[-1].val);v5=dispatch4(command_call,v1,v2,v3,v4);v6=v5;v7=(yyvsp[0].val);v8=dispatch2(method_add_block,v6,v7);(yyval.val)=v8;}
		    }
#line 10905 "ripper.c"
    break;

  case 458: /* method_call: fcall paren_args  */
#line 3891 "ripper.y"
                    {
#if 0
			(yyval.val) = (yyvsp[-1].val);
			(yyval.val)->nd_args = (yyvsp[0].val);
			nd_set_last_loc((yyvsp[-1].val), (yylsp[0]).end_pos);
#endif
			{VALUE v1,v2,v3,v4,v5;v1=(yyvsp[-1].val);v2=dispatch1(fcall,v1);v3=v2;v4=(yyvsp[0].val);v5=dispatch2(method_add_arg,v3,v4);(yyval.val)=v5;}
		    }
#line 10918 "ripper.c"
    break;

  case 459: /* method_call: primary_value call_op operation2 opt_paren_args  */
#line 3900 "ripper.y"
                    {
#if 0
			(yyval.val) = new_qcall(p, (yyvsp[-2].val), (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
			nd_set_line((yyval.val), (yylsp[-1]).end_pos.lineno);
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-3].val);v2=(yyvsp[-2].val);v3=(yyvsp[-1].val);v4=dispatch3(call,v1,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=v6==Qundef ? v5 : dispatch2(method_add_arg,v5,v6);(yyval.val)=v7;}
		    }
#line 10930 "ripper.c"
    break;

  case 460: /* method_call: primary_value "::" operation2 paren_args  */
#line 3908 "ripper.y"
                    {
#if 0
			(yyval.val) = new_qcall(p, ID2VAL(idCOLON2), (yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
			nd_set_line((yyval.val), (yylsp[-1]).end_pos.lineno);
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-3].val);v2=ID2VAL(idCOLON2);v3=(yyvsp[-1].val);v4=dispatch3(call,v1,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=dispatch2(method_add_arg,v5,v6);(yyval.val)=v7;}
		    }
#line 10942 "ripper.c"
    break;

  case 461: /* method_call: primary_value "::" operation3  */
#line 3916 "ripper.y"
                    {
#if 0
			(yyval.val) = new_qcall(p, ID2VAL(idCOLON2), (yyvsp[-2].val), (yyvsp[0].val), Qnull, &(yylsp[0]), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-2].val);v2=ID2VAL(idCOLON2);v3=(yyvsp[0].val);v4=dispatch3(call,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 10953 "ripper.c"
    break;

  case 462: /* method_call: primary_value call_op paren_args  */
#line 3923 "ripper.y"
                    {
#if 0
			(yyval.val) = new_qcall(p, (yyvsp[-1].val), (yyvsp[-2].val), ID2VAL(idCall), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
			nd_set_line((yyval.val), (yylsp[-1]).end_pos.lineno);
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-2].val);v2=(yyvsp[-1].val);v3=ID2VAL(idCall);v4=dispatch3(call,v1,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=dispatch2(method_add_arg,v5,v6);(yyval.val)=v7;}
		    }
#line 10965 "ripper.c"
    break;

  case 463: /* method_call: primary_value "::" paren_args  */
#line 3931 "ripper.y"
                    {
#if 0
			(yyval.val) = new_qcall(p, ID2VAL(idCOLON2), (yyvsp[-2].val), ID2VAL(idCall), (yyvsp[0].val), &(yylsp[-1]), &(yyloc));
			nd_set_line((yyval.val), (yylsp[-1]).end_pos.lineno);
#endif
			{VALUE v1,v2,v3,v4,v5,v6,v7;v1=(yyvsp[-2].val);v2=ID2VAL(idCOLON2);v3=ID2VAL(idCall);v4=dispatch3(call,v1,v2,v3);v5=v4;v6=(yyvsp[0].val);v7=dispatch2(method_add_arg,v5,v6);(yyval.val)=v7;}
		    }
#line 10977 "ripper.c"
    break;

  case 464: /* method_call: "`super'" paren_args  */
#line 3939 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_SUPER((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2;v1=(yyvsp[0].val);v2=dispatch1(super,v1);(yyval.val)=v2;}
		    }
#line 10988 "ripper.c"
    break;

  case 465: /* method_call: "`super'"  */
#line 3946 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_ZSUPER(&(yyloc));
#endif
			{VALUE v1;v1=dispatch0(zsuper);(yyval.val)=v1;}
		    }
#line 10999 "ripper.c"
    break;

  case 466: /* method_call: primary_value '[' opt_call_args rbracket  */
#line 3953 "ripper.y"
                    {
#if 0
			if ((yyvsp[-3].val) && nd_type_p((yyvsp[-3].val), NODE_SELF))
			    (yyval.val) = NEW_FCALL(tAREF, (yyvsp[-1].val), &(yyloc));
			else
			    (yyval.val) = NEW_CALL((yyvsp[-3].val), tAREF, (yyvsp[-1].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-3].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=escape_Qundef((yyvsp[-1].val));v3=dispatch2(aref,v1,v2);(yyval.val)=v3;}
		    }
#line 11014 "ripper.c"
    break;

  case 467: /* brace_block: '{' brace_body '}'  */
#line 3966 "ripper.y"
                    {
			(yyval.val) = (yyvsp[-1].val);
#if 0
			(yyval.val)->nd_body->nd_loc = code_loc_gen(&(yylsp[-2]), &(yylsp[0]));
			nd_set_line((yyval.val), (yylsp[-2]).end_pos.lineno);
#endif
		    }
#line 11026 "ripper.c"
    break;

  case 468: /* brace_block: k_do do_body k_end  */
#line 3974 "ripper.y"
                    {
			(yyval.val) = (yyvsp[-1].val);
#if 0
			(yyval.val)->nd_body->nd_loc = code_loc_gen(&(yylsp[-2]), &(yylsp[0]));
			nd_set_line((yyval.val), (yylsp[-2]).end_pos.lineno);
#endif
		    }
#line 11038 "ripper.c"
    break;

  case 469: /* @29: %empty  */
#line 3983 "ripper.y"
                  {(yyval.vars) = dyna_push(p);}
#line 11044 "ripper.c"
    break;

  case 470: /* @30: %empty  */
#line 3984 "ripper.y"
                    {
			(yyval.num) = p->max_numparam;
			p->max_numparam = 0;
		    }
#line 11053 "ripper.c"
    break;

  case 471: /* @31: %empty  */
#line 3988 "ripper.y"
                    {
			(yyval.node) = numparam_push(p);
		    }
#line 11061 "ripper.c"
    break;

  case 472: /* brace_body: @29 @30 @31 opt_block_param compstmt  */
#line 3992 "ripper.y"
                    {
			int max_numparam = p->max_numparam;
			p->max_numparam = (yyvsp[-3].num);
			(yyvsp[-1].val) = args_with_numbered(p, (yyvsp[-1].val), max_numparam);
#if 0
			(yyval.val) = NEW_ITER((yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=escape_Qundef((yyvsp[-1].val));v2=(yyvsp[0].val);v3=dispatch2(brace_block,v1,v2);(yyval.val)=v3;}
			numparam_pop(p, (yyvsp[-2].node));
			dyna_pop(p, (yyvsp[-4].vars));
		    }
#line 11077 "ripper.c"
    break;

  case 473: /* @32: %empty  */
#line 4005 "ripper.y"
                  {(yyval.vars) = dyna_push(p);}
#line 11083 "ripper.c"
    break;

  case 474: /* @33: %empty  */
#line 4006 "ripper.y"
                    {
			(yyval.num) = p->max_numparam;
			p->max_numparam = 0;
		    }
#line 11092 "ripper.c"
    break;

  case 475: /* @34: %empty  */
#line 4010 "ripper.y"
                    {
			(yyval.node) = numparam_push(p);
			CMDARG_PUSH(0);
		    }
#line 11101 "ripper.c"
    break;

  case 476: /* do_body: @32 @33 @34 opt_block_param bodystmt  */
#line 4015 "ripper.y"
                    {
			int max_numparam = p->max_numparam;
			p->max_numparam = (yyvsp[-3].num);
			(yyvsp[-1].val) = args_with_numbered(p, (yyvsp[-1].val), max_numparam);
#if 0
			(yyval.val) = NEW_ITER((yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=escape_Qundef((yyvsp[-1].val));v2=(yyvsp[0].val);v3=dispatch2(do_block,v1,v2);(yyval.val)=v3;}
			CMDARG_POP();
			numparam_pop(p, (yyvsp[-2].node));
			dyna_pop(p, (yyvsp[-4].vars));
		    }
#line 11118 "ripper.c"
    break;

  case 477: /* case_args: arg_value  */
#line 4030 "ripper.y"
                    {
#if 0
			check_literal_when(p, (yyvsp[0].val), &(yylsp[0]));
			(yyval.val) = NEW_LIST((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(args_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(args_add,v2,v3);(yyval.val)=v4;}
		    }
#line 11130 "ripper.c"
    break;

  case 478: /* case_args: "*" arg_value  */
#line 4038 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_SPLAT((yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=dispatch0(args_new);v2=v1;v3=(yyvsp[0].val);v4=dispatch2(args_add_star,v2,v3);(yyval.val)=v4;}
		    }
#line 11141 "ripper.c"
    break;

  case 479: /* case_args: case_args ',' arg_value  */
#line 4045 "ripper.y"
                    {
#if 0
			check_literal_when(p, (yyvsp[0].val), &(yylsp[0]));
			(yyval.val) = last_arg_append(p, (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-2].val);v2=(yyvsp[0].val);v3=dispatch2(args_add,v1,v2);(yyval.val)=v3;}
		    }
#line 11153 "ripper.c"
    break;

  case 480: /* case_args: case_args ',' "*" arg_value  */
#line 4053 "ripper.y"
                    {
#if 0
			(yyval.val) = rest_arg_append(p, (yyvsp[-3].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[-3].val);v2=(yyvsp[0].val);v3=dispatch2(args_add_star,v1,v2);(yyval.val)=v3;}
		    }
#line 11164 "ripper.c"
    break;

  case 481: /* case_body: k_when case_args then compstmt cases  */
#line 4064 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_WHEN((yyvsp[-3].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
			fixpos((yyval.val), (yyvsp[-3].val));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-3].val);v2=(yyvsp[-1].val);v3=escape_Qundef((yyvsp[0].val));v4=dispatch3(when,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 11176 "ripper.c"
    break;

  case 484: /* @35: %empty  */
#line 4078 "ripper.y"
                    {
			SET_LEX_STATE(EXPR_BEG|EXPR_LABEL);
			p->command_start = FALSE;
			(yyvsp[0].ctxt) = p->ctxt;
			p->ctxt.in_kwarg = 1;
			(yyval.tbl) = push_pvtbl(p);
		    }
#line 11188 "ripper.c"
    break;

  case 485: /* @36: %empty  */
#line 4085 "ripper.y"
                    {
			(yyval.tbl) = push_pktbl(p);
		    }
#line 11196 "ripper.c"
    break;

  case 486: /* $@37: %empty  */
#line 4089 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			pop_pvtbl(p, (yyvsp[-3].tbl));
			p->ctxt.in_kwarg = (yyvsp[-4].ctxt).in_kwarg;
		    }
#line 11206 "ripper.c"
    break;

  case 487: /* p_case_body: "`in'" @35 @36 p_top_expr then $@37 compstmt p_cases  */
#line 4096 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_IN((yyvsp[-4].val), (yyvsp[-1].val), (yyvsp[0].val), &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-4].val);v2=(yyvsp[-1].val);v3=escape_Qundef((yyvsp[0].val));v4=dispatch3(in,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 11217 "ripper.c"
    break;

  case 491: /* p_top_expr: p_top_expr_body "`if' modifier" expr_value  */
#line 4110 "ripper.y"
                    {
#if 0
			(yyval.val) = new_if(p, (yyvsp[0].val), (yyvsp[-2].val), 0, &(yyloc));
			fixpos((yyval.val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[0].val);v2=(yyvsp[-2].val);v3=dispatch2(if_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 11229 "ripper.c"
    break;

  case 492: /* p_top_expr: p_top_expr_body "`unless' modifier" expr_value  */
#line 4118 "ripper.y"
                    {
#if 0
			(yyval.val) = new_unless(p, (yyvsp[0].val), (yyvsp[-2].val), 0, &(yyloc));
			fixpos((yyval.val), (yyvsp[0].val));
#endif
			{VALUE v1,v2,v3;v1=(yyvsp[0].val);v2=(yyvsp[-2].val);v3=dispatch2(unless_mod,v1,v2);(yyval.val)=v3;}
		    }
#line 11241 "ripper.c"
    break;

  case 494: /* p_top_expr_body: p_expr ','  */
#line 4129 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, Qnone, 1, 0, Qnone, &(yyloc));
			(yyval.val) = new_array_pattern(p, Qnone, get_value((yyvsp[-1].val)), (yyval.val), &(yyloc));
		    }
#line 11250 "ripper.c"
    break;

  case 495: /* p_top_expr_body: p_expr ',' p_args  */
#line 4134 "ripper.y"
                    {
			(yyval.val) = new_array_pattern(p, Qnone, get_value((yyvsp[-2].val)), (yyvsp[0].val), &(yyloc));
#if 0
			nd_set_first_loc((yyval.val), (yylsp[-2]).beg_pos);
#endif

		    }
#line 11262 "ripper.c"
    break;

  case 496: /* p_top_expr_body: p_find  */
#line 4142 "ripper.y"
                    {
			(yyval.val) = new_find_pattern(p, Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 11270 "ripper.c"
    break;

  case 497: /* p_top_expr_body: p_args_tail  */
#line 4146 "ripper.y"
                    {
			(yyval.val) = new_array_pattern(p, Qnone, Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 11278 "ripper.c"
    break;

  case 498: /* p_top_expr_body: p_kwargs  */
#line 4150 "ripper.y"
                    {
			(yyval.val) = new_hash_pattern(p, Qnone, (yyvsp[0].val), &(yyloc));
		    }
#line 11286 "ripper.c"
    break;

  case 500: /* p_as: p_expr "=>" p_variable  */
#line 4159 "ripper.y"
                    {
#if 0
			NODE *n = NEW_LIST((yyvsp[-2].val), &(yyloc));
			n = list_append(p, n, (yyvsp[0].val));
			(yyval.val) = new_hash(p, n, &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-2].val);v2=STATIC_ID2SYM(id_assoc);v3=(yyvsp[0].val);v4=dispatch3(binary,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 11299 "ripper.c"
    break;

  case 502: /* p_alt: p_alt '|' p_expr_basic  */
#line 4171 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_NODE(NODE_OR, (yyvsp[-2].val), (yyvsp[0].val), 0, &(yyloc));
#endif
			{VALUE v1,v2,v3,v4;v1=(yyvsp[-2].val);v2=STATIC_ID2SYM(idOr);v3=(yyvsp[0].val);v4=dispatch3(binary,v1,v2,v3);(yyval.val)=v4;}
		    }
#line 11310 "ripper.c"
    break;

  case 504: /* p_lparen: '('  */
#line 4180 "ripper.y"
                      {(yyval.tbl) = push_pktbl(p);}
#line 11316 "ripper.c"
    break;

  case 505: /* p_lbracket: '['  */
#line 4181 "ripper.y"
                      {(yyval.tbl) = push_pktbl(p);}
#line 11322 "ripper.c"
    break;

  case 508: /* p_expr_basic: p_const p_lparen p_args rparen  */
#line 4186 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			(yyval.val) = new_array_pattern(p, (yyvsp[-3].val), Qnone, (yyvsp[-1].val), &(yyloc));
#if 0
			nd_set_first_loc((yyval.val), (yylsp[-3]).beg_pos);
#endif

		    }
#line 11335 "ripper.c"
    break;

  case 509: /* p_expr_basic: p_const p_lparen p_find rparen  */
#line 4195 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			(yyval.val) = new_find_pattern(p, (yyvsp[-3].val), (yyvsp[-1].val), &(yyloc));
#if 0
			nd_set_first_loc((yyval.val), (yylsp[-3]).beg_pos);
#endif

		    }
#line 11348 "ripper.c"
    break;

  case 510: /* p_expr_basic: p_const p_lparen p_kwargs rparen  */
#line 4204 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			(yyval.val) = new_hash_pattern(p, (yyvsp[-3].val), (yyvsp[-1].val), &(yyloc));
#if 0
			nd_set_first_loc((yyval.val), (yylsp[-3]).beg_pos);
#endif

		    }
#line 11361 "ripper.c"
    break;

  case 511: /* p_expr_basic: p_const '(' rparen  */
#line 4213 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, Qnone, 0, 0, Qnone, &(yyloc));
			(yyval.val) = new_array_pattern(p, (yyvsp[-2].val), Qnone, (yyval.val), &(yyloc));
		    }
#line 11370 "ripper.c"
    break;

  case 512: /* p_expr_basic: p_const p_lbracket p_args rbracket  */
#line 4218 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			(yyval.val) = new_array_pattern(p, (yyvsp[-3].val), Qnone, (yyvsp[-1].val), &(yyloc));
#if 0
			nd_set_first_loc((yyval.val), (yylsp[-3]).beg_pos);
#endif

		    }
#line 11383 "ripper.c"
    break;

  case 513: /* p_expr_basic: p_const p_lbracket p_find rbracket  */
#line 4227 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			(yyval.val) = new_find_pattern(p, (yyvsp[-3].val), (yyvsp[-1].val), &(yyloc));
#if 0
			nd_set_first_loc((yyval.val), (yylsp[-3]).beg_pos);
#endif

		    }
#line 11396 "ripper.c"
    break;

  case 514: /* p_expr_basic: p_const p_lbracket p_kwargs rbracket  */
#line 4236 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			(yyval.val) = new_hash_pattern(p, (yyvsp[-3].val), (yyvsp[-1].val), &(yyloc));
#if 0
			nd_set_first_loc((yyval.val), (yylsp[-3]).beg_pos);
#endif

		    }
#line 11409 "ripper.c"
    break;

  case 515: /* p_expr_basic: p_const '[' rbracket  */
#line 4245 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, Qnone, 0, 0, Qnone, &(yyloc));
			(yyval.val) = new_array_pattern(p, (yyvsp[-2].val), Qnone, (yyval.val), &(yyloc));
		    }
#line 11418 "ripper.c"
    break;

  case 516: /* p_expr_basic: "[" p_args rbracket  */
#line 4250 "ripper.y"
                    {
			(yyval.val) = new_array_pattern(p, Qnone, Qnone, (yyvsp[-1].val), &(yyloc));
		    }
#line 11426 "ripper.c"
    break;

  case 517: /* p_expr_basic: "[" p_find rbracket  */
#line 4254 "ripper.y"
                    {
			(yyval.val) = new_find_pattern(p, Qnone, (yyvsp[-1].val), &(yyloc));
		    }
#line 11434 "ripper.c"
    break;

  case 518: /* p_expr_basic: "[" rbracket  */
#line 4258 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, Qnone, 0, 0, Qnone, &(yyloc));
			(yyval.val) = new_array_pattern(p, Qnone, Qnone, (yyval.val), &(yyloc));
		    }
#line 11443 "ripper.c"
    break;

  case 519: /* @38: %empty  */
#line 4263 "ripper.y"
                    {
			(yyval.tbl) = push_pktbl(p);
			(yyvsp[0].ctxt) = p->ctxt;
			p->ctxt.in_kwarg = 0;
		    }
#line 11453 "ripper.c"
    break;

  case 520: /* p_expr_basic: "{" @38 p_kwargs rbrace  */
#line 4269 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			p->ctxt.in_kwarg = (yyvsp[-3].ctxt).in_kwarg;
			(yyval.val) = new_hash_pattern(p, Qnone, (yyvsp[-1].val), &(yyloc));
		    }
#line 11463 "ripper.c"
    break;

  case 521: /* p_expr_basic: "{" rbrace  */
#line 4275 "ripper.y"
                    {
			(yyval.val) = new_hash_pattern_tail(p, Qnone, 0, &(yyloc));
			(yyval.val) = new_hash_pattern(p, Qnone, (yyval.val), &(yyloc));
		    }
#line 11472 "ripper.c"
    break;

  case 522: /* @39: %empty  */
#line 4279 "ripper.y"
                          {(yyval.tbl) = push_pktbl(p);}
#line 11478 "ripper.c"
    break;

  case 523: /* p_expr_basic: "(" @39 p_expr rparen  */
#line 4280 "ripper.y"
                    {
			pop_pktbl(p, (yyvsp[-2].tbl));
			(yyval.val) = (yyvsp[-1].val);
		    }
#line 11487 "ripper.c"
    break;

  case 524: /* p_args: p_expr  */
#line 4287 "ripper.y"
                    {
#if 0
			NODE *pre_args = NEW_LIST((yyvsp[0].val), &(yyloc));
			(yyval.val) = new_array_pattern_tail(p, pre_args, 0, 0, Qnone, &(yyloc));
#endif
			(yyval.val) = new_array_pattern_tail(p, rb_ary_new_from_args(1, get_value((yyvsp[0].val))), 0, 0, Qnone, &(yyloc));

		    }
#line 11500 "ripper.c"
    break;

  case 525: /* p_args: p_args_head  */
#line 4296 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, (yyvsp[0].val), 1, 0, Qnone, &(yyloc));
		    }
#line 11508 "ripper.c"
    break;

  case 526: /* p_args: p_args_head p_arg  */
#line 4300 "ripper.y"
                    {
#if 0
			(yyval.val) = new_array_pattern_tail(p, list_concat((yyvsp[-1].val), (yyvsp[0].val)), 0, 0, Qnone, &(yyloc));
#endif
			VALUE pre_args = rb_ary_concat((yyvsp[-1].val), get_value((yyvsp[0].val)));
			(yyval.val) = new_array_pattern_tail(p, pre_args, 0, 0, Qnone, &(yyloc));

		    }
#line 11521 "ripper.c"
    break;

  case 527: /* p_args: p_args_head "*" "local variable or method"  */
#line 4309 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, (yyvsp[-2].val), 1, (yyvsp[0].val), Qnone, &(yyloc));
		    }
#line 11529 "ripper.c"
    break;

  case 528: /* p_args: p_args_head "*" "local variable or method" ',' p_args_post  */
#line 4313 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, (yyvsp[-4].val), 1, (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
		    }
#line 11537 "ripper.c"
    break;

  case 529: /* p_args: p_args_head "*"  */
#line 4317 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, (yyvsp[-1].val), 1, 0, Qnone, &(yyloc));
		    }
#line 11545 "ripper.c"
    break;

  case 530: /* p_args: p_args_head "*" ',' p_args_post  */
#line 4321 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, (yyvsp[-3].val), 1, 0, (yyvsp[0].val), &(yyloc));
		    }
#line 11553 "ripper.c"
    break;

  case 532: /* p_args_head: p_arg ','  */
#line 4328 "ripper.y"
                    {
			(yyval.val) = (yyvsp[-1].val);
		    }
#line 11561 "ripper.c"
    break;

  case 533: /* p_args_head: p_args_head p_arg ','  */
#line 4332 "ripper.y"
                    {
#if 0
			(yyval.val) = list_concat((yyvsp[-2].val), (yyvsp[-1].val));
#endif
			(yyval.val)=rb_ary_concat((yyvsp[-2].val), get_value((yyvsp[-1].val)));
		    }
#line 11572 "ripper.c"
    break;

  case 534: /* p_args_tail: p_rest  */
#line 4341 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, Qnone, 1, (yyvsp[0].val), Qnone, &(yyloc));
		    }
#line 11580 "ripper.c"
    break;

  case 535: /* p_args_tail: p_rest ',' p_args_post  */
#line 4345 "ripper.y"
                    {
			(yyval.val) = new_array_pattern_tail(p, Qnone, 1, (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));
		    }
#line 11588 "ripper.c"
    break;

  case 536: /* p_find: p_rest ',' p_args_post ',' p_rest  */
#line 4351 "ripper.y"
                    {
			(yyval.val) = new_find_pattern_tail(p, (yyvsp[-4].val), (yyvsp[-2].val), (yyvsp[0].val), &(yyloc));

			if (rb_warning_category_enabled_p(RB_WARN_CATEGORY_EXPERIMENTAL))
			    rb_warn0L_experimental(nd_line((yyval.val)), "Find pattern is experimental, and the behavior may change in future versions of Ruby!");
		    }
#line 11599 "ripper.c"
    break;

  case 537: /* p_rest: "*" "local variable or method"  */
#line 4361 "ripper.y"
                    {
			(yyval.val) = (yyvsp[0].val);
		    }
#line 11607 "ripper.c"
    break;

  case 538: /* p_rest: "*"  */
#line 4365 "ripper.y"
                    {
			(yyval.val) = 0;
		    }
#line 11615 "ripper.c"
    break;

  case 540: /* p_args_post: p_args_post ',' p_arg  */
#line 4372 "ripper.y"
                    {
#if 0
			(yyval.val) = list_concat((yyvsp[-2].val), (yyvsp[0].val));
#endif
			(yyval.val)=rb_ary_concat((yyvsp[-2].val), get_value((yyvsp[0].val)));
		    }
#line 11626 "ripper.c"
    break;

  case 541: /* p_arg: p_expr  */
#line 4381 "ripper.y"
                    {
#if 0
			(yyval.val) = NEW_LIST((yyvsp[0].val), &(yyloc));
#endif
			(yyval.val)=rb_ary_new_from_args(1, get_value((yyvsp[0].val)));
		    }
#line 11637 "ripper.c"
    break;

  case 542: /* p_kwargs: p_kwarg ',' p_any_kwrest  */
#line 4390 "ripper.y"
                    {
			(yyval.val) =  new_hash_pattern_tail(p, new_unique_key_hash(p, (yyvsp[-2].val), &(yyloc)), (yyvsp[0].val), &(yyloc));
		    }
#line 11645 "ripper.c"
    break;

  case 543: /* p_kwargs: p_kwarg  */
#line 4394 "ripper.y"
                    {
			(yyval.val) =  new_hash_pattern_tail(p, new_unique_key_hash(p, (yyvsp[0].val), &(yyloc)), 0, &(yyloc));
		    }
#line 11653 "ripper.c"
    break;

  case 544: /* p_kwargs: p_kwarg ','  */
#line 4398 "ripper.y"
                    {
			(yyval.val) =  new_hash_pattern_tail(p, new_unique_key_hash(p, (yyvsp[-1].val), &(yyloc)), 0, &(yyloc));
		    }
#line 11661 "ripper.c"
    break;

  case 545: /* p_kwargs: p_any_kwrest  */
#line 4402 "ripper.y"
                    {
			(yyval.val) =  new_hash_pattern_tail(p, new_hash(p, Qnone, &(yyloc)), (yyvsp[0].val), &(yyloc));
		    }
#line 11669 "ripper.c"
    break;

  case 546: /* p_kwarg: p_kw  */
#line 4408 "ripper.y"
                        {(yyval.val)=rb_ary_new_from_args(1, (yyvsp[0].val));}
#line 11675 "ripper.c"
    break;

  case 547: /* p_kwarg: p_kwarg ',' p_kw  */
#line 4410 "ripper.y"
                    {
#if 0
			(yyval.val) = list_concat((yyvsp[-2].val), (yyvsp[0].val));
#endif
			(yyval.val)=rb_ary_push((yyvsp[-2].val), (yyvsp[0].val));
		    }
#line 11686 "ripper.c"
    break;

  case 548: /* p_kw: p_kw_label p_expr  */
#line 4419 "ripper.y"
                    {
			error_duplicate_pattern_key(p, get_id((yyvsp[-1].val)), &(yylsp[-1]));
#if 0
			(yyval.val) = list_append(p, NEW_LIST(NEW_LIT(ID2SYM((yyvsp[-1].val)), &(yyloc)), &(yyloc)), (yyvsp[0].val));
#endif
			(yyval.val)=rb_ary_new_from_args(2, get_value((yyvsp[-1].val)), get_value((yyvsp[0].val)));
		    }
#line 11698 "ripper.c"
    break;

  case 549: /* p_kw: p_kw_label  */
#line 4427 "ripper.y"
                    {
			error_duplicate_pattern_key(p, get_id((yyvsp[0].val)), &(yylsp[0]));
			if ((yyvsp[0].val) && !is_local_id(get_id((yyvsp[0].val)))) {
			    yyerror1(&(yylsp[0]), "key must be valid as local variables");
			}
			error_duplicate_pattern_variable(p, get_id((yyvsp[0].val)), &(yylsp[0]));
#if 0
			(yyval.val) = list_append(p, NEW_LIST(NEW_LI