                         (NewBonusInst->getName() + ".merge").str());
    SSAUpdate.AddAvailableValue(BB, &BonusInst);
    SSAUpdate.AddAvailableValue(PredBlock, NewBonusInst);
    for (Use &U : make_early_inc_range(BonusInst.uses())) {
      auto *UI = cast<Instruction>(U.getUser());
      if (UI->getParent() != PredBlock)
        SSAUpdate.RewriteUseAfterInsertions(U);
      else // Use is in the same block as, and comes before, NewBonusInst.