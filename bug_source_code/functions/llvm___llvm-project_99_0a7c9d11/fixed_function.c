  Value *StorePtr = StoreToHoist->getPointerOperand();

  // Look for a store to the same pointer in BrBB.
  unsigned MaxNumInstToLookAt = 9;
  for (BasicBlock::reverse_iterator RI = BrBB->rbegin(),
       RE = BrBB->rend(); RI != RE && MaxNumInstToLookAt; ++RI) {
    Instruction *CurI = &*RI;
    // Skip debug info.
    if (isa<DbgInfoIntrinsic>(CurI))
      continue;