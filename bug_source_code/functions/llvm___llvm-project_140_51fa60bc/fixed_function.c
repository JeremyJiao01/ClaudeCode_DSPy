  // TODO: This should be sunk inside SCEV.
  if (const SCEVUnknown *U = dyn_cast<SCEVUnknown>(S))
    if (const auto *LI = dyn_cast<LoadInst>(U->getValue()))
      if (LI->isUnordered() && L->hasLoopInvariantOperands(LI))
        if (AA->pointsToConstantMemory(LI->getOperand(0)) ||
            LI->getMetadata(LLVMContext::MD_invariant_load) != nullptr)
          return true;