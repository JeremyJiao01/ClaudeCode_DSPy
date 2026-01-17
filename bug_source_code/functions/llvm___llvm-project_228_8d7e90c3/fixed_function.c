
  // Do not generate something ridiculous.
  auto *PHTerm = Preheader->getTerminator();
  if (Rewriter.isHighCostExpansion({InvariantLHS, InvariantRHS}, L,
                                   2 * SCEVCheapExpansionBudget, TTI, PHTerm) ||
      !Rewriter.isSafeToExpandAt(InvariantLHS, PHTerm) ||
      !Rewriter.isSafeToExpandAt(InvariantRHS, PHTerm))
    return false;