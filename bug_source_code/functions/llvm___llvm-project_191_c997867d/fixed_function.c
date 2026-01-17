  for (BasicBlock *Pred : predecessors(Entry))
    if (R->contains(Pred))
      return nullptr;
  // If any of the basic blocks have address taken, we must skip this region
  // because we cannot clone basic blocks that have address taken.
  for (BasicBlock *BB : R->blocks())