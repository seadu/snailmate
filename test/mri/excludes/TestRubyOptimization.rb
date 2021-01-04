
exclude :test_eqq, "needs investigation"
exclude :test_fixnum_gt, "needs investigation"
exclude :test_hash_aref_with, "needs investigation"
exclude :test_nil_safe_conditional_assign, "needs investigation"
exclude :test_optimized_rescue, "needs investigation"
exclude :test_string_freeze, "TruffleRuby does not consider redefined String#freeze on string literals for simplicity"
exclude :test_string_uminus, "TruffleRuby does not consider redefined String#-@ on string literals for simplicity"
exclude :test_string_freeze_block, "needs investigation"
exclude :test_string_freeze_saves_memory, "needs investigation"
exclude :test_string_length, "needs investigation"
exclude :test_string_ltlt, "needs investigation"
exclude :test_block_parameter_should_not_create_objects, "needs investigation"
exclude :test_callinfo_unreachable_path, "needs investigation"
exclude :test_clear_unreachable_keyword_args, "needs investigation"
exclude :test_fixnum_lt, "needs investigation"
exclude :test_fixnum_minus, "needs investigation"
exclude :test_fixnum_plus, "needs investigation"
exclude :test_opt_case_dispatch, "needs investigation"
exclude :test_peephole_dstr, "needs investigation"
exclude :test_peephole_optimization_without_trace, "needs investigation"
exclude :test_peephole_string_literal_range, "needs investigation"
exclude :test_string_plus, "needs investigation"
exclude :test_tailcall, "needs investigation"
exclude :test_tailcall_condition_block, "needs investigation"
exclude :test_tailcall_inhibited_by_block, "needs investigation"
exclude :test_tailcall_inhibited_by_rescue, "needs investigation"
exclude :test_tailcall_not_to_grow_stack, "needs investigation"
exclude :test_tailcall_symbol_block_arg, "needs investigation"
exclude :test_tailcall_with_block, "needs investigation"
exclude :test_trace_optimized_methods, "ArgumentError: unknown event: c_call"
exclude :test_objtostring, "assert_separately failed with error message"