
  // Do not generate something ridiculous.
  auto *PHTerm = Preheader->getTerminator();
  if (Rewriter.isHighCostExpansion({ InvariantLHS, InvariantRHS }, L,
                                   2 * SCEVCheapExpansionBudget, TTI, PHTerm))
    return false;
  auto *NewLHS =
      Rewriter.expandCodeFor(InvariantLHS, IVOperand->getType(), PHTerm);