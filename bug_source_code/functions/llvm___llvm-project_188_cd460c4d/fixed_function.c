    for (BasicBlock::iterator
             i = ++BasicBlock::iterator(I),
             UI = BasicBlock::iterator(dyn_cast<Instruction>(Use));
         i != UI; ++i) {
      if (i == I->getParent()->end())
        return false;
      if (!isGuaranteedToTransferExecutionToSuccessor(&*i))
        return false;
    }