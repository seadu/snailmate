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
       0,     0,   222,   222,     0,     0,     0,     