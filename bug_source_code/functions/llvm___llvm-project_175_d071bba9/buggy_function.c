                                          PHI->getNumIncomingValues(),
                                          PHI->getName() + ".sroa.phi");
    for (unsigned I = 0, E = PHI->getNumIncomingValues(); I != E; ++I) {
      Instruction *In = cast<Instruction>(PHI->getIncomingValue(I));

      IRBuilderTy B(In->getParent(), std::next(In->getIterator()));
      Value *NewVal = IsInBounds
          ? B.CreateInBoundsGEP(In, Index, In->getName() + ".sroa.gep")
          : B.CreateGEP(In, Index, In->getName() + ".sroa.gep");
      NewPN->addIncoming(NewVal, PHI->getIncomingBlock(I));
    }

    Visited.erase(&GEPI);