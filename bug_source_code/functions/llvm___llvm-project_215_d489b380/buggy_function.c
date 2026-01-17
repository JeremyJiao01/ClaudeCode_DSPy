bool Loop::isSafeToClone() const {
  // Return false if any loop blocks contain indirectbrs, or there are any calls
  // to noduplicate functions.
  // FIXME: it should be ok to clone CallBrInst's if we correctly update the
  // operand list to reflect the newly cloned labels.
  for (BasicBlock *BB : this->blocks()) {
    if (isa<IndirectBrInst>(BB->getTerminator()) ||
        isa<CallBrInst>(BB->getTerminator()))
      return false;

    for (Instruction &I : *BB)