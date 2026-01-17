  Value *StorePtr = StoreToHoist->getPointerOperand();

  // Look for a store to the same pointer in BrBB.
  unsigned MaxNumInstToLookAt = 10;
  for (BasicBlock::reverse_iterator RI = BrBB->rbegin(),
       RE = BrBB->rend(); RI != RE && (--MaxNumInstToLookAt); ++RI) {
    Instruction *CurI = &*RI;

    // Could be calling an instruction that effects memory like free().
    if (CurI->mayHaveSideEffects() && !isa<StoreInst>(CurI))