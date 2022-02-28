# Copyright (c) 2013 Peter Zotov  <whitequark@whitequark.org>
#
# Parts of the source are derived from ruby_parser:
# Copyright (c) Ryan Davis, seattle.rb
#
# MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

class Parser::MacRuby

token kCLASS kMODULE kDEF kUNDEF kBEGIN kRESCUE kENSURE kEND kIF kUNLESS
      kTHEN kELSIF kELSE kCASE kWHEN kWHILE kUNTIL kFOR kBREAK kNEXT
      kREDO kRETRY kIN kDO kDO_COND kDO_BLOCK kDO_LAMBDA kRETURN kYIELD kSUPER
      kSELF kNIL kTRUE kFALSE kAND kOR kNOT kIF_MOD kUNLESS_MOD kWHILE_MOD
      kUNTIL_MOD kRESCUE_MOD kALIAS kDEFINED klBEGIN klEND k__LINE__
      k__FILE__ k__ENCODING__ tIDENTIFIER tFID tGVAR tIVAR tCONSTANT
      tLABEL tCVAR tNTH_REF tBACK_REF tSTRING_CONTENT tINTEGER tFLOAT
      tREGEXP_END tUPLUS tUMINUS tUMINUS_NUM tPOW tCMP tEQ tEQQ tNEQ
      tGEQ tLEQ tANDOP tOROP tMATCH tNMATCH tDOT tDOT2 tDOT3 tAREF
      tASET tLSHFT tRSHFT tCOLON2 tCOLON3 tOP_ASGN tASSOC tLPAREN
      tLPAREN2 tRPAREN tLPAREN_ARG tLBRACK tLBRACK2 tRBRACK tLBRACE
      tLBRACE_ARG tSTAR tSTAR2 tAMPER tAMPER2 tTILDE tPERCENT tDIVIDE
      tPLUS tMINUS tLT tGT tPIPE tBANG tCARET tLCURLY tRCURLY
      tBACK_REF2 tSYMBEG tSTRING_BEG tXSTRING_BEG tREGEXP_BEG tREGEXP_OPT
      tWORDS_BEG tQWORDS_BEG tSTRING_DBEG tSTRING_DVAR tSTRING_END
      tSTRING tSYMBOL tNL tEH tCOLON tCOMMA tSPACE tSEMI tLAMBDA tLAMBEG
      tCHARACTER

prechigh
  right    tBANG tTILDE tUPLUS
  right    tPOW
  right    tUMINUS_NUM tUMINUS
  left     tSTAR2 tDIVIDE tPERCENT
  left     tPLUS tMINUS
  left     tLSHFT tRSHFT
  left     tAMPER2
  left     tPIPE tCARET
  left     tGT tGEQ tLT tLEQ
  nonassoc tCMP tEQ tEQQ tNEQ tMATCH tNMATCH
  left     tANDOP
  left     tOROP
  nonassoc tDOT2 tDOT3
  right    tEH tCOLON
  left     kRESCUE_MOD
  right    tEQL tOP_ASGN
  nonassoc kDEFINED
  right    kNOT
  left     kOR kAND
  nonassoc kIF_MOD kUNLESS_MOD kWHILE_MOD kUNTIL_MOD
  nonassoc tLBRACE_ARG
  nonassoc tLOWEST
preclow

rule

         program: top_compstmt

    top_compstmt: top_stmts opt_terms
                    {
                      result = @builder.compstmt(val[0])
                    }

       top_stmts: # nothing
                    {
                      result = []
                    }
                | top_stmt
                    {
                      result = [ val[0] ]
                    }
                | top_stmts terms top_stmt
                    {
                      result = val[0] << val[2]
                    }
                | error top_stmt
                    {
                      result = [ val[1] ]
                    }

        top_stmt: stmt
                | klBEGIN tLCURLY top_compstmt tRCURLY
                    {
                      result = @builder.preexe(val[0], val[1], val[2], val[3])
                    }

        bodystmt: compstmt opt_rescue opt_else opt_ensure
                    {
                      rescue_bodies     = val[1]
                      else_t,   else_   = val[2]
                      ensure_t, ensure_ = val[3]

                      if rescue_bodies.empty? && !else_.nil?
                        diagnostic :warning, :useless_else, nil, else_t
                      end

                      result = @builder.begin_body(val[0],
                                  rescue_bodies,
                                  else_t,   else_,
                                  ensure_t, ensure_)
                    }

        compstmt: stmts opt_terms
                    {
                      result = @builder.compstmt(val[0])
                    }

           stmts: # nothing
                    {
                      result = []
                    }
                | stmt
                    {
                      result = [ val[0] ]
                    }
                | stmts terms stmt
                    {
                      result = val[0] << val[2]
                    }
                | error stmt
                    {
                      result = [ val[1] ]
                    }

            stmt: kALIAS fitem
                    {
                      @lexer.state = :expr_fname
                    }
                    fitem
                    {
                      result = @builder.alias(val[0], val[1], val[3])
                    }
                | kALIAS tGVAR tGVAR
                    {
                      result = @builder.alias(val[0],
                                  @builder.gvar(val[1]),
                                  @builder.gvar(val[2]))
                    }
                | kALIAS tGVAR tBACK_REF
                    {
                      result = @builder.alias(val[0],
                                  @builder.gvar(val[1]),
                                  @builder.back_ref(val[2]))
                    }
                | kALIAS tGVAR tNTH_REF
                    {
                      diagnostic :error, :nth_ref_alias, nil, val[2]
                    }
                | kUNDEF undef_list
                    {
                      result = @builder.undef_method(val[0], val[1])
                    }
                | stmt kIF_MOD expr_value
                    {
                      result = @builder.condition_mod(val[0], nil,
                                                      val[1], val[2])
                    }
                | stmt kUNLESS_MOD expr_value
                    {
                      result = @builder.condition_mod(nil, val[0],
                                                      val[1], val[2])
                    }
                | stmt kWHILE_MOD expr_value
                    {
                      result = @builder.loop_mod(:while, val[0], val[1], val[2])
                    }
                | stmt kUNTIL_MOD expr_value
                    {
                      result = @builder.loop_mod(:until, val[0], val[1], val[2])
                    }
                | stmt kRESCUE_MOD stmt
                    {
                      rescue_body = @builder.rescue_body(val[1],
                                        nil, nil, nil,
                                        nil, val[2])

                      result = @builder.begin_body(val[0], [ rescue_body ])
                    }
                | klEND tLCURLY compstmt tRCURLY
                    {
                      result = @builder.postexe(val[0], val[1], val[2], val[3])
                    }
                | lhs tEQL command_call
                    {
                      result = @builder.assign(val[0], val[1], val[2])
                    }
                | mlhs tEQL command_call
                    {
                      result = @builder.multi_assign(val[0], val[1], val[2])
                    }
                | var_lhs tOP_ASGN command_call
                    {
                      result = @builder.op_assign(val[0], val[1], val[2])
                    }
                | primary_value tLBRACK2 opt_call_args rbracket tOP_ASGN command_call
                    {
                      result = @builder.op_assign(
                                  @builder.index(
                                    val[0], val[1], val[2], val[3]),
                                  val[4], val[5])
                    }
                | primary_value tDOT tIDENTIFIER tOP_ASGN command_call
                    {
                      result = @builder.op_assign(
                                  @builder.call_method(
                                    val[0], val[1], val[2]),
                                  val[3], val[4])
                    }
                | primary_value tDOT tCONSTANT tOP_ASGN command_call
                    {
                      result = @builder.op_assign(
                                  @builder.call_method(
                                    val[0], val[1], val[2]),
                                  val[3], val[4])
                    }
                | primary_value tCOLON2 tCONSTANT tOP_ASGN command_call
                    {
                      result = @builder.op_assign(
                                  @builder.call_method(
                                    val[0], val[1], val[2]),
                                  val[3], val[4])
                    }
                | primary_value tCOLON2 tIDENTIFIER tOP_ASGN command_call
                    {
                      result = @builder.op_assign(
                                  @builder.call_method(
                                    val[0], val[1], val[2]),
                                  val[3], val[4])
                    }
                | backref tOP_ASGN command_call
                    {
                      @builder.op_assign(val[0], val[1], val[2])
                    }
                | lhs tEQL mrhs
                    {
                      result = @builder.assign(val[0], val[1],
                                  @builder.array(nil, val[2], nil))
                    }
                | mlhs tEQL arg_value
                    {
                      result = @builder.multi_assign(val[0], val[1], val[2])
                    }
                | mlhs tEQL mrhs
                    {
                      result = @builder.multi_assign(val[0], val[1],
                                  @builder.array(nil, val[2], nil))
                    }
                | expr

            expr: command_call
                | expr kAND expr
                    {
                      result = @builder.logical_op(:and, val[0], val[1], val[2])
                    }
                | expr kOR expr
                    {
                      result = @builder.logical_op(:or, val[0], val[1], val[2])
                    }
                | kNOT opt_nl expr
                    {
                      result = @builder.not_op(val[0], nil, val[2], nil)
                    }
                | tBANG command_call
                    {
                      result = @builder.not_op(val[0], nil, val[1], nil)
                    }
                | arg

      expr_value: expr

    command_call: command
                | block_command

   block_command: block_call
                | block_call tDOT operation2 command_args
                    {
                      result = @builder.call_method(val[0], val[1], val[2],
                                  *val[3])
                    }
                | block_call tCOLON2 operation2 command_args
                    {
                      result = @builder.call_method(val[0], val[1], val[2],
                                  *val[3])
                    }

 cmd_brace_block: tLBRACE_ARG
                    {
                      @static_env.extend_dynamic
                    }
                    opt_block_param compstmt tRCURLY
                    {
                      result = [ val[0], val[2], val[3], val[4] ]

                      @static_env.unextend
                    }

         command: operation command_args =tLOWEST
                    {
                      result = @builder.call_method(nil, nil, val[0],
                                  *val[1])
                    }
                | operation command_args cmd_brace_block
                    {
                      method_call = @builder.call_method(nil, nil, val[0],
                                        *val[1])

                      begin_t, args, body, end_t = val[2]
                      result      = @builder.block(method_call,
                                      begin_t, args, body, end_t)
                    }
                | primary_value tDOT operation2 command_args =tLOWEST
                    {
                      result = @builder.call_method(val[0], val[1], val[2],
                                  *val[3])
                    }
                | primary_value tDOT operation2 command_args cmd_brace_block
                    {
                      method_call = @builder.call_method(val[0], val[1], val[2],
                                        *val[3])

                      begin_t, args, body, end_t = val[4]
                      result      = @builder.block(method_call,
                                      begin_t, args, body, end_t)
                    }
                | primary_value tCOLON2 operation2 command_args =tLOWEST
                    {
                      result = @builder.call_method(val[0], val[1], val[2],
                                  *val[3])
                    }
                | primary_value tCOLON2 operation2 command_args cmd_brace_block
                    {
                      method_call = @builder.call_method(val[0], val[1], val[2],
                                        *val[3])

                      begin_t, args, body, end_t = val[4]
                      result      = @builder.block(method_call,
                                      begin_t, args, body, end_t)
                    }
                | kSUPER command_args
                    {
                      result = @builder.keyword_cmd(:super, val[0],
                                  *val[1])
                    }
                | kYIELD command_args
                    {
                      result = @builder.keyword_cmd(:yield, val[0],
                                  *val[1])
                    }
                | kRETURN call_args
                    {
                      result = @builder.keyword_cmd(:return, val[0],
                                  nil, val[1], nil)
                    }
                | kBREAK call_args
                    {
                      result = @builder.keyword_cmd(:break, val[0],
                                  nil, val[1], nil)
                    }
                | kNEXT call_args
                    {
                      result = @builder.keyword_cmd(:next, val[0],
                                  nil, val[1], nil)
                    }

            mlhs: mlhs_basic
                    {
                      result = @builder.multi_lhs(nil, val[0], nil)
                    }
                | tLPAREN mlhs_inner rparen
                    {
                      result = @builder.begin(val[0], val[1], val[2])
                    }

      mlhs_inner: mlhs_basic
                    {
                      result = @builder.multi_lhs(nil, val[0], nil)
                    }
                | tLPAREN mlhs_inner rparen
                    {
                      result = @builder.multi_lhs(val[0], val[1], val[2])
                    }

      mlhs_basic: mlhs_head
                | mlhs_head mlhs_item
                    {
                      result = val[0].
                                  push(val[1])
                    }
                | mlhs_head tSTAR mlhs_node
                    {
                      result = val[0].
                                  push(@builder.splat(val[1], val[2]))
                    }
                | mlhs_head tSTAR mlhs_node tCOMMA mlhs_post
                    {
                      result = val[0].
                                  push(@builder.splat(val[1], val[2])).
                                  concat(val[4])
                    }
                | mlhs_head tSTAR
                    {
                      result = val[0].
                                  push(@builder.splat(val[1]))
                    }
                | mlhs_head tSTAR tCOMMA mlhs_post
                    {
                      result = val[0].
                                  push(@builder.splat(val[1])).
                                  concat(val[3])
                    }
                | tSTAR mlhs_node
                    {
                      result = [ @builder.splat(val[0], val[1]) ]
                    }
                | tSTAR mlhs_node tCOMMA mlhs_post
                    {
                      result = [ @builder.splat(val[0], val[1]),
                                 *val[3] ]
                    }
                | tSTAR
                    {
                      result = [ @builder.splat(val[0]) ]
                    }
                | tSTAR tCOMMA mlhs_post
                    {
                      result = [ @builder.splat(val[0]),
                                 *val[2] ]
                    }

       mlhs_item: mlhs_node
                | tLPAREN mlhs_inner rparen
                    {
                      result = @builder.begin(val[0], val[1], val[2])
                    }

       mlhs_head: mlhs_item tCOMMA
                    {
                      result = [ val[0] ]
                    }
                | mlhs_head mlhs_item tCOMMA
                    {
                      result = val[0] << val[1]
                    }

       mlhs_post: mlhs_item
                    {
                      result = [ val[0] ]
                    }
                | mlhs_post tCOMMA mlhs_item
                    {
                      result = val[0] << val[2]
                    }

       mlhs_node: variable
                    {
                      result = @builder.assignable(val[0])
                    }
                | primary_value tLBRACK2 opt_call_args rbracket
                    {
                      result = @builder.index_asgn(val[0], val[1], val[2], val[3])
                    }
                | primary_value tDOT tIDENTIFIER
                    {
                      result = @builder.attr_asgn(val[0], val[1], val[2])
                    }
                | primary_value tCOLON2 tIDENTIFIER
                    {
                      result = @builder.attr_asgn(val[0], val[1], val[2])
                    }
                | primary_value tDOT tCONSTANT
                    {
                      result = @builder.attr_asgn(val[0], val[1], val[2])
                    }
                | primary_value tCOLON2 tCONSTANT
                    {
                      result = @builder.assignable(
                                  @builder.const_fetch(val[0], val[1], val[2]))
                    }
                | tCOLON3 tCONSTANT
                    {
                      result = @builder.assignable(
                                  @builder.const_global(val[0], val[1]))
                    }
                | backref
                    {
                      result = @builder.assignable(val[0])
                    }

             lhs: variable
                    {
                      result = @builder.assignable(val[0])
                    }
                | primary_value tLBRACK2 opt_call_args rbracket
                    {
                      result = @builder.index_asgn(val[0], val[1], val[2], val[3])
                    }
                | primary_value tDOT tIDENTIFIER
                    {
                      result = @builder.attr_asgn(val[0], val[1], val[2])
                    }
                | primary_value tCOLON2 tIDENTIFIER
                    {
                      result = @builder.attr_asgn(val[0], val[1], val[2])
                    }
                | primary_value tDOT tCONSTANT
                    {
                      result = @builder.attr_asgn(val[0], val[1], val[2])
                    }
                | primary_value tCOLON2 tCONSTANT
                    {
                      result = @builder.assignable(
                                  @builder.const_fetch(val[0], val[1], val[2]))
                    }
                | tCOLON3 tCONSTANT
                    {
                      result = @builder.assignable(
                                  @builder.const_global(val[0], val[1]))
                    }
                | backref
                    {
                      result = @builder.assignable(val[0])
                    }

           cname: tIDENTIFIER
                    {
                      diagnostic :error, :module_name_const, nil, val[0]
                    }
                | tCONSTANT

           cpath: tCOLON3 cname
                    {
                      result = @builder.const_global(val[0], val[1])
                    }
                | cname
                    {
                      result = @builder.const(val[0])
                    }
                | primary_value tCOLON2 cname
                    {
                      result = @builder.const_fetch(val[0], val[1], val[2])
                    }

           fname: tIDENTIFIER | tCONSTANT | tFID
                | op
                | reswords

            fsym: fname
                    {
                      result = @builder.symbol(val[0])
                    }
                | symbol

           fitem: fsym
                | dsym

      undef_list: fitem
                    {
                      result = [ val[0] ]
                    }
                | undef_list tCOMMA
                    {
                      @lexer.state = :expr_fname
                    }
                    fitem
                    {
                      result = val[0] << val[3]
                    }

              op:   tPIPE    | tCARET  | tAMPER2  | tCMP  | tEQ     | tEQQ
                |   tMATCH   | tNMATCH | tGT      | tGEQ  | tLT     | tLEQ
                |   tNEQ     | tLSHFT  | tRSHFT   | tPLUS | tMINUS  | tSTAR2
                |   tSTAR    | tDIVIDE | tPERCENT | tPOW  | tBANG   | tTILDE
                |   tUPLUS   | tUMINUS | tAREF    | tASET | tBACK_REF2

        reswords: k__LINE__ | k__FILE__ | k__ENCODING__ | klBEGIN | klEND
                | kALIAS    | kAND      | kBEGIN        | kBREAK  | kCASE
                | kCLASS    | kDEF      | kDEFINED      | kDO     | kELSE
                | kELSIF    | kEND      | kENSURE       | kFALSE  | kFOR
                | kIN       | kMODULE   | kNEXT         | kNIL    | kNOT
                | kOR       | kREDO     | kRESCUE       | kRETRY  | kRETURN
                | kSELF     | kSUPER    | kTHEN         | kTRUE   | kUNDEF
                | kWHEN     | kYIELD    | kIF           | kUNLESS | kWHILE
                | kUNTIL

             arg: lhs tEQL arg
                    {
                      result = @builder.assign(val[0], val[1], val[2])
                    }
                | lhs tEQL arg kRESCUE_MOD arg
                    {
                      rescue_body = @builder.rescue_body(val[3],
                                        nil, nil, nil,
                                        nil, val[4])

                      rescue_ = @builder.begin_body(val[2], [ rescue_body ])

                      result  = @builder.assign(val[0], val[1], rescue_)
                    }
                | var_lhs tOP_ASGN arg
                    {
                      result = @builder.op_assign(val[0], val[1], val[2])
                    }
                | var_lhs tOP_ASGN arg kRESCUE_MOD arg
                    {
                      rescue_body = @builder.rescue_body(val[3],
                                        nil, nil, nil,
                                        nil, val[4])

                      rescue_ = @builder.begin_body(val[2], [ rescue_body ])

                      result = @builder.op_assign(val[0], val[1], rescue_)
                    }
                | primary_value tLBRACK2 opt_call_args rbracket tOP_ASGN arg
                    {
                      result = @builder.op_assign(
                                  @builder.index(
                                    val[0], val[1], val[2], val[3]),
                                  val[4], val[5])
                    }
                | primary_value tDOT tIDENTIFIER tOP_ASGN arg
                    {
                      result = @builder.op_assign(
                                  @builder.call_method(
                                    val[0], val[1], val[2]),
                                  val[3], val[4])
                    }
                | primary_value tDOT tCONSTANT tOP_ASGN arg
                    {
                      result = @builder.op_assign(
                                  @builder.call_method(
                                    val[0], val[1], val[2]),
                                  val[3], val[4])
                    }
                | primary_value tCOLON2 tIDENTIFIER tOP_ASGN arg
                    {
                      result = @builder.op_assign(
                                  @builder.call_method(
                                    val[0], val[1], val[2]),
                                  val[3], val[4])
                    }
                | primary_value tCOLON2 tCONSTANT tOP_ASGN arg
                    {
                      diagnostic :error, :dynamic_const, nil, val[2], [ val[3] ]
                    }
                | tCOLON3 tCONSTANT tOP_ASGN arg
                    {
                      diagnostic :error, :dynamic_const, nil, val[1], [ val[2] ]
                    }
                | backref tOP_ASGN arg
                    {
                      result = @builder.op_assign(val[0], val[1], val[2])
                    }
                | arg tDOT2 arg
                    {
                      result = @builder.range_inclusive(val[0], val[1], val[2])
                    }
                | arg tDOT3 arg
                    {
                      result = @builder.range_exclusive(val[0], val[1], val[2])
                    }
                | arg tPLUS arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tMINUS arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tSTAR2 arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tDIVIDE arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tPERCENT arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tPOW arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | tUMINUS_NUM tINTEGER tPOW arg
                    {
                      result = @builder.unary_op(val[0],
                                  @builder.binary_op(
                                    @builder.integer(val[1]),
                                      val[2], val[3]))
                    }
                | tUMINUS_NUM tFLOAT tPOW arg
                    {
                      result = @builder.unary_op(val[0],
                                  @builder.binary_op(
                                    @builder.float(val[1]),
                                      val[2], val[3]))
                    }
                | tUPLUS arg
                    {
                      result = @builder.unary_op(val[0], val[1])
                    }
                | tUMINUS arg
                    {
                      result = @builder.unary_op(val[0], val[1])
                    }
                | arg tPIPE arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tCARET arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tAMPER2 arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tCMP arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tGT arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tGEQ arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tLT arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tLEQ arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tEQ arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tEQQ arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tNEQ arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tMATCH arg
                    {
                      result = @builder.match_op(val[0], val[1], val[2])
                    }
                | arg tNMATCH arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | tBANG arg
                    {
                      result = @builder.not_op(val[0], nil, val[1], nil)
                    }
                | tTILDE arg
                    {
                      result = @builder.unary_op(val[0], val[1])
                    }
                | arg tLSHFT arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tRSHFT arg
                    {
                      result = @builder.binary_op(val[0], val[1], val[2])
                    }
                | arg tANDOP arg
                    {
                      result = @builder.logical_op(:and, val[0], val[1], val[2])
                    }
                | arg tOROP arg
                    {
                      result = @builder.logical_op(:or, val[0], val[1], val[2])
                    }
                | kDEFINED opt_nl arg
                    {
                      result = @builder.keyword_cmd(:defined?, val[0], nil, [ val[2] ], nil)
                    }

                | arg tEH arg opt_nl tCOLON arg
                    {
                      result = @builder.ternary(val[0], val[1],
                                                val[2], val[4], val[5])
                    }
                | primary

       arg_value: arg

       aref_args: none
                | args trailer
                | args tCOMMA assocs trailer
                    {
                      result = val[0] << @builder.associate(nil, val[2], nil)
                    }
                | assocs trailer
                    {
                      result = [ @builder.associate(nil, val[0], nil) ]
                    }

      paren_args: tLPAREN2 opt_call_args rparen
                    {
                      result = val
                    }

  opt_paren_args: # nothing
                    {
                      result = [ nil, [], nil ]
                    }
                | paren_args

   opt_call_args: # nothing
                    {
                      result = []
                    }
                | call_args

       call_args: command
                    {
                      result = [ val[0] ]
                    }
                | args opt_block_arg
                    {
                      result = val[0].concat(val[1])
                    }
                | assocs opt_block_arg
                    {
                      result = [ @builder.associate(nil, val[0], nil) ]
                      result.concat(val[1])
                    }
                | args tCOMMA assocs opt_block_arg
                    {
                      assocs = @builder.associate(nil, val[2], nil)
                      result = val[0] << assocs
                      result.concat(val[3])
                    }
                | args tCOMMA assocs tCOMMA args opt_block_arg
                    {
                      val[2][-1] = @builder.objc_varargs(val[2][-1], val[4])
                      assocs = @builder.associate(nil, val[2], nil)
                      result = val[0] << assocs
                      result.concat(val[5])
                    }
                | block_arg
                    {
                      result =  [ val[0] ]
                    }

      call_args2: arg_value tCOMMA args opt_block_arg
                    {
                      result = [ val[0], *val[2].concat(val[3]) ]
                    }
                | arg_value tCOMMA block_arg
                    {
                      result = [ val[0], val[2] ]
                    }
                | assocs opt_block_arg
                    {
                      result =  [ @builder.associate(nil, val[0], nil),
                                  *val[1] ]
                    }
                | arg_value tCOMMA assocs opt_block_arg
                    {
                      result =  [ val[0],
                                  @builder.associate(nil, val[2], nil),
                                  *val[3] ]
                    }
                | arg_value tCOMMA args tCOMMA assocs opt_block_arg
                    {
                      result =  [ val[0],
                                  *val[2].
                                    push(@builder.associate(nil, val[4], nil)).
                                    concat(val[5]) ]
                    }
                | block_arg
                    {
                      result = [ val[0] ]
                    }

    command_args:   {
                      result = @lexer.cmdarg.dup
                      @lexer.cmdarg.push(true)
                    }
                    open_args
                    {
                      @lexer.cmdarg = val[0]

                      result = val[1]
                    }

       open_args: call_args
                    {
                      result = [ nil, val[0], nil ]
                    }
                | tLPAREN_ARG
                    {
                      @lexer.state = :expr_endarg
                    }
                    rparen
                    {
                      result = [ val[0], [], val[2] ]
                    }
                | tLPAREN_ARG call_args2
                    {
                      @lexer.state = :expr_endarg
                    }
                    rparen
                    {
                      result = [ val[0], val[1], val[3] ]
                    }

       block_arg: tAMPER arg_value
                    {
                      result = @builder.block_pass(val[0], val[1])
                    }

   opt_block_arg: tCOMMA block_arg
                    {
                      result = [ val[1] ]
                    }
                | tCOMMA
                    {
                      result = []
                    }
                | # nothing
                    {
                      result = []
                    }

            args: arg_value
                    {
                      result = [ val[0] ]
                    }
                | tSTAR arg_value
                    {
                      result = [ @builder.splat(val[0], val[1]) ]
                    }
                | args tCOMMA arg_value
                    {
                      result = val[0] << val[2]
                    }
                | args tCOMMA tSTAR arg_value
                    {
                      result = val[0] << @builder.splat(val[2], val[3])
                    }

            mrhs: args tCOMMA arg_value
                    {
                      result = val[0] << val[2]
                    }
                | args tCOMMA tSTAR arg_value
                    {
                      result = val[0] << @builder.splat(val[2], val[3])
                    }
                | tSTAR arg_value
                    {
                      result = [ @builder.splat(val[0], val[1]) ]
                    }

         primary: literal
                | strings
                | xstring
                | regexp
                | words
                | qwords
                | var_ref
                | backref
                | tFID
                    {
                      result = @builder.call_method(nil, nil, val[0])
                    }
                | kBEGIN bodystmt kEND
                    {
                      result = @builder.begin_keyword(val[0], val[1], val[2])
                    }
                | tLPAREN_ARG expr
                    {
                      @lexer.state = :expr_endarg
                    }
                    rparen
                    {
                      result = @builder.begin(val[0], val[1], val[3])
                    }
                | tLPAREN compstmt tRPAREN
                    {
                      result = @builder.begin(val[0], val[1], val[2])
                    }
                | primary_value tCOLON2 tCONSTANT
                    {
                      result = @builder.const_fetch(val[0], val[1], val[2])
                    }
                | tCOLON3 tCONSTANT
                    {
                      result = @builder.const_global(val[0], val[1])
                    }
                | tLBRACK aref_args tRBRACK
                    {
                      result = @builder.array(val[0], val[1], val[2])
                    }
                | tLBRACE assoc_list tRCURLY
                    {
                      result = @builder.associate(val[0], val[1], val[2])
                    }
                | kRETURN
                    {
                      result = @builder.keyword_cmd(:return, val[0])
                    }
                | kYIELD tLPAREN2 call_args rparen
                    {
                      result = @builder.keyword_cmd(:yield, val[0], val[1], val[2], val[3])
                    }
                | kYIELD tLPAREN2 rparen
                    {
                      result = @builder.keyword_cmd(:yield, val[0], val[1], [], val[2])
                    }
                | kYIELD
                    {
                      result = @builder.keyword_cmd(:yield, val[0])
                    }
                | kDEFINED opt_nl tLPAREN2 expr rparen
                    {
                      result = @builder.keyword_cmd(:defined?, val[0],
                                                    val[2], [ val[3] ], val[4])
                    }
                | kNOT tLPAREN2 expr rparen
                    {
                      result = @builder.not_op(val[0], val[1], val[2], val[3])
                    }
                | kNOT tLPAREN2 rparen
                    {
                      result = @builder.not_op(val[0], val[1], nil, val[2])
                    }
                | operation brace_block
                    {
                      method_call = @builder.call_method(nil, nil, val[0])

                      begin_t, args, body, end_t = val[1]
                      result      = @builder.block(method_call,
                                      begin_t, args, body, end_t)
                    }
                | method_call
                | method_call brace_block
                    {
                      begin_t, args, body, end_t = val[1]
                      result      = @builder.block(val[0],
                                      begin_t, args, body, end_t)
                    }
                | tLAMBDA lambda
                    {
                      lambda_call = @builder.call_lambda(val[0])

                      args, (begin_t, body, end_t) = val[1]
                      result      = @builder.block(lambda_call,
                                      begin_t, args, body, end_t)
                    }
                | kIF expr_value then compstmt if_tail kEND
                    {
                      else_t, else_ = val[4]
                      result = @builder.condition(val[0], val[1], val[2],
                                                  val[3], else_t,
                                                  else_,  val[5])
                    }
                | kUNLESS expr_value then compstmt opt_else kEND
                    {
                      else_t, else_ = val[4]
                      result = @builder.condition(val[0], val[1], val[2],
                                                  else_,  else_t,
                                                  val[3], val[5])
                    }
                | kWHILE
                    {
                      @lexer.cond.push(true)
                    }
                    expr_value do
                    {
                      @lexer.cond.pop
                    }
                    compstmt kEND
                    {
                      result = @builder.loop(:while, val[0], val[2], val[3],
                                             val[5], val[6])
                    }
                | kUNTIL
                    {
                      @lexer.cond.push(true)
                    }
                    expr_value do
                    {
                      @lexer.cond.pop
                    }
                    compstmt kEND
                    {
                      result = @builder.loop(:until, val[0], val[2], val[3],
                                             val[5], val[6])
                    }
                | kCASE expr_value opt_terms case_body kEND
                    {
                      *when_bodies, (else_t, else_body) = *val[3]

                      result = @builder.case(val[0], val[1],
                                             when_bodies, else_t, else_body,
                                             val[4])
                    }
                | kCASE            opt_terms case_body kEND
                    {
                      *when_bodies, (else_t, else_body) = *val[2]

                      result = @builder.case(val[0], nil,
                                             when_bodies, else_t, else_body,
                                             val[3])
                    }
                | kFOR for_var kIN
                    {
                      @lexer.cond.push(true)
                    }
                    expr_value do
                    {
                      @lexer.cond.pop
                    }
                    compstmt kEND
                    {
                      result = @builder.for(val[0], val[1],
                                            val[2], val[4],
                                            val[5], val[7], val[8])
                    }
                | kCLASS cpath superclass
                    {
                      @static_env.extend_static
                      @lexer.push_cmdarg
                    }
                    bodystmt kEND
                    {
                      if in_def?
                        diagnostic :error, :class_in_def, nil, val[0]
                      end

                      lt_t, superclass = val[2]
                      result = @builder.def_class(val[0], val[1],
                                                  lt_t, superclass,
                                                  val[4], val[5])

                      @lexer.pop_cmdarg
                      @static_env.unextend
                    }
                | kCLASS tLSHFT expr term
                    {
                      result = @def_level
                      @def_level = 0

                      @static_env.extend_static
                      @lexer.push_cmdarg
                    }
                    bodystmt kEND
                    {
                      result = @builder.def_sclass(val[0], val[1], val[2],
                                                   val[5], val[6])

                      @lexer.pop_cmdarg
                      @static_env.unextend

                      @def_level = val[4]
                    }
                | kMODULE cpath
                    {
                      @static_env.extend_static
                      @lexer.push_cmdarg
                    }
                    bodystmt kEND
                    {
                      if in_def?
                        diagnostic :error, :module_in_def, nil, val[0]
                      end

                      result = @builder.def_module(val[0], val[1],
                                                   val[3], val[4])

                      @lexer.pop_cmdarg
                      @static_env.unextend
                    }
                | kDEF fname
                    {
                      @def_level += 1
                      @static_env.extend_static
                      @lexer.push_cmdarg
                    }
                    f_arglist bodystmt kEND
                    {
                      result = @builder.def_method(val[0], val[1],
                                  val[3], val[4], val[5])

                      @lexer.pop_cmdarg
                      @static_env.unextend
                      @def_level -= 1
                    }
                | kDEF singleton dot_or_colon
                    {
                      @lexer.state = :expr_fname
                    }
                    fname
                    {
                      @def_level += 1
                      @static_env.extend_static
                      @lexer.push_cmdarg
                    }
                    f_arglist bodystmt kEND
                    {
                      result = @builder.def_singleton(val[0], val[1], val[2],
                                  val[4], val[6], val[7], val[8])

                      @lexer.pop_cmdarg
                      @static_env.unextend
                      @def_level -= 1
                    }
                | kBREAK
                    {
                      result = @builder.keyword_cmd(:break, val[0])
                    }
                | kNEXT
                    {
                      result = @builder.keyword_cmd(:next, val[0])
                    }
                | kREDO
                    {
                      result = @builder.keyword_cmd(:redo, val[0])
                    }
                | kRETRY
                    {
                      result = @builder.keyword_cmd(:retry, val[0])
                    }

   primary_value: primary

            then: term
                | kTHEN
                | term kTHEN
                    {
                      result = val[1]
                    }

              do: term
                | kDO_COND

         if_tail: opt_else
                | kELSIF expr_value then compstmt if_tail
                    {
                      else_t, else_ = val[4]
                      result = [ val[0],
                                 @builder.condition(val[0], val[1], val[2],
              