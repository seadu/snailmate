/*
 ***** BEGIN LICENSE BLOCK *****
 * Version: EPL 2.0/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Eclipse Public
 * License Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.eclipse.org/legal/epl-v20.html
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * Copyright (C) 2008-2017 Thomas E Enebo <enebo@acm.org>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the EPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the EPL, the GPL or the LGPL.
 *
 * Copyright (c) 2016 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 ***** END LICENSE BLOCK *****/
// created by jay 1.0.2 (c) 2002-2004 ats@cs.rit.edu
// skeleton Java 1.0 (c) 2002 ats@cs.rit.edu

// line 2 "RubyParser.y"
package org.truffleruby.parser.parser;

import com.oracle.truffle.api.strings.TruffleString;

import org.truffleruby.Layouts;
import org.truffleruby.annotations.SuppressFBWarnings;
import org.truffleruby.core.encoding.Encodings;
import org.truffleruby.core.encoding.RubyEncoding;
import org.truffleruby.core.encoding.TStringUtils;
import org.truffleruby.core.string.TStringConstants;
import org.truffleruby.language.SourceIndexLength;
import org.truffleruby.parser.RubyDeferredWarnings;
import org.truffleruby.parser.ast.ArgsParseNode;
import org.truffleruby.parser.ast.ArgumentParseNode;
import org.truffleruby.parser.ast.ArrayParseNode;
import org.truffleruby.parser.ast.AssignableParseNode;
import org.truffleruby.parser.ast.BackRefParseNode;
import org.truffleruby.parser.ast.BeginParseNode;
import org.truffleruby.parser.ast.BlockAcceptingParseNode;
import org.truffleruby.parser.ast.BlockArgParseNode;
import org.truffleruby.parser.ast.BlockParseNode;
import org.truffleruby.parser.ast.BlockPassParseNode;
import org.truffleruby.parser.ast.BreakParseNode;
import org.truffleruby.parser.ast.ClassParseNode;
import org.truffleruby.parser.ast.ClassVarAsgnParseNode;
import org.truffleruby.parser.ast.ClassVarParseNode;
import org.truffleruby.parser.ast.Colon3ParseNode;
import org.truffleruby.parser.ast.ConstDeclParseNode;
import org.truffleruby.parser.ast.ConstParseNode;
import org.truffleruby.parser.ast.DStrParseNode;
import org.truffleruby.parser.ast.DSymbolParseNode;
import org.truffleruby.parser.ast.DXStrParseNode;
import org.truffleruby.parser.ast.DefnParseNode;
import org.truffleruby.parser.ast.DefsParseNode;
import org.truffleruby.parser.ast.DotParseNode;
import org.truffleruby.parser.ast.EncodingParseNode;
import org.truffleruby.parser.ast.EnsureParseNode;
import org.truffleruby.parser.ast.EvStrParseNode;
import org.truffleruby.parser.ast.FCallParseNode;
import org.truffleruby.parser.ast.FalseParseNode;
import org.truffleruby.parser.ast.FileParseNode;
import org.truffleruby.parser.ast.FixnumParseNode;
import org.truffleruby.parser.ast.FloatParseNode;
import org.truffleruby.parser.ast.ForParseNode;
import org.truffleruby.parser.ast.GlobalAsgnParseNode;
import org.truffleruby.parser.ast.GlobalVarParseNode;
import org.truffleruby.parser.ast.HashParseNode;
import org.truffleruby.parser.ast.IfParseNode;
import org.truffleruby.parser.ast.InstAsgnParseNode;
import org.truffleruby.parser.ast.InstVarParseNode;
import org.truffleruby.parser.ast.IterParseNode;
import org.truffleruby.parser.ast.LambdaParseNode;
import org.truffleruby.parser.ast.ListParseNode;
import org.truffleruby.parser.ast.LiteralParseNode;
import org.truffleruby.parser.ast.LocalVarParseNode;
import org.truffleruby.parser.ast.ModuleParseNode;
import org.truffleruby.parser.ast.MultipleAsgnParseNode;
import org.truffleruby.parser.ast.NextParseNode;
import org.truffleruby.parser.ast.NilImplicitParseNode;
import org.truffleruby.parser.ast.NilParseNode;
import org.truffleruby.parser.ast.NonLocalControlFlowParseNode;
import org.truffleruby.parser.ast.NumericParseNode;
import org.truffleruby.parser.ast.OpAsgnAndParseNode;
import org.truffleruby.parser.ast.OpAsgnOrParseNode;
import org.truffleruby.parser.ast.OptArgParseNode;
import org.truffleruby.parser.ast.ParseNode;
import org.truffleruby.parser.ast.PostExeParseNode;
import org.truffleruby.parser.ast.PreExe19ParseNode;
import org.truffleruby.parser.ast.RationalParseNode;
import org.truffleruby.parser.ast.RedoParseNode;
import org.truffleruby.parser.ast.RegexpParseNode;
import org.truffleruby.parser.ast.RequiredKeywordArgumentValueParseNode;
import org.truffleruby.parser.ast.RescueBodyParseNode;
import org.truffleruby.parser.ast.RescueParseNode;
import org.truffleruby.parser.ast.RestArgParseNode;
import org.truffleruby.parser.ast.RetryParseNode;
import org.truffleruby.parser.ast.ReturnParseNode;
import org.truffleruby.parser.ast.SClassParseNode;
import org.truffleruby.parser.ast.SelfParseNode;
import org.truffleruby.parser.ast.SplatParseNode;
import org.truffleruby.parser.ast.StarParseNode;
import org.truffleruby.parser.ast.StrParseNode;
import org.truffleruby.parser.ast.TrueParseNode;
import org.truffleruby.parser.ast.UnnamedRestArgParseNode;
import org.truffleruby.parser.ast.UntilParseNode;
import org.truffleruby.parser.ast.VAliasParseNode;
import org.truffleruby.parser.ast.WhileParseNode;
import org.truffleruby.parser.ast.XStrParseNode;
import org.truffleruby.parser.ast.YieldParseNode;
import org.truffleruby.parser.ast.ZArrayParseNode;
import org.truffleruby.parser.ast.ZSuperParseNode;
import org.truffleruby.parser.ast.types.ILiteralNode;
import org.truffleruby.parser.lexer.LexerSource;
import org.truffleruby.parser.lexer.RubyLexer;
import org.truffleruby.parser.lexer.StrTerm;
import org.truffleruby.parser.lexer.SyntaxException.PID;

import static org.truffleruby.parser.lexer.RubyLexer.EXPR_BEG;
import static org.truffleruby.parser.lexer.RubyLexer.EXPR_END;
import static org.truffleruby.parser.lexer.RubyLexer.EXPR_ENDARG;
import static org.truffleruby.parser.lexer.RubyLexer.EXPR_ENDFN;
import static org.truffleruby.parser.lexer.RubyLexer.EXPR_FITEM;
import static org.truffleruby.parser.lexer.RubyLexer.EXPR_FNAME;
import static org.truffleruby.parser.lexer.RubyLexer.EXPR_LABEL;
import static org.truffleruby.parser.parser.ParserSupport.value_expr;

// @formatter:off
// CheckStyle: start generated
@SuppressFBWarnings("IP")
@SuppressWarnings({"unchecked", "fallthrough", "cast"})
public class RubyParser {
    protected final ParserSupport support;
    protected final RubyLexer lexer;

    public RubyParser(LexerSource source, RubyDeferredWarnings warnings) {
        this.support = new ParserSupport(source, warnings);
        this.lexer = new RubyLexer(support, source, warnings);
        support.setLexer(lexer);
    }
// line 125 "-"
  // %token constants
  public static final int keyword_class = 257;
  public static final int keyword_module = 258;
  public static final int keyword_def = 259;
  public static final int keyword_undef = 260;
  public static final int keyword_begin = 261;
  public static final int keyword_rescue = 262;
  public static final int keyword_ensure = 263;
  public static final int keyword_end = 264;
  public static final int keyword_if = 265;
  public static final int keyword_unless = 266;
  public static final int keyword_then = 267;
  public static final int keyword_elsif = 268;
  public static final int keyword_else = 269;
  public static final int keyword_case = 270;
  public static final int keyword_when = 271;
  public static final int keyword_while = 272;
  public static final int keyword_until = 273;
  public static final int keyword_for = 274;
  public static final int keyword_break = 275;
  public static final int keyword_next = 276;
  public static final int keyword_redo = 277;
  public static final int keyword_retry = 278;
  public static final int keyword_in = 279;
  public static final int keyword_do = 280;
  public static final int keyword_do_cond = 281;
  public static final int keyword_do_block = 282;
  public static final int keyword_return = 283;
  public static final int keyword_yield = 284;
  public static final int keyword_super = 285;
  public static final int keyword_self = 286;
  public static final int keyword_nil = 287;
  public static final int keyword_true = 288;
  public static final int keyword_false = 289;
  public static final int keyword_and = 290;
  public static final int keyword_or = 291;
  public static final int keyword_not = 292;
  public static final int modifier_if = 293;
  public static final int modifier_unless = 294;
  public static final int modifier_while = 295;
  public static final int modifier_until = 296;
  public static final int modifier_rescue = 297;
  public static final int keyword_alias = 298;
  public static final int keyword_defined = 299;
  public static final int keyword_BEGIN = 300;
  public static final int keyword_END = 301;
  public static final int keyword__LINE__ = 302;
  public static final int keyword__FILE__ = 303;
  public static final int keyword__ENCODING__ = 304;
  public static final int keyword_do_lambda = 305;
  public static final int tIDENTIFIER = 306;
  public static final int tFID = 307;
  public static final int tGVAR = 308;
  public static final int tIVAR = 309;
  public static final int tCONSTANT = 310;
  public static final int tCVAR = 311;
  public static final int tLABEL = 312;
  public static final int tCHAR = 313;
  public static final int tUPLUS = 314;
  public static final int tUMINUS = 315;
  public static final int tUMINUS_NUM = 316;
  public static final int tPOW = 317;
  public static final int tCMP = 318;
  public static final int tEQ = 319;
  public static final int tEQQ = 320;
  public static final int tNEQ = 321;
  public static final int tGEQ = 322;
  public static final int tLEQ = 323;
  public static final int tANDOP = 324;
  public static final int tOROP = 325;
  public static final int tMATCH = 326;
  public static final int tNMATCH = 327;
  public static final int tDOT = 328;
  public static final int tDOT2 = 329;
  public static final int tDOT3 = 330;
  public static final int tBDOT2 = 331;
  public static final int tBDOT3 = 332;
  public static final int tAREF = 333;
  public static final int tASET = 334;
  public static final int tLSHFT = 335;
  public static final int tRSHFT = 336;
  public static final int tANDDOT = 337;
  public static final int tCOLON2 = 338;
  public static final int tCOLON3 = 339;
  public static final int tOP_ASGN = 340;
  public static final int tASSOC = 341;
  public static final int tLPAREN = 342;
  public static final int tLPAREN2 = 343;
  public static final int tRPAREN = 344;
  public static final int tLPAREN_ARG = 345;
  public static final int tLBRACK = 346;
  public static final int tRBRACK = 347;
  public static final int tLBRACE = 348;
  public static final int tLBRACE_ARG = 349;
  public static final int tSTAR = 350;
  public static final int tSTAR2 = 351;
  public static final int tAMPER = 352;
  public static final int tAMPER2 = 353;
  public static final int tTILDE = 354;
  public static final int tPERCENT = 355;
  public static final int tDIVIDE = 356;
  public static final int tPLUS = 357;
  public static final int tMINUS = 358;
  public static final int tLT = 359;
  public static final int tGT = 360;
  public static final int tPIPE = 361;
  public static final int tBANG = 362;
  public static final int tCARET = 363;
  public static final int tLCURLY = 364;
  public static final int tRCURLY = 365;
  public static final int tBACK_REF2 = 366;
  public static final int tSYMBEG = 367;
  public static final int tSTRING_BEG = 368;
  public static final int tXSTRING_BEG = 369;
  public static final int tREGEXP_BEG = 370;
  public static final int tWORDS_BEG = 371;
  public static final int tQWORDS_BEG = 372;
  public static final int tSTRING_DBEG = 373;
  public static final int tSTRING_DVAR = 374;
  public static final int tSTRING_END = 375;
  public static final int tLAMBDA = 376;
  public static final int tLAMBEG = 377;
  public static final int tNTH_REF = 378;
  public static final int tBACK_REF = 379;
  public static final int tSTRING_CONTENT = 380;
  public static final int tINTEGER = 381;
  public static final int tIMAGINARY = 382;
  public static final int tFLOAT = 383;
  public static final int tRATIONAL = 384;
  public static final int tREGEXP_END = 385;
  public static final int tSYMBOLS_BEG = 386;
  public static final int tQSYMBOLS_BEG = 387;
  public static final int tDSTAR = 388;
  public static final int tSTRING_DEND = 389;
  public static final int tLABEL_END = 390;
  public static final int tLOWEST = 391;
  public static final int yyErrorCode = 256;

  /** number of final state.
    */
  protected static final int yyFinal = 1;

  /** parser tables.
      Order is mandated by <i>jay</i>.
    */
  protected static final short[] yyLhs = {
//yyLhs 675
    -1,   156,     0,   142,   143,   143,   143,   143,   144,   144,
    37,    36,    38,    38,    38,    38,    44,   159,    44,   160,
    39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
    39,    39,    39,    39,    39,    39,    39,    31,    31,    31,
    31,    31,    31,    31,    31,    62,    62,    62,    40,    40,
    40,    40,    40,    40,    45,    32,    32,    61,    61,   116,
   152,    43,    43,    43,    43,    43,    43,    43,    43,    43,
    43,    43,   119,   119,   130,   130,   120,   120,   120,   120,
   120,   120,   120,   120,   120,   120,    76,    76,   106,   106,
   107,   107,    77,    77,    77,    77,    77,    77,    77,    77,
    77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
    77,    83,    83,    83,    83,    83,    83,    83,    83,    83,
    83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
     8,     8,    30,    30,    30,     7,     7,     7,     7,     7,
   123,   123,   124,   124,    65,   162,    65,     6,     6,     6,
     6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
     6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
     6,     6,     6,     6,     6,     6,     6,   137,   137,   137,
   137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
   137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
   137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
   137,   137,   137,   137,   137,   137,   137,   137,   137,    41,
    41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
    41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
    41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
    41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
    41,    41,   139,   139,   139,   139,    52,    52,    78,    82,
    82,    82,    82,    63,    63,    55,    55,    55,    59,    59,
   133,   133,   133,   133,   133,    53,    53,    53,    53,    53,
   164,    57,   110,   110,   109,   109,    85,    85,    85,    85,
    35,    35,    75,    75,    75,    42,    42,    42,    42,    42,
    42,    42,    42,    42,    42,    42,   165,    42,   166,    42,
   167,   168,    42,    42,    42,    42,    42,    42,    42,    42,
    42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
    42,   170,   172,    42,   173,   174,    42,    42,    42,    42,
   175,   176,    42,   177,    42,   179,    42,   180,    42,   181,
   182,    42,   183,   184,    42,    42,    42,    42,    42,    46,
   154,   155,   153,   169,   169,   169,   171,   171,    50,    50,
    47,    47,   132,   132,   134,   134,    90,    90,   135,   135,
   135,   135,   135,   135,   135,   135,   135,    97,    97,    97,
    97,    97,    96,    96,    71,    71,    71,    71,    71,    71,
    71,    71,    71,    71,    71,    71,    71,    71,    71,    73,
   186,    73,    72,    72,    72,   127,   127,   126,   126,   136,
   136,   187,   188,   129,   189,    70,   190,    70,    70,   128,
   128,   115,    60,    60,    60,    60,    22,    22,    22,    22,
    22,    22,    22,    22,    22,   114,   114,   191,   192,   117,
   193,   194,   118,    79,    48,    48,    80,    49,    49,   121,
   121,    81,    81,    81,    51,    51,    54,    54,    28,    28,
    28,    15,    16,    16,    16,    17,    18,    19,    25,    87,
    87,    27,    27,    93,    91,    91,    26,    94,    86,    86,
    92,    92,    20,    20,    21,    21,    24,    24,    23,   195,
    23,   196,   197,   198,   199,   200,    23,    66,    66,    66,
    66,     2,     1,     1,     1,     1,    29,    33,    33,    34,
    34,    34,    34,    58,    58,    58,    58,    58,    58,    58,
    58,    58,    58,    58,    58,   122,   122,   122,   122,   122,
   122,   122,   122,   122,   122,   122,   122,    67,    67,   201,
    56,    56,    74,   202,    74,    98,    98,    98,    98,    98,
    95,    95,    68,    68,    69,    69,    69,    69,    69,    69,
    69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
   148,   138,   138,   138,   138,     9,     9,   151,   125,   125,
    88,    88,   147,    99,    99,   100,   100,   101,   101,   102,
   102,   145,   145,   185,   146,   146,    64,   131,   108,   108,
    89,    89,    10,    10,    13,    13,    12,    12,   113,   113,
   112,   112,    14,   203,    14,   103,   103,   104,   104,   105,
   105,   105,   105,   105,     3,     3,     3,     4,     4,     4,
     4,     5,     5,     5,    11,    11,   149,   149,   150,   150,
   157,   157,   161,   161,   140,   141,   163,   163,   163,   178,
   178,   158,   158,    84,   111,
    }, yyLen = {
//yyLen 675
     2,     0,     2,     2,     1,     1,     3,     2,     1,     4,
     4,     2,     1,     1,     3,     2,     1,     0,     5,     0,
     4,     3,     3,     3,     2,     3,     3,     3,     3,     3,
     4,     1,     3,     3,     5,     3,     1,     3,     3,     6,
     5,     5,     5,     5,     3,     1,     3,     1,     1,     3,
     3,     3,     2,     1,     1,     1,     1,     1,     4,     3,
     1,     2,     3,     4,     5,     4,     5,     2,     2,     2,
     2,     2,     1,     3,     1,     3,     1,     2,     3,     5,
     2,     4,     2,     4,     1,     3,     1,     3,     2,     3,
     1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     4,     3,     3,     3,     3,     2,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     4,     3,     3,     3,     3,     2,     1,
     1,     1,     2,     1,     3,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     0,     4,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
     3,     6,     5,     5,     5,     5,     4,     3,     3,     2,
     2,     3,     2,     2,     3,     3,     3,     3,     3,     3,
     4,     2,     2,     3,     3,     3,     3,     1,     3,     3,
     3,     3,     3,     2,     2,     3,     3,     3,     3,     3,
     6,     1,     1,     1,     1,     1,     3,     3,     1,     1,
     2,     4,     2,     1,     3,     3,     5,     3,     1,     1,
     1,     1,     2,     4,     2,     1,     2,     2,     4,     1,
     0,     2,     2,     1,     2,     1,     1,     2,     3,     4,
     1,     1,     3,     4,     2,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     0,     4,     0,     3,
     0,     0,     5,     3,     3,     2,     3,     3,     1,     4,
     3,     1,     5,     4,     3,     2,     1,     2,     2,     6,
     6,     0,     0,     7,     0,     0,     7,     5,     4,     5,
     0,     0,     9,     0,     6,     0,     7,     0,     5,     0,
     0,     7,     0,     0,     9,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     2,     1,     1,     1,     5,
     1,     2,     1,     1,     1,     3,     1,     3,     1,     4,
     6,     3,     5,     2,     4,     1,     3,     4,     2,     2,
     2,     1,     2,     0,     6,     8,     4,     6,     4,     2,
     6,     2,     4,     6,     2,     4,     2,     4,     1,     1,
     0,     2,     3,     1,     4,     1,     4,     1,     3,     1,
     1,     0,     0,     4,     0,     5,     0,     2,     0,     3,
     3,     3,     2,     4,     5,     5,     2,     4,     4,     3,
     3,     3,     2,     1,     4,     3,     3,     0,     0,     4,
     0,     0,     4,     5,     1,     1,     5,     1,     1,     6,
     0,     1,     1,     1,     2,     1,     2,     1,     1,     1,
     1,     1,     1,     1,     2,     3,     3,     3,     4,     0,
     3,     1,     2,     4,     0,     3,     4,     4,     0,     3,
     0,     3,     0,     2,     0,     2,     0,     2,     1,     0,
     3,     0,     0,     0,     0,     0,     8,     1,     1,     1,
     1,     2,     1,     1,     1,     1,     3,     1,     2,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
     4,     0,     3,     0,     3,     4,     2,     2,     2,     1,
     2,     0,     1,     0,     6,     8,     4,     6,     4,     6,
     2,     4,     6,     2,     4,     2,     4,     1,     3,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
     1,     3,     1,     2,     1,     2,     1,     1,     3,     1,
     3,     1,     1,     2,     2,     1,     3,     3,     1,     3,
     1,     3,     1,     1,     2,     1,     1,     1,     2,     1,
     2,     0,     1,     0,     4,     1,     2,     1,     3,     3,
     2,     1,     4,     2,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     0,     1,     0,     1,     2,     2,     0,     1,     1,     1,
     1,     1,     2,     0,     0,
    }, yyDefRed = {
//yyDefRed 1142
     1,     0,     0,     0,   370,   371,     0,     0,   316,     0,
     0,     0,   341,   344,     0,     0,     0,   367,   368,   372,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   502,   504,   506,     0,     0,
   431,   557,   558,   529,   532,   530,   531,     0,     0,   479,
    60,   306,     0,   483,   307,   308,     0,   309,   310,   305,
   480,    31,    48,   478,   527,     0,     0,     0,     0,     0,
     0,     0,   313,     0,    56,     0,     0,    86,     0,     4,
   311,   312,     0,     0,    72,     0,     2,     0,     5,     0,
     0,     0,     0,     7,   187,   198,   188,   211,   184,   204,
   194,   193,   214,   215,   209,   192,   191,   186,   212,   216,
   217,   196,   185,   199,   203,   205,   197,   190,   206,   213,
   208,     0,     0,     0,     0,   183,   202,   201,   218,   182,
   189,   180,   181,     0,     0,     0,     0,   137,   535,   534,
     0,   537,   172,   173,   169,   150,   151,   152,   159,   156,
   158,   153,   154,   174,   175,   160,   161,   633,   166,   165,
   149,   171,   168,   167,   163,   164,   157,   155,   147,   170,
   148,   176,   162,   138,   359,     0,   632,   139,   207,   200,
   210,   195,   177,   178,   179,   135,   136,   141,   140,   143,
     0,   142,   144,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   669,   670,     0,     0,     0,
   671,     0,     0,   365,   366,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   369,     0,     0,   382,   383,     0,     0,
   328,     0,     0,     0,     0,   502,     0,     0,   285,    70,
     0,     0,     0,   637,   289,    71,     0,    68,     0,     0,
   452,    67,     0,   663,     0,     0,    19,     0,     0,     0,
   241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    13,    12,     0,     0,     0,     0,     0,
   269,     0,     0,     0,   635,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   254,    52,   253,   524,   523,   525,
   521,   522,     0,     0,     0,     0,   489,   498,   338,     0,
   494,   500,   484,   460,   457,   337,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   264,   265,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   263,   262,     0,     0,     0,     0,
   460,   442,   656,   657,     0,     0,     0,     0,   659,   658,
     0,     0,    88,     0,     0,     0,     0,     0,     0,     3,
     0,   446,     0,   335,    69,   539,   538,   540,   541,   543,
   542,   544,     0,     0,     0,     0,   133,     0,     0,   314,
   357,     0,   360,   654,   655,   362,   145,     0,     0,     0,
   374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   672,     0,     0,     0,   528,     0,     0,
     0,     0,   350,   640,   297,   292,     0,   643,     0,     0,
   286,   295,     0,   287,     0,   330,     0,   291,     0,   281,
   280,     0,     0,     0,     0,     0,   334,    51,    21,    23,
    22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   323,    11,     0,     0,   319,     0,   326,
     0,   667,   270,     0,   272,   327,   636,     0,    90,     0,
     0,     0,     0,     0,   511,   509,   526,   508,   505,   485,
   503,   486,   487,   507,     0,     0,   434,   432,     0,     0,
     0,     0,   461,     0,   458,    25,    26,    27,    28,    29,
    49,    50,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   449,     0,   451,     0,     0,     0,   649,     0,     0,   650,
   450,     0,   647,   648,     0,    47,     0,     0,     0,    44,
   227,     0,     0,     0,     0,    37,   219,    33,   296,     0,
     0,     0,     0,    89,    32,     0,   300,     0,    38,   220,
     6,   457,    62,     0,   130,     0,   132,   559,   353,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   317,
     0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   348,   377,   342,   376,   345,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   639,     0,     0,     0,
   294,   638,   329,   664,     0,     0,   275,   277,   333,    20,
     0,     9,    30,     0,   226,     0,     0,    14,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   512,     0,   488,
   491,     0,   496,     0,     0,     0,   596,   593,   592,   591,
   594,   602,   611,   590,     0,   623,   622,   627,   626,   612,
   597,     0,     0,     0,   620,   437,     0,     0,   587,   609,
     0,   569,   600,   595,     0,     0,     0,   589,     0,     0,
   493,     0,   497,     0,   456,     0,   455,     0,     0,   441,
     0,     0,   448,     0,     0,     0,     0,     0,     0,   279,
     0,   447,   278,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   134,     0,     0,   634,     0,     0,     0,
   363,   146,   472,     0,     0,   473,     0,     0,   380,     0,
   378,     0,     0,     0,     0,     0,     0,     0,     0,   347,
   349,     0,     0,     0,     0,     0,     0,   642,   299,   288,
     0,     0,   332,     0,   322,   271,    91,     0,   513,   517,
   518,   519,   510,   520,   490,   492,   499,     0,   572,     0,
     0,   433,     0,     0,   384,     0,   386,     0,   624,   628,
     0,   585,     0,   580,     0,   583,     0,   566,   613,   614,
     0,   567,   603,     0,   568,   495,   501,     0,   419,     0,
     0,     0,    43,   224,    42,   225,    66,     0,   665,    40,
   222,    41,   223,    64,   445,   444,    46,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    34,    59,     0,     0,
     0,   454,   358,     0,     0,     0,     0,     0,     0,   475,
   381,     0,    10,   477,     0,   339,     0,   340,     0,   298,
     0,     0,     0,   351,     0,   276,    18,   514,     0,     0,
     0,     0,     0,     0,     0,     0,   599,     0,   570,   598,
     0,     0,   601,   588,     0,   621,     0,   610,   630,     0,
     0,   616,   462,   423,     0,   421,   459,     0,     0,    39,
     0,     0,     0,   560,   354,   562,   361,   564,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   474,     0,   476,     0,     0,   464,
   463,   465,   343,   346,     0,   515,   435,     0,   440,   439,
   385,     0,     0,     0,   387,     0,   586,     0,   578,     0,
   576,     0,   581,   584,   565,     0,     0,     0,   418,   607,
     0,     0,   401,     0,   618,     0,     0,     0,     0,   356,
     0,     0,     0,     0,     0,     0,     0,   467,   466,   468,
     0,     0,   429,     0,   427,   430,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   416,     0,     0,   411,     0,
   398,     0,   414,   422,   399,     0,     0,     0,     0,     0,
   400,   364,     0,     0,     0,     0,     0,   469,   379,   352,
     0,     0,   426,     0,     0,   579,     0,   574,   577,   582,
     0,   402,   424,     0,     0,   608,     0,     0,     0,   619,
   325,     0,     0,     0,   516,   428,     0,     0,     0,   417,
     0,   408,     0,   406,   397,     0,   412,   415,     0,     0,
   575,     0,     0,     0,     0,   410,     0,   404,   407,   413,
     0,   405,
    }, yyDgoto = {
//yyDgoto 204
     1,   350,    69,    70,   669,   590,   591,   208,   436,   730,
   731,   445,   732,   733,   195,    71,    72,    73,    74,    75,
   353,   352,    76,   538,   355,    77,    78,   711,    79,    80,
   437,    81,    82,    83,    84,   625,   447,   448,   311,   312,
    86,    87,    88,    89,   313,   229,   301,   810,  1000,  1048,
   811,   918,    91,   489,   922,   592,   638,   287,    92,   771,
    93,    94,   615,   616,   734,   210,   842,   231,   847,   848,
   547,  1026,   965,   877,   798,   617,    96,    97,   280,   462,
   660,   804,   319,   232,   314,   593,   545,   544,   736,   737,
   855,   549,   550,   100,   101,   861,  1065,  1101,   948,   739,
  1029,  1030,   740,   325,   492,   283,   102,   529,  1031,   480,
   284,   481,   867,   741,   423,   401,   632,   553,   551,   103,
   104,   648,   233,   211,   212,   742,  1053,   938,   851,   358,
   316,  1034,   268,   493,   856,   857,  1054,   197,   743,   399,
   485,   765,   106,   107,   108,   744,   745,   746,   747,   641,
   410,   949,   269,   270,   111,   112,     2,   238,   239,   511,
   501,   486,   646,   522,   288,   213,   317,   318,   698,   451,
   241,   664,   823,   242,   824,   674,  1004,   790,   452,   788,
   642,   442,   644,   645,   916,   749,   879,   359,   715,   714,
   548,   554,   757,   552,   755,   708,   707,   838,   937,  1005,
  1051,   789,   799,   441,
    }, yySindex = {
//yySindex 1142
     0,     0, 19997, 21580,     0,     0, 19352, 19748,     0, 22759,
 22759, 18556,     0,     0, 23283, 20392, 20392,     0,     0,     0,
  -278,  -196,     0,     0,     0,     0,    91, 19616,   149,  -202,
  -161,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0, 22890, 22890,   867, 22890, 22890,   -92, 20129,     0, 21052,
 21448, 18824, 22890, 23021, 19484,     0,     0,     0,   211,   261,
     0,     0,     0,     0,     0,     0,     0,   269,   288,     0,
     0,     0,   -64,     0,     0,     0,  -222,     0,     0,     0,
     0,     0,     0,     0,     0,   925,    49,  3634,     0,    95,
   699,   477,     0,   481,     0,   -11,   309,     0,   320,     0,
     0,     0, 23414,   345,     0,   116,     0,   135,     0,  -197,
 20392, 23545, 23676,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -105,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   423,     0,     0, 20261,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   160, 20261,    49,   107,
   823,   178,   461,   185,   107,     0,     0,   135,   274,   492,
     0, 22759, 22759,     0,     0,  -278,  -196,     0,     0,     0,
     0,   267,   149,     0,     0,     0,     0,     0,     0,     0,
     0,   867,   314,     0,  1154,     0,     0,     0,   327,  -197,
     0, 22890, 22890, 22890, 22890,     0, 22890,  3634,     0,     0,
   294,   607,   609,     0,     0,     0, 16851,     0, 20392, 20524,
     0,     0, 18691,     0, 22759,   322,     0, 21842, 19997, 20261,
     0,  1175,   351,   357,  3720,  3720,   338, 21711,     0, 20129,
   337,   135,   925,     0,     0,     0,   149,   149, 21711,   335,
     0,   158,   163,   294,     0,   319,   163,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
 23807,  1248,     0,   651,     0,     0,     0,     0,     0,     0,
     0,     0,   612,   827,  1214,   323,     0,     0,     0,   369,
     0,     0,     0,     0,     0,     0, 22759, 22759, 22759, 22759,
 21711, 22759, 22759, 22890, 22890, 22890, 22890, 22890,     0,     0,
 22890, 22890, 22890, 22890, 22890, 22890, 22890, 22890, 22890, 22890,
 22890, 22890, 22890, 22890,     0,     0, 22890, 22890, 22890, 22890,
     0,     0,     0,     0,  4093, 20392,  4601, 22890,     0,     0,
 24673, 23021,     0, 21973, 20129, 18959,   675, 21973, 23021,     0,
 19090,     0,   375,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0, 22759,    -4,     0,   367,  1276,     0,
     0, 22759,     0,     0,     0,     0,     0,   469,   479,   338,
     0, 20261,   466,  5201, 20392,  5667, 22890, 22890, 22890, 20261,
    29, 22104,   485,     0,    75,    75,   402,     0,     0,  6089,
 20392,  8232,     0,     0,     0,     0,   754,     0, 22890, 20656,
     0,     0, 21184,     0,   149,     0,   409,     0, 22890,     0,
     0,   719,   720,   149,   149,   272,     0,     0,     0,     0,
     0, 19748, 22759,  3634,   405,   410,  5201,  5667, 22890, 22890,
   925,   403,   149,     0,     0, 19221,     0,     0,   925,     0,
 21316,     0,     0, 21448,     0,     0,     0,     0,     0,   728,
 24151, 20392, 24209, 23807,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  1243,   233,     0,     0,  4179,  1345,
   264,   513,     0,   416,     0,     0,     0,     0,     0,     0,
     0,     0,   351,  3241,  3241,  3241,  3241,  4736,  4228,  3241,
  3241,  3720,  3720,   457,   457,   351,  2645,   351,   351,   815,
   815,  2673,  2673,  6598,  3095,   521,   452,     0,   454,  -196,
     0,     0,     0,   751,   149,   505,     0,   510,  -196,     0,
     0,  3095,     0,     0,  -196,     0,   500,  3164,  1303,     0,
     0,   -11,   745, 22890,  3164,     0,     0,     0,     0,   808,
   149, 23807,   810,     0,     0,   528,     0,     0,     0,     0,
     0,     0,     0,    49,     0,     0,     0,     0,     0, 24267,
 20392, 24325, 20261,   272,   518, 19880, 19748, 22235,   587,     0,
   300,     0,   522,   529,   149,   530,   532,   587, 22104,   604,
   615,   710,     0,     0,     0,     0,     0,     0,     0,  -196,
   149,     0,     0,  -196, 22759, 22890,     0, 22890,   294,   609,
     0,     0,     0,     0, 20788, 21184,     0,     0,     0,     0,
   272,     0,     0,   351,     0, 19997,     0,     0,   149,   163,
 23807,     0,     0,   149,     0,     0,   728,     0,   717,     0,
     0,    10,     0,   844,  4179,  -151,     0,     0,     0,     0,
     0,     0,     0,     0,  1533,     0,     0,     0,     0,     0,
     0,   584,   586,   849,     0,     0,   854,   857,     0,     0,
   866,     0,     0,     0,   -27,   868, 22890,     0,   852,   868,
     0,   159,     0,   884,     0,     0,     0,     0,   864,     0,
 23021, 23021,     0,   375, 20656,   585,   581, 23021, 23021,     0,
   375,     0,     0,    95,  -222, 21711, 22890, 24383, 20392, 24441,
 23021,     0, 22366,     0,   728, 23807, 21711,   567,   135, 22759,
 20261,     0,     0,     0,   149,   685,     0,  4179, 20261,  4179,
     0,     0,     0,     0,   603,     0, 20261,   698,     0, 22759,
     0,   700, 22890, 22890,   623, 22890, 22890,   702,   710,     0,
     0, 22497, 20261, 20261, 20261,     0,    75,     0,     0,     0,
   924,   149,     0,   605,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   149,     0, 20261,
 20261,     0,  1533,   549,     0,   935,     0,   149,     0,     0,
  1927,     0,  4179,     0,  4687,     0,   842,     0,     0,     0,
   290,     0,     0, 22890,     0,     0,     0, 20261,     0,  -140,
 20261, 22890,     0,     0,     0,     0,     0, 23021,     0,     0,
     0,     0,     0,     0,     0,     0,     0,  3634,   452,   454,
   149,   505,   510, 22890,     0,   728,     0,     0, 20261,   135,
   718,     0,     0,   149,   725,   135,   518, 23938,   107,     0,
     0, 20261,     0,     0,   107,     0, 22890,     0, 20261,     0,
   316,   730,   733,     0, 21184,     0,     0,     0,   639,   941,
   737,   642,   149,  2297,   967,  2236,     0,   968,     0,     0,
   975,   976,     0,     0,   978,     0,   968,     0,     0,   723,
   868,     0,     0,     0,   663,     0,     0,  3634,  3634,     0,
   585,     0,   759,     0,     0,     0,     0,     0, 20261,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   721,  1307,     0,     0, 20261,     0, 20261,   -31,     0,
     0,     0,     0,     0, 20261,     0,     0,  1616,     0,     0,
     0,   995,  2297,   625,     0,  1927,     0,  1927,     0,  4687,
     0,  1927,     0,     0,     0,   998,   149,  1004,     0,     0,
  1009,  1010,     0,   694,     0,   868, 24069,  1006,   868,     0,
   795,     0, 24499, 20392, 24557,   469,   300,     0,     0,     0,
   801, 20261,     0,   202,     0,     0,  2297,   995,  2297,  1024,
   968,  1028,   968,   968,  1927,     0,   712,  4687,     0,   842,
     0,  4687,     0,     0,     0,     0,     0,   768,  1439, 24069,
     0,     0,     0,     0,   149,     0,     0,     0,     0,     0,
   690,  1616,     0,   995,  2297,     0,  1927,     0,     0,     0,
  1036,     0,     0,  1040,  1043,     0,   868,  1044,  1036,     0,
     0, 24615,  1439,     0,     0,     0,   995,   968,  1927,     0,
  1927,     0,  4687,     0,     0,  1927,     0,     0,     0,     0,
     0,  1036,  1045,  1036,  1036,     0,  1927,     0,     0,     0,
  1036,     0,
    }, yyRindex = {
//yyRindex 1142
     0,     0,   161,     0,     0,     0,     0,     0,     0,     0,
     0,   822,     0,     0,     0, 10675, 10812,     0,     0,     0,
  5486,  5020, 12220, 12324, 12435, 12572, 23152,     0, 22628,     0,
     0, 12676, 12787, 12924,  5819,  4004, 13028, 13139,  5952, 13276,
     0,     0,     0,     0,     0,     0,     0,   139, 18422,   753,
   736,   114,     0,     0,  1367,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0, 10108,     0,     0,     0, 10212,     0,     0,     0,
     0,     0,     0,     0,     0,    72, 16729,  7210, 10323,  7718,
     0, 13380,     0, 16527,     0, 13491,     0,     0,     0,     0,
     0,     0,   263,     0,     0,     0,     0,    36,     0, 20920,
 10916,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,  1484,  2710,  3091,  5126,     0,     0,     0,     0,     0,
     0,     0,     0,  5592,  6041,  6550,  7058,     0,     0,     0,
  7566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
 16787,     0,     0,  1064,  8452,  8563,  8700,  8804,  8915,  9052,
  9156,  2852,  9267,  9404,  2988,  9508,     0,   139, 16674,     0,
     0,  9860,     0,     0,     0,     0,     0,    80,     0,    92,
     0,     0,     0,     0,     0, 10460,  9619,  1146,  1172,  1301,
  1435,     0,   765,  1700,  1713,  1756,  1322,  1938,  2474,  1785,
  2531,     0,     0,     0,     0,  3277,     0,     0,     0,     0,
     0,  1302,     0,  3738,     0,     0,     0, 16299,     0,     0,
 16427,  4246,  4246,     0,     0,     0,   769,     0,     0,   166,
     0,     0,   769,     0,     0,     0,     0,     0,    37,    37,
     0,     0, 11268, 10564,  1918,  2553, 13628,     0, 18018,   139,
     0,  2689,  1282,     0,     0,    38,   769,   769,     0,     0,
     0,   770,   770,     0,     0,     0,   750,  1471,  1611,  1677,
  2131,  2300,  9479,  9831,  1663, 10183, 10535,  1998, 10887,     0,
     0,     0, 11239,   273,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,  3589,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0, 11027, 11164,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    40,     0,     0,     0,     0,
     0,     0,     0,     0,   139,   344,   538,     0,     0,     0,
    71,     0,  4754,     0,     0,     0,     0,     0,     0,     0,
     0,     0, 16984, 17120,     0,     0,     0, 18153,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   570,     0,  9971,
     0,   488, 17748,     0,    40,     0,     0,     0,     0,   673,
     0,     0,     0,     0,     0,     0,     0,     0,  3785,     0,
    40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   769,     0,     0,     0,   170,     0,
     0,   183,    52,   769,   769,   769,     0,     0,     0,     0,
     0,     0,     0, 15935,     0,     0,     0,     0,     0,     0,
  1414,     0,   769,     0,     0,  3127,    46,     0,   194,     0,
   778,     0,     0,   -52,     0,     0,     0, 11591,     0,   616,
     0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0, 11379, 14909, 15032, 15149, 15249, 15340, 15691, 15489,
 15600, 15781, 15845, 14084, 14195, 11516, 14332, 11620, 11731, 13732,
 13843, 14436, 14547,  1084, 14680,     0,  6327,  4379,  7851, 20920,
     0,  4512,     0,    47,   796,  6460,     0,  6835,  5353,     0,
     0, 14792,     0,     0,  2394,     0,  1840, 16363,     0,     0,
     0, 13980,     0,     0, 15404,     0,     0,     0,     0,     0,
   769,     0,   618,     0,     0,  2023,     0,  2061,     0,     0,
     0,     0,     0,   150,     0, 17614,     0,     0,     0,     0,
    40,     0,  1064,   769,  8072,     0,     0,   617,   407,     0,
   883,     0,  3363,  4887,   796,  3496,  3871,   883,     0,     0,
     0,     0,     0,     0,     0,     0,     0,  2099,  1299,     0,
   796,  2601,  3198,  9756,     0,     0,     0,     0, 16487,  4246,
     0,     0,     0,     0,   206,   129,     0,     0,     0,     0,
   769,     0,     0, 11868,     0,    37,   134,     0,   769,   770,
     0,  2085,  1065,   796,  2197,  2529,   644,     0,     0,     0,
     0,     0,     0,     0,   212,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   221,   268,   200,     0,     0,   200,   200,     0,     0,
   244,     0,     0,     0,   271,   244,    53,     0,   120,   244,
     0,     0,     0,     0,     0, 17884,     0, 18287,     0,     0,
     0,     0,     0,  6194,    57, 11972,     0,     0,     0,     0,
  6702,     0,     0, 16591,  1152,     0,     0,     0,    40,     0,
     0,   843,     0,     0,   677,     0,     0,     0,     0,     0,
  1064, 17300, 17434,     0,   796,     0,     0,   214,  1064,   155,
     0,     0,     0,   171,   606,     0,   781,   883,     0,     0,
     0,     0,     0,     0,  8348,     0,     0,     0,     0,     0,
     0,     0,   640,   191,   191,   671,     0,     0,     0,     0,
    52,   769,     0,     0,     0,     0,     0,  2727,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     2,     0,  1064,
    37,     0,     0,   222,     0,   226,     0,   769,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,  1064,     0,     0,
    37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0, 16023,  6968,  7984,
   796,  7343,  7476,     0,  1690,   680,     0,     0,  1064,     0,
     0,     0,     0,   769,     0,     0,  8072,     0,     0,     0,
     0,   191,     0,     0,     0,     0,     0,     0,   449,     0,
   883,     0,     0,     0,   143,     0,     0,     0,     0,   -70,
     0,     0,   769,     0,   231,     0,     0,   200,     0,     0,
   200,   200,     0,     0,   200,     0,   200,     0,     0,   271,
   244,     0,     0,     0,    -6,     0,     0, 16123, 16211,     0,
 12083, 16655,     0,     0,     0,     0,     0,     0,  1064,  1779,
  2151,  2246,  2500,  2523,  2526,  2636,   919,  8389,  8427,   947,
  8457,     0,     0,  8599,     0,  1064,     0,   488,   883,     0,
     0,     0,     0,     0,   191,     0,     0,     0,     0,     0,
     0,   234,     0,   235,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   130,    -6,   130,     0,     0,
   140,   130,     0,     0,     0,   140,   111,   125,   140,     0,
     0,  8671,     0,    40,     0,   570,   883,     0,     0,     0,
     0,    25,     0,    17,     0,     0,     0,   237,     0,   246,
   200,   200,   200,   200,     0,     0,     0,   141,     0,     0,
     0,     0,     0,     0,     0,  2010, 13918,     0,   128,     0,
     0,     0,  1810,  1565,   796,  2461,  2463,     0,     0,     0,
     0,     0,     0,   253,     0,     0,     0,     0,     0,     0,
   130,     0,     0,   130,   130,     0,   140,   130,   130,     0,
     0,     0,   138,   543,     0,     0,   260,   200,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0, 14577,  1394,
     0,   130,   130,   130,   130,     0,     0,     0,     0,     0,
   130,     0,
    }, yyGindex = {
//yyGindex 204
     0,     0,     3,     0,  -218,     0,    14,    22,  -354,  -113,
     0,     0,     0,   318,     0,     0,     0,  1076,     0,     0,
   878,  1099,     0,  -280,     0,     0,     0,   608,     0,    16,
  1046,  -286,   -38,     0,    27,     0,   437,   391,     0,    30,
    94,  2110,     4,    23,   648,    77,    -2,  -406,     0,     0,
   131,     0,     0,   764,     0,    48,     0,   -15,  1169,   589,
     0,     0,  -320,   509,  -800,     0,     0,   296,  -177,   637,
     0,     0,     0,   431,   275,  -371,   -91,   -20,   385,  -435,
   198,     0,     0,  1250,    87,   103,     0,     0, 11855,   341,
  -737,     0,     0,     0,     0,   218,  2439,   241,  -437,   331,
   146,     0,     0,     0,    64,  -428,     0,  -413,   145,  -258,
  -423,     0,    86,  1970,   -72,   455,  -421,   579,   838,  1225,
    13,   196,  1553,     0,    -5,  -229,     0,  -821,     0,     0,
  -186,  -993,     0,  -368,  -879,   390,   174,     0,  -933,  1163,
   917,  -577,  -269,     0,    31,  -814,  1186,   398,  -262,   -74,
     0,  -531,   937,  1088,     0,     0,     0,    21,    34,     0,
     0,   -23,     0,  -282,     0,     0,     0,     0,     0,  -232,
     0,  -373,     0,     0,     0,     0,     0,     0,    15,     0,
     0,     0,     0,     0,     0,  1841,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,
    };
    protected static final short[] yyTable = YyTables.yyTable();
    protected static final short[] yyCheck = YyTables.yyCheck();

  /** maps symbol value to printable name.
      @see #yyExpecting
    */
  protected static final String[] yyNames = {
    "end-of-file",null,null,null,null,null,null,null,null,null,"'\\n'",
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,"' '",null,null,null,null,null,
    null,null,null,null,null,null,"','",null,null,null,null,null,null,
    null,null,null,null,null,null,null,"':'","';'",null,"'='",null,"'?'",
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,
    "'['",null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,null,null,
    "keyword_class","keyword_module","keyword_def","keyword_undef",
    "keyword_begin","keyword_rescue","keyword_ensure","keyword_end",
    "keyword_if","keyword_unless","keyword_then","keyword_elsif",
    "keyword_else","keyword_case","keyword_when","keyword_while",
    "keyword_until","keyword_for","keyword_break","keyword_next",
    "keyword_redo","keyword_retry","keyword_in","keyword_do",
    "keyword_do_cond","keyword_do_block","keyword_return","keyword_yield",
    "keyword_super","keyword_self","keyword_nil","keyword_true",
    "keyword_false","keyword_and","keyword_or","keyword_not",
    "modifier_if","modifier_unless","modifier_while","modifier_until",
    "modifier_rescue","keyword_alias","keyword_defined","keyword_BEGIN",
    "keyword_END","keyword__LINE__","keyword__FILE__",
    "keyword__ENCODING__","keyword_do_lambda","tIDENTIFIER","tFID",
    "tGVAR","tIVAR","tCONSTANT","tCVAR","tLABEL","tCHAR","unary+",
"unary-","tUMINUS_NUM","'**'","'<=>'","'=='","'==='","'!='","'>='",
"'<='","'&&'","'||'","'=~'","'!~'","'.'","'..'","'...'",
    "tBDOT2","tBDOT3","'[]'","'[]='","'<<'","'>>'","'&.'",
"'::'","':: at EXPR_BEG'","tOP_ASGN","'=>'","'('","'( arg'",
"')'","'['","'{'","'{ arg'","'['","'[ args'",
"'*'","'*'","'&'","'&'","'~'","'%'","'/'",
"'+'","'-'","'<'","'>'","'|'","'!'","'^'","'{'",
"'}'","'`'","':'","tSTRING_BEG","tXSTRING_BEG",
    "tREGEXP_BEG","tWORDS_BEG","tQWORDS_BEG","tSTRING_DBEG",
    "tSTRING_DVAR","tSTRING_END","'->'","tLAMBEG","tNTH_REF",
    "tBACK_REF","tSTRING_CONTENT","tINTEGER","tIMAGINARY","tFLOAT",
    "tRATIONAL","tREGEXP_END","tSYMBOLS_BEG","tQSYMBOLS_BEG","'**'",
    "tSTRING_DEND","tLABEL_END","tLOWEST",
    };


  /** computes list of expected tokens on error by tracing the tables.
      @param state for which to compute the list.
      @return list of token names.
    */
  protected String[] yyExpecting (int state) {
    int token, n, len = 0;
    boolean[] ok = new boolean[yyNames.length];

    if ((n = yySindex[state]) != 0)
      for (token = n < 0 ? -n : 0;
           token < yyNames.length && n+token < yyTable.length; ++ token)
        if (yyCheck[n+token] == token && !ok[token] && yyNames[token] != null) {
          ++ len;
          ok[token] = true;
        }
    if ((n = yyRindex[state]) != 0)
      for (token = n < 0 ? -n : 0;
           token < yyNames.length && n+token < yyTable.length; ++ token)
        if (yyCheck[n+token] == token && !ok[token] && yyNames[token] != null) {
          ++ len;
          ok[token] = true;
        }

    String result[] = new String[len];
    for (n = token = 0; n < len;  ++ token)
      if (ok[token]) result[n++] = yyNames[token];
    return result;
  }

  /** the generated parser, with debugging messages.
      Maintains a dynamic state and value stack.
      @param yyLex scanner.
      @return result of the last reduction, if any.
    */
  public Object yyparse (RubyLexer yyLex, Object ayydebug) {
    return yyparse(yyLex);
  }

  /** initial size and increment of the state/value stack [default 256].
      This is not final so that it can be overwritten outside of invocations
      of {@link #yyparse}.
    */
  protected int yyMax;

  /** executed at the beginning of a reduce action.
      Used as <tt>$$ = yyDefault($1)</tt>, prior to the user-specified action, if any.
      Can be overwritten to provide deep copy, etc.
      @param first value for <tt>$1</tt>, or <tt>null</tt>.
      @return first.
    */
  protected Object yyDefault (Object first) {
    return first;
  }

  /** the generated parser.
      Maintains a dynamic state and value stack.
      @param yyLex scanner.
      @return result of the last reduction, if any.
    */
  public Object yyparse (RubyLexer yyLex) {
    if (yyMax <= 0) yyMax = 256;			// initial size
    int yyState = 0, yyStates[] = new int[yyMax];	// state stack
    Object yyVal = null, yyVals[] = new Object[yyMax];	// value stack
    int yyToken = -1;					// current input
    int yyErrorFlag = 0;				// #tokens to shift

    yyLoop: for (int yyTop = 0;; ++ yyTop) {
      if (yyTop >= yyStates.length) {			// dynamically increase
        int[] i = new int[yyStates.length+yyMax];
        System.arraycopy(yyStates, 0, i, 0, yyStates.length);
        yyStates = i;
        Object[] o = new Object[yyVals.length+yyMax];
        System.arraycopy(yyVals, 0, o, 0, yyVals.length);
        yyVals = o;
      }
      yyStates[yyTop] = yyState;
      yyVals[yyTop] = yyVal;

      yyDiscarded: for (;;) {	// discarding a token does not change stack
        int yyN;
        if ((yyN = yyDefRed[yyState]) == 0) {	// else [default] reduce (yyN)
          if (yyToken < 0) {
//            yyToken = yyLex.advance() ? yyLex.token() : 0;
            yyToken = yyLex.nextToken();
          }
          if ((yyN = yySindex[yyState]) != 0 && (yyN += yyToken) >= 0
              && yyN < yyTable.length && yyCheck[yyN] == yyToken) {
            yyState = yyTable[yyN];		// shift to yyN
            yyVal = yyLex.value();
            yyToken = -1;
            if (yyErrorFlag > 0) -- yyErrorFlag;
            continue yyLoop;
          }
          if ((yyN = yyRindex[yyState]) != 0 && (yyN += yyToken) >= 0
              && yyN < yyTable.length && yyCheck[yyN] == yyToken)
            yyN = yyTable[yyN];			// reduce (yyN)
          else
            switch (yyErrorFlag) {
  
            case 0:
              support.yyerror("syntax error", yyExpecting(yyState), yyNames[yyToken]);
              break;
  
            case 1: case 2:
              yyErrorFlag = 3;
              do {
                if ((yyN = yySindex[yyStates[yyTop]]) != 0
                    && (yyN += yyErrorCode) >= 0 && yyN < yyTable.length
                    && yyCheck[yyN] == yyErrorCode) {
                  yyState = yyTable[yyN];
                  yyVal = yyLex.value();
                  continue yyLoop;
                }
              } while (-- yyTop >= 0);
              support.yyerror("irrecoverable syntax error");
              break;

            case 3:
              if (yyToken == 0) {
                support.yyerror("irrecoverable syntax error at end-of-file");
              }
              yyToken = -1;
              continue yyDiscarded;		// leave stack alone
            }
        }
        int yyV = yyTop + 1-yyLen[yyN];
        ParserState state = states[yyN];
        if (state == null) {
            yyVal = yyDefault(yyV > yyTop ? null : yyVals[yyV]);
        } else {
            yyVal = state.execute(support, lexer, yyVal, yyVals, yyTop);
        }
//        switch (yyN) {
// ACTIONS_END
//        }
        yyTop -= yyLen[yyN];
        yyState = yyStates[yyTop];
        int yyM = yyLhs[yyN];
        if (yyState == 0 && yyM == 0) {
          yyState = yyFinal;
          if (yyToken < 0) {
            yyToken = yyLex.nextToken();
//            yyToken = yyLex.advance() ? yyLex.token() : 0;
          }
          if (yyToken == 0) {
            return yyVal;
          }
          continue yyLoop;
        }
        if ((yyN = yyGindex[yyM]) != 0 && (yyN += yyState) >= 0
            && yyN < yyTable.length && yyCheck[yyN] == yyState)
          yyState = yyTable[yyN];
        else
          yyState = yyDgoto[yyM];
        continue yyLoop;
      }
    }
  }

static ParserState[] states = new ParserState[675];
static {
states[1] = (support, lexer, yyVal, yyVals, yyTop) -> {
    lexer.setState(EXPR_BEG);
    support.initTopLocalVariables();
    return yyVal;
};
states[2] = (support, lexer, yyVal, yyVals, yyTop) -> {
  /* ENEBO: Removed !compile_for_eval which probably is to reduce warnings*/
                  if (((ParseNode)yyVals[0+yyTop]) != null) {
                      /* last expression should not be void */
                      if (((ParseNode)yyVals[0+yyTop]) instanceof BlockParseNode) {
                          support.checkUselessStatement(((BlockParseNode)yyVals[0+yyTop]).getLast());
                      } else {
                          support.checkUselessStatement(((ParseNode)yyVals[0+yyTop]));
                      }
                  }
                  support.getResult().setAST(support.addRootNode(((ParseNode)yyVals[0+yyTop])));
    return yyVal;
};
states[3] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (((ParseNode)yyVals[-1+yyTop]) instanceof BlockParseNode) {
        support.checkUselessStatements(((BlockParseNode)yyVals[-1+yyTop]));
    }
    yyVal = ((ParseNode)yyVals[-1+yyTop]);
    return yyVal;
};
states[5] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.newline_node(((ParseNode)yyVals[0+yyTop]), support.getPosition(((ParseNode)yyVals[0+yyTop])));
    return yyVal;
};
states[6] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.appendToBlock(((ParseNode)yyVals[-2+yyTop]), support.newline_node(((ParseNode)yyVals[0+yyTop]), support.getPosition(((ParseNode)yyVals[0+yyTop]))));
    return yyVal;
};
states[7] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((ParseNode)yyVals[0+yyTop]);
    return yyVal;
};
states[9] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.getResult().addBeginNode(new PreExe19ParseNode(((SourceIndexLength)yyVals[-3+yyTop]), support.getCurrentScope(), ((ParseNode)yyVals[-1+yyTop])));
    yyVal = null;
    return yyVal;
};
states[10] = (support, lexer, yyVal, yyVals, yyTop) -> {
    ParseNode node = ((ParseNode)yyVals[-3+yyTop]);

    if (((RescueBodyParseNode)yyVals[-2+yyTop]) != null) {
        node = new RescueParseNode(support.getPosition(((ParseNode)yyVals[-3+yyTop])), ((ParseNode)yyVals[-3+yyTop]), ((RescueBodyParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[-1+yyTop]));
    } else if (((ParseNode)yyVals[-1+yyTop]) != null) {
        support.warn(support.getPosition(((ParseNode)yyVals[-3+yyTop])), "else without rescue is useless");
        node = support.appendToBlock(((ParseNode)yyVals[-3+yyTop]), ((ParseNode)yyVals[-1+yyTop]));
    }
    if (((ParseNode)yyVals[0+yyTop]) != null) {
        if (node != null) {
            node = new EnsureParseNode(support.extendedUntil(support.getPosition(((ParseNode)yyVals[-3+yyTop])), support.getPosition(((ParseNode)yyVals[0+yyTop]))), support.makeNullNil(node), ((ParseNode)yyVals[0+yyTop]));
        } else {
            node = support.appendToBlock(((ParseNode)yyVals[0+yyTop]), NilImplicitParseNode.NIL);
        }
    }

    yyVal = node;
    return yyVal;
};
states[11] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (((ParseNode)yyVals[-1+yyTop]) instanceof BlockParseNode) {
        support.checkUselessStatements(((BlockParseNode)yyVals[-1+yyTop]));
    }
    yyVal = ((ParseNode)yyVals[-1+yyTop]);
    return yyVal;
};
states[13] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.newline_node(((ParseNode)yyVals[0+yyTop]), support.getPosition(((ParseNode)yyVals[0+yyTop])));
    return yyVal;
};
states[14] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.appendToBlock(((ParseNode)yyVals[-2+yyTop]), support.newline_node(((ParseNode)yyVals[0+yyTop]), support.getPosition(((ParseNode)yyVals[0+yyTop]))));
    return yyVal;
};
states[15] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((ParseNode)yyVals[0+yyTop]);
    return yyVal;
};
states[16] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((ParseNode)yyVals[0+yyTop]);
    return yyVal;
};
states[17] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.yyerror("BEGIN is permitted only at toplevel");
    return yyVal;
};
states[18] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new BeginParseNode(((SourceIndexLength)yyVals[-4+yyTop]), support.makeNullNil(((ParseNode)yyVals[-3+yyTop])));
    return yyVal;
};
states[19] = (support, lexer, yyVal, yyVals, yyTop) -> {
    lexer.setState(EXPR_FNAME|EXPR_FITEM);
    return yyVal;
};
states[20] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.newAlias(((SourceIndexLength)yyVals[-3+yyTop]), ((ParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[21] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new VAliasParseNode(((SourceIndexLength)yyVals[-2+yyTop]), support.symbolID(((TruffleString)yyVals[-1+yyTop])), support.symbolID(((TruffleString)yyVals[0+yyTop])));
    return yyVal;
};
states[22] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new VAliasParseNode(((SourceIndexLength)yyVals[-2+yyTop]), support.symbolID(((TruffleString)yyVals[-1+yyTop])), support.symbolID(((BackRefParseNode)yyVals[0+yyTop]).getByteName()));
    return yyVal;
};
states[23] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.yyerror("can't make alias for the number variables");
    return yyVal;
};
states[24] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((ParseNode)yyVals[0+yyTop]);
    return yyVal;
};
states[25] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new IfParseNode(support.getPosition(((ParseNode)yyVals[-2+yyTop])), support.getConditionNode(((ParseNode)yyVals[0+yyTop])), ((ParseNode)yyVals[-2+yyTop]), null);
    ((ParseNode)yyVal).extendPosition(((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[26] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new IfParseNode(support.getPosition(((ParseNode)yyVals[-2+yyTop])), support.getConditionNode(((ParseNode)yyVals[0+yyTop])), null, ((ParseNode)yyVals[-2+yyTop]));
    ((ParseNode)yyVal).extendPosition(((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[27] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (((ParseNode)yyVals[-2+yyTop]) != null && ((ParseNode)yyVals[-2+yyTop]) instanceof BeginParseNode) {
        yyVal = new WhileParseNode(support.getPosition(((ParseNode)yyVals[-2+yyTop])), support.getConditionNode(((ParseNode)yyVals[0+yyTop])), ((BeginParseNode)yyVals[-2+yyTop]).getBodyNode(), false);
    } else {
        yyVal = new WhileParseNode(support.getPosition(((ParseNode)yyVals[-2+yyTop])), support.getConditionNode(((ParseNode)yyVals[0+yyTop])), ((ParseNode)yyVals[-2+yyTop]), true);
    }
    return yyVal;
};
states[28] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (((ParseNode)yyVals[-2+yyTop]) != null && ((ParseNode)yyVals[-2+yyTop]) instanceof BeginParseNode) {
        yyVal = new UntilParseNode(support.getPosition(((ParseNode)yyVals[-2+yyTop])), support.getConditionNode(((ParseNode)yyVals[0+yyTop])), ((BeginParseNode)yyVals[-2+yyTop]).getBodyNode(), false);
    } else {
        yyVal = new UntilParseNode(support.getPosition(((ParseNode)yyVals[-2+yyTop])), support.getConditionNode(((ParseNode)yyVals[0+yyTop])), ((ParseNode)yyVals[-2+yyTop]), true);
    }
    return yyVal;
};
states[29] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.newRescueModNode(((ParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[30] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (support.isInDef()) {
        support.warn(((SourceIndexLength)yyVals[-3+yyTop]), "END in method; use at_exit");
    }
    yyVal = new PostExeParseNode(((SourceIndexLength)yyVals[-3+yyTop]), ((ParseNode)yyVals[-1+yyTop]));
    return yyVal;
};
states[32] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));
    ((MultipleAsgnParseNode)yyVals[-2+yyTop]).setValueNode(((ParseNode)yyVals[0+yyTop]));
    yyVal = ((MultipleAsgnParseNode)yyVals[-2+yyTop]);
    return yyVal;
};
states[33] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));
    yyVal = support.node_assign(((ParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[34] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[-2+yyTop]));
    yyVal = support.node_assign(((MultipleAsgnParseNode)yyVals[-4+yyTop]), support.newRescueModNode(((ParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop])));
    return yyVal;
};
states[35] = (support, lexer, yyVal, yyVals, yyTop) -> {
    ((AssignableParseNode)yyVals[-2+yyTop]).setValueNode(((ParseNode)yyVals[0+yyTop]));
    yyVal = ((MultipleAsgnParseNode)yyVals[-2+yyTop]);
    return yyVal;
};
states[37] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));
    yyVal = support.node_assign(((ParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[38] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));

    SourceIndexLength pos = ((AssignableParseNode)yyVals[-2+yyTop]).getPosition();
    TruffleString asgnOp = ((TruffleString)yyVals[-1+yyTop]);
    if (asgnOp == TStringConstants.OR_OR) {
        ((AssignableParseNode)yyVals[-2+yyTop]).setValueNode(((ParseNode)yyVals[0+yyTop]));
        yyVal = new OpAsgnOrParseNode(pos, support.gettable2(((AssignableParseNode)yyVals[-2+yyTop])), ((AssignableParseNode)yyVals[-2+yyTop]));
    } else if (asgnOp == TStringConstants.AMPERSAND_AMPERSAND) {
        ((AssignableParseNode)yyVals[-2+yyTop]).setValueNode(((ParseNode)yyVals[0+yyTop]));
        yyVal = new OpAsgnAndParseNode(pos, support.gettable2(((AssignableParseNode)yyVals[-2+yyTop])), ((AssignableParseNode)yyVals[-2+yyTop]));
    } else {
        ((AssignableParseNode)yyVals[-2+yyTop]).setValueNode(support.getOperatorCallNode(support.gettable2(((AssignableParseNode)yyVals[-2+yyTop])), asgnOp, ((ParseNode)yyVals[0+yyTop])));
        yyVal = ((AssignableParseNode)yyVals[-2+yyTop]);
    }
    return yyVal;
};
states[39] = (support, lexer, yyVal, yyVals, yyTop) -> {
  /* FIXME: arg_concat logic missing for opt_call_args*/
                    yyVal = support.new_opElementAsgnNode(((ParseNode)yyVals[-5+yyTop]), ((TruffleString)yyVals[-1+yyTop]), ((ParseNode)yyVals[-3+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[40] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));
    yyVal = support.newOpAsgn(support.getPosition(((ParseNode)yyVals[-4+yyTop])), ((ParseNode)yyVals[-4+yyTop]), ((TruffleString)yyVals[-3+yyTop]), ((ParseNode)yyVals[0+yyTop]), ((TruffleString)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]));
    return yyVal;
};
states[41] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));
    yyVal = support.newOpAsgn(support.getPosition(((ParseNode)yyVals[-4+yyTop])), ((ParseNode)yyVals[-4+yyTop]), ((TruffleString)yyVals[-3+yyTop]), ((ParseNode)yyVals[0+yyTop]), ((TruffleString)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]));
    return yyVal;
};
states[42] = (support, lexer, yyVal, yyVals, yyTop) -> {
    SourceIndexLength pos = ((ParseNode)yyVals[-4+yyTop]).getPosition();
    yyVal = support.newOpConstAsgn(pos, support.new_colon2(pos, ((ParseNode)yyVals[-4+yyTop]), ((TruffleString)yyVals[-2+yyTop])), ((TruffleString)yyVals[-1+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[43] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));
    yyVal = support.newOpAsgn(support.getPosition(((ParseNode)yyVals[-4+yyTop])), ((ParseNode)yyVals[-4+yyTop]), ((TruffleString)yyVals[-3+yyTop]), ((ParseNode)yyVals[0+yyTop]), ((TruffleString)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]));
    return yyVal;
};
states[44] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.backrefAssignError(((ParseNode)yyVals[-2+yyTop]));
    return yyVal;
};
states[45] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));
    yyVal = ((ParseNode)yyVals[0+yyTop]);
    return yyVal;
};
states[46] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[-2+yyTop]));
    yyVal = support.newRescueModNode(((ParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[49] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.newAndNode(support.getPosition(((ParseNode)yyVals[-2+yyTop])), ((ParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[50] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.newOrNode(support.getPosition(((ParseNode)yyVals[-2+yyTop])), ((ParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[51] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.getOperatorCallNode(support.getConditionNode(((ParseNode)yyVals[0+yyTop])), TStringConstants.BANG);
    return yyVal;
};
states[52] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.getOperatorCallNode(support.getConditionNode(((ParseNode)yyVals[0+yyTop])), ((TruffleString)yyVals[-1+yyTop]));
    return yyVal;
};
states[54] = (support, lexer, yyVal, yyVals, yyTop) -> {
    value_expr(lexer, ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[58] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.new_call(((ParseNode)yyVals[-3+yyTop]), ((TruffleString)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]), ((ParseNode)yyVals[0+yyTop]), null);
    return yyVal;
};
states[59] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((IterParseNode)yyVals[-1+yyTop]);
    return yyVal;
};
states[60] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.new_fcall(((TruffleString)yyVals[0+yyTop]));
    return yyVal;
};
states[61] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.frobnicate_fcall_args(((FCallParseNode)yyVals[-1+yyTop]), ((ParseNode)yyVals[0+yyTop]), null);
    yyVal = ((FCallParseNode)yyVals[-1+yyTop]);
    return yyVal;
};
states[62] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.frobnicate_fcall_args(((FCallParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[-1+yyTop]), ((IterParseNode)yyVals[0+yyTop]));
    yyVal = ((FCallParseNode)yyVals[-2+yyTop]);
    return yyVal;
};
states[63] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.new_call(((ParseNode)yyVals[-3+yyTop]), ((TruffleString)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]), ((ParseNode)yyVals[0+yyTop]), null);
    return yyVal;
};
states[64] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.new_call(((ParseNode)yyVals[-4+yyTop]), ((TruffleString)yyVals[-3+yyTop]), ((TruffleString)yyVals[-2+yyTop]), ((ParseNode)yyVals[-1+yyTop]), ((IterParseNode)yyVals[0+yyTop])); 
    return yyVal;
};
states[65] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.new_call(((ParseNode)yyVals[-3+yyTop]), ((TruffleString)yyVals[-1+yyTop]), ((ParseNode)yyVals[0+yyTop]), null);
    return yyVal;
};
states[66] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.new_call(((ParseNode)yyVals[-4+yyTop]), ((TruffleString)yyVals[-2+yyTop]), ((ParseNode)yyVals[-1+yyTop]), ((IterParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[67] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.new_super(((SourceIndexLength)yyVals[-1+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[68] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.new_yield(((SourceIndexLength)yyVals[-1+yyTop]), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[69] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new ReturnParseNode(((SourceIndexLength)yyVals[-1+yyTop]), support.ret_args(((ParseNode)yyVals[0+yyTop]), ((SourceIndexLength)yyVals[-1+yyTop])));
    return yyVal;
};
states[70] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new BreakParseNode(((SourceIndexLength)yyVals[-1+yyTop]), support.ret_args(((ParseNode)yyVals[0+yyTop]), ((SourceIndexLength)yyVals[-1+yyTop])));
    return yyVal;
};
states[71] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new NextParseNode(((SourceIndexLength)yyVals[-1+yyTop]), support.ret_args(((ParseNode)yyVals[0+yyTop]), ((SourceIndexLength)yyVals[-1+yyTop])));
    return yyVal;
};
states[73] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((ParseNode)yyVals[-1+yyTop]);
    return yyVal;
};
states[74] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((MultipleAsgnParseNode)yyVals[0+yyTop]);
    return yyVal;
};
states[75] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((SourceIndexLength)yyVals[-2+yyTop]), support.newArrayNode(((SourceIndexLength)yyVals[-2+yyTop]), ((ParseNode)yyVals[-1+yyTop])), null, null);
    return yyVal;
};
states[76] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((ListParseNode)yyVals[0+yyTop]).getPosition(), ((ListParseNode)yyVals[0+yyTop]), null, null);
    return yyVal;
};
states[77] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((ListParseNode)yyVals[-1+yyTop]).getPosition(), ((ListParseNode)yyVals[-1+yyTop]).add(((ParseNode)yyVals[0+yyTop])), null, null);
    return yyVal;
};
states[78] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((ListParseNode)yyVals[-2+yyTop]).getPosition(), ((ListParseNode)yyVals[-2+yyTop]), ((ParseNode)yyVals[0+yyTop]), (ListParseNode) null);
    return yyVal;
};
states[79] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((ListParseNode)yyVals[-4+yyTop]).getPosition(), ((ListParseNode)yyVals[-4+yyTop]), ((ParseNode)yyVals[-2+yyTop]), ((ListParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[80] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((ListParseNode)yyVals[-1+yyTop]).getPosition(), ((ListParseNode)yyVals[-1+yyTop]), new StarParseNode(lexer.getPosition()), null);
    return yyVal;
};
states[81] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((ListParseNode)yyVals[-3+yyTop]).getPosition(), ((ListParseNode)yyVals[-3+yyTop]), new StarParseNode(lexer.getPosition()), ((ListParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[82] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((ParseNode)yyVals[0+yyTop]).getPosition(), null, ((ParseNode)yyVals[0+yyTop]), null);
    return yyVal;
};
states[83] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(((ParseNode)yyVals[-2+yyTop]).getPosition(), null, ((ParseNode)yyVals[-2+yyTop]), ((ListParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[84] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(lexer.getPosition(), null, new StarParseNode(lexer.getPosition()), null);
    return yyVal;
};
states[85] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new MultipleAsgnParseNode(lexer.getPosition(), null, new StarParseNode(lexer.getPosition()), ((ListParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[87] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((ParseNode)yyVals[-1+yyTop]);
    return yyVal;
};
states[88] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.newArrayNode(((ParseNode)yyVals[-1+yyTop]).getPosition(), ((ParseNode)yyVals[-1+yyTop]));
    return yyVal;
};
states[89] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((ListParseNode)yyVals[-2+yyTop]).add(((ParseNode)yyVals[-1+yyTop]));
    return yyVal;
};
states[90] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.newArrayNode(((ParseNode)yyVals[0+yyTop]).getPosition(), ((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[91] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = ((ListParseNode)yyVals[-2+yyTop]).add(((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[92] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.assignableLabelOrIdentifier(((TruffleString)yyVals[0+yyTop]), null);
    return yyVal;
};
states[93] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new InstAsgnParseNode(lexer.tokline, support.symbolID(((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[94] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new GlobalAsgnParseNode(lexer.tokline, support.symbolID(((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[95] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (support.isInDef()) support.compile_error("dynamic constant assignment");
    yyVal = new ConstDeclParseNode(lexer.tokline, support.symbolID(((TruffleString)yyVals[0+yyTop])), null, NilImplicitParseNode.NIL);
    return yyVal;
};
states[96] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new ClassVarAsgnParseNode(lexer.tokline, support.symbolID(((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[97] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to nil");
    yyVal = null;
    return yyVal;
};
states[98] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't change the value of self");
    yyVal = null;
    return yyVal;
};
states[99] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to true");
    yyVal = null;
    return yyVal;
};
states[100] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to false");
    yyVal = null;
    return yyVal;
};
states[101] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to __FILE__");
    yyVal = null;
    return yyVal;
};
states[102] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to __LINE__");
    yyVal = null;
    return yyVal;
};
states[103] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to __ENCODING__");
    yyVal = null;
    return yyVal;
};
states[104] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.aryset(((ParseNode)yyVals[-3+yyTop]), ((ParseNode)yyVals[-1+yyTop]));
    return yyVal;
};
states[105] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.attrset(((ParseNode)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]), ((TruffleString)yyVals[0+yyTop]));
    return yyVal;
};
states[106] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.attrset(((ParseNode)yyVals[-2+yyTop]), ((TruffleString)yyVals[0+yyTop]));
    return yyVal;
};
states[107] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.attrset(((ParseNode)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]), ((TruffleString)yyVals[0+yyTop]));
    return yyVal;
};
states[108] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (support.isInDef()) support.yyerror("dynamic constant assignment");

    SourceIndexLength position = support.getPosition(((ParseNode)yyVals[-2+yyTop]));

    yyVal = new ConstDeclParseNode(position, (TruffleString) null, support.new_colon2(position, ((ParseNode)yyVals[-2+yyTop]), ((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[109] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (support.isInDef()) {
        support.yyerror("dynamic constant assignment");
    }

    SourceIndexLength position = lexer.tokline;

    yyVal = new ConstDeclParseNode(position, (TruffleString) null, support.new_colon3(position, ((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[110] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.backrefAssignError(((ParseNode)yyVals[0+yyTop]));
    return yyVal;
};
states[111] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.assignableLabelOrIdentifier(((TruffleString)yyVals[0+yyTop]), null);
    return yyVal;
};
states[112] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new InstAsgnParseNode(lexer.tokline, support.symbolID(((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[113] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new GlobalAsgnParseNode(lexer.tokline, support.symbolID(((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[114] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (support.isInDef()) support.compile_error("dynamic constant assignment");

    yyVal = new ConstDeclParseNode(lexer.tokline, support.symbolID(((TruffleString)yyVals[0+yyTop])), null, NilImplicitParseNode.NIL);
    return yyVal;
};
states[115] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = new ClassVarAsgnParseNode(lexer.tokline, support.symbolID(((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[116] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to nil");
    yyVal = null;
    return yyVal;
};
states[117] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't change the value of self");
    yyVal = null;
    return yyVal;
};
states[118] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to true");
    yyVal = null;
    return yyVal;
};
states[119] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to false");
    yyVal = null;
    return yyVal;
};
states[120] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to __FILE__");
    yyVal = null;
    return yyVal;
};
states[121] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to __LINE__");
    yyVal = null;
    return yyVal;
};
states[122] = (support, lexer, yyVal, yyVals, yyTop) -> {
    support.compile_error("Can't assign to __ENCODING__");
    yyVal = null;
    return yyVal;
};
states[123] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.aryset(((ParseNode)yyVals[-3+yyTop]), ((ParseNode)yyVals[-1+yyTop]));
    return yyVal;
};
states[124] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.attrset(((ParseNode)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]), ((TruffleString)yyVals[0+yyTop]));
    return yyVal;
};
states[125] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.attrset(((ParseNode)yyVals[-2+yyTop]), ((TruffleString)yyVals[0+yyTop]));
    return yyVal;
};
states[126] = (support, lexer, yyVal, yyVals, yyTop) -> {
    yyVal = support.attrset(((ParseNode)yyVals[-2+yyTop]), ((TruffleString)yyVals[-1+yyTop]), ((TruffleString)yyVals[0+yyTop]));
    return yyVal;
};
states[127] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (support.isInDef()) {
        support.yyerror("dynamic constant assignment");
    }

    SourceIndexLength position = support.getPosition(((ParseNode)yyVals[-2+yyTop]));

    yyVal = new ConstDeclParseNode(position, (TruffleString) null, support.new_colon2(position, ((ParseNode)yyVals[-2+yyTop]), ((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
states[128] = (support, lexer, yyVal, yyVals, yyTop) -> {
    if (support.isInDef()) {
        support.yyerror("dynamic constant assignment");
    }

    SourceIndexLength position = lexer.tokline;

    yyVal = new ConstDeclParseNode(position, (TruffleString) null, support.new_colon3(position, ((TruffleString)yyVals[0+yyTop])), NilImplicitParseNode.NIL);
    return yyVal;
};
state