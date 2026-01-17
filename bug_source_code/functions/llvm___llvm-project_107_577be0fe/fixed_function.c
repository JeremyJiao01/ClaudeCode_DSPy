    DEBUG(dbgs() << "  Not unrolling loop with inlinable calls.\n");
    return false;
  }
  if (!L->isLoopSimplifyForm()) {
    DEBUG(
        dbgs() << "  Not unrolling loop which is not in loop-simplify form.\n");