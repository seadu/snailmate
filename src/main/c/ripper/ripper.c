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
       2,     2,     2,     2