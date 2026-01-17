void GVNHoist::checkSafety(CHIArgs C, BasicBlock *BB, GVNHoist::InsKind K,
                           SmallVectorImpl<CHIArg> &Safe) {
  int NumBBsOnAllPaths = MaxNumberOfBBSInPath;
  for (auto CHI : C) {
    Instruction *Insn = CHI.I;
    if (!Insn) // No instruction was inserted in this CHI.
      continue;
    if (K == InsKind::Scalar) {
      if (safeToHoistScalar(BB, Insn->getParent(), NumBBsOnAllPaths))
        Safe.push_back(CHI);
    } else {
      auto *T = BB->getTerminator();
      if (MemoryUseOrDef *UD = MSSA->getMemoryAccess(Insn))
        if (safeToHoistLdSt(T, Insn, UD, K, NumBBsOnAllPaths))
          Safe.push_back(CHI);