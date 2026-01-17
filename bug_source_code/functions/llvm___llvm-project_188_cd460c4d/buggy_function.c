    for (BasicBlock::iterator
             i = ++BasicBlock::iterator(I),
             UI = BasicBlock::iterator(dyn_cast<Instruction>(Use));
         i != UI; ++i)
      if (i == I->getParent()->end() || i->mayHaveSideEffects())
        return false;

    // Look through GEPs. A load from a GEP derived from NULL is still undefined
    if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(Use))