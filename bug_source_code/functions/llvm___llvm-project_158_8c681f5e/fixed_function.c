    // Don't do this if it would create a PHI node with an illegal type from a
    // legal type.
    if (!Src->getType()->isIntegerTy() || !CI.getType()->isIntegerTy() ||
        shouldChangeType(CI.getSrcTy(), CI.getType()))
      if (Instruction *NV = foldOpIntoPhi(CI, PN))
        return NV;
  }