
  const SCEV *Start = IV->getStart();
  const SCEV *End = RHS;
  if (!isLoopEntryGuardedByCond(L, Cond, getAddExpr(Start, Stride), RHS))
    End = IsSigned ? getSMinExpr(RHS, Start) : getUMinExpr(RHS, Start);

  const SCEV *BECount = computeBECount(getMinusSCEV(Start, End), Stride, false);
