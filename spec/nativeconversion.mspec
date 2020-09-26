class NativeHandleChecker
  EXPECTED_FAILURES = {
    "C-API Debug function rb_debug_inspector_open creates a debug context and calls the given callback" => 2,
    "C-API Debug function rb_debug_inspector_frame_self_get returns self" => 2,
    "C-API Debug function rb_debug_inspector_frame_class_get returns the frame class" => 2,
    "C-API Debug function rb_debug_inspector_frame_binding_get returns the current binding" => 2,
    "C-API Debug function rb_debug_inspector_frame_binding_get matches the locations in rb_debug_inspector_backtrace_locations" => 2,
    "C-API Debug function rb_debug_inspector_frame_iseq_get returns an InstructionSequence" => 2,
    "C-API Debug function rb_debug_inspector_backtrace_locations returns an array of Thread::Backtrace::Location" => 2,
    "C-API IO function rb_io_printf calls #to_str to convert the format object to a String" => 2,
    "C-API IO function rb_io_printf calls #to_s to convert the object to a String" => 2,
    "C-API IO function rb_io_printf writes the Strings to the IO" => 4,
    "C-API IO function rb_io_print calls #to_s to convert the object to a String" => 1,
    "C-API IO function rb_io_print writes the Strings to the IO with no separator" => 3,
    "C-API IO function rb_io_puts calls #to_s to convert the object to a String" => 1,
    "C-API IO function rb_io_puts writes the Strings to the IO separated by newlines" => 3,
    "C-API Kernel function rb_rescue executes the passed 'rescue function' if a StandardError exception is raised" => 3,
    "C-API Kernel function rb_rescue passes the user supplied argument to the 'rescue function' if a StandardError exception is raised"