    if (!MathDominates && !DT.dominates(Cmp, BO))
      return false;

    BasicBlock *MathBB = BO->getParent(), *CmpBB = Cmp->getParent();
    if (MathBB != CmpBB) {
      // Avoid hoisting an extra op into a dominating block and creating a
      // potentially longer critical path.
      if (!MathDominates)
        return false;
      // Check that the insertion doesn't create a value that is live across