  // have to do anything here to lower funclet bundles.
  assert(!I.hasOperandBundlesOtherThan({LLVMContext::OB_deopt,
                                        LLVMContext::OB_gc_transition,
                                        LLVMContext::OB_funclet,
                                        LLVMContext::OB_cfguardtarget}) &&
         "Cannot lower invokes with arbitrary operand bundles yet!");