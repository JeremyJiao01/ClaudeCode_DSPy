bool Loop::isSafeToClone() const {
  // Return false if any loop blocks contain indirectbrs, or there are any calls
  // to noduplicate functions.
  for (BasicBlock *BB : this->blocks()) {
    if (isa<IndirectBrInst>(BB->getTerminator()))
      return false;

    for (Instruction &I : *BB)
      if (auto *CB = dyn_cast<CallBase>(&I))
        if (CB->cannotDuplicate())
          return false;