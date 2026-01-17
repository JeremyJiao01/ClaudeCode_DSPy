bool AtomicExpand::expandAtomicLoadToCmpXchg(LoadInst *LI) {
  IRBuilder<> Builder(LI);
  AtomicOrdering Order = LI->getOrdering();
  Value *Addr = LI->getPointerOperand();
  Type *Ty = cast<PointerType>(Addr->getType())->getElementType();
  Constant *DummyVal = Constant::getNullValue(Ty);