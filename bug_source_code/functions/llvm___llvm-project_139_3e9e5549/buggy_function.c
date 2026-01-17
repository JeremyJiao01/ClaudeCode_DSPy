    if (!MathDominates && !DT.dominates(Cmp, BO))
      return false;

    // Check that the insertion doesn't create a value that is live across more
    // than two blocks, so to minimise the increase in register pressure.
    BasicBlock *MathBB = BO->getParent(), *CmpBB = Cmp->getParent();
    if (MathBB != CmpBB) {
      BasicBlock *Dominator = MathDominates ? MathBB : CmpBB;
      BasicBlock *Dominated = MathDominates ? CmpBB : MathBB;
      auto Successors = successors(Dominator);