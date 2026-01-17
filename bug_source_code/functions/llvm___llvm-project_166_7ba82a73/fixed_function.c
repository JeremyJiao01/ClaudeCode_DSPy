
  const SCEV *Start = IV->getStart();
  const SCEV *End = RHS;
  if (!isLoopEntryGuardedByCond(L, Cond, getAddExpr(Start, Stride), RHS)) {
    // If we know that Start >= RHS in the context of loop, then we know that
    // min(RHS, Start) = RHS at this point.
    if (isLoopEntryGuardedByCond(
            L, IsSigned ? ICmpInst::ICMP_SGE : ICmpInst::ICMP_UGE, Start, RHS))