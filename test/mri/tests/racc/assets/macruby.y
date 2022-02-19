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
      