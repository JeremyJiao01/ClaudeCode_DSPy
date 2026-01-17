  for (BasicBlock *Pred : predecessors(Entry))
    if (R->contains(Pred))
      return nullptr;
  if (Exit) {
    // Try to find an if-then block (check if R is an if-then).
    // if (cond) {