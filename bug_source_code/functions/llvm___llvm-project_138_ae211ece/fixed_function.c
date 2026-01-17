bool AtomicExpand::expandAtomicLoadToCmpXchg(LoadInst *LI) {
  IRBuilder<> Builder(LI);
  AtomicOrdering Order = LI->getOrdering();
  if (Order == AtomicOrdering::Unordered)
    Order = AtomicOrdering::Monotonic;
