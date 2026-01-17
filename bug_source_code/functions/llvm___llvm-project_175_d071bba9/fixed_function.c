                                          PHI->getNumIncomingValues(),
                                          PHI->getName() + ".sroa.phi");
    for (unsigned I = 0, E = PHI->getNumIncomingValues(); I != E; ++I) {
      BasicBlock *B = PHI->getIncomingBlock(I);
      Value *NewVal = nullptr;
      int Idx = NewPN->getBasicBlockIndex(B);
      if (Idx >= 0) {
        NewVal = NewPN->getIncomingValue(Idx);
      } else {
        Instruction *In = cast<Instruction>(PHI->getIncomingValue(I));

        IRBuilderTy B(In->getParent(), std::next(In->getIterator()));
        NewVal = IsInBounds