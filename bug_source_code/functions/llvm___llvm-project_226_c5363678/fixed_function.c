void GVNHoist::checkSafety(CHIArgs C, BasicBlock *BB, GVNHoist::InsKind K,
                           SmallVectorImpl<CHIArg> &Safe) {
  int NumBBsOnAllPaths = MaxNumberOfBBSInPath;
  const Instruction *T = BB->getTerminator();
  for (auto CHI : C) {
    Instruction *Insn = CHI.I;
    if (!Insn) // No instruction was inserted in this CHI.
      continue;
    // If the Terminator is some kind of "exotic terminator" that produces a
    // value (such as InvokeInst, CallBrInst, or CatchSwitchInst) which the CHI
    // uses, it is not safe to hoist the use above the def.
    if (!T->use_empty() && is_contained(Insn->operands(), T))
      continue;
    if (K == InsKind::Scalar) {
      if (safeToHoistScalar(BB, Insn->getParent(), NumBBsOnAllPaths))