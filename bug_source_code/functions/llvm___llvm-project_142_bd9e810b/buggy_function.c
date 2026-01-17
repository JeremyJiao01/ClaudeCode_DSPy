  }

  auto *BI = dyn_cast<BranchInst>(LoopLatch->getTerminator());
  if (!BI) {
    LLVM_DEBUG(dbgs() << "Failed to match the latch terminator!\n");
    return None;
  }
  BasicBlock *TrueDest = BI->getSuccessor(0);
  assert(
      (TrueDest == L->getHeader() || BI->getSuccessor(1) == L->getHeader()) &&
      "One of the latch's destinations must be the header");

  auto *ICI = dyn_cast<ICmpInst>(BI->getCondition());
  if (!ICI || !BI->isConditional()) {
    LLVM_DEBUG(dbgs() << "Failed to match the latch condition!\n");
    return None;
  }