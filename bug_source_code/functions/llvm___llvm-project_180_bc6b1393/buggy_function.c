                         (NewBonusInst->getName() + ".merge").str());
    SSAUpdate.AddAvailableValue(BB, &BonusInst);
    SSAUpdate.AddAvailableValue(PredBlock, NewBonusInst);
    for (Use &U : make_early_inc_range(BonusInst.uses()))
      SSAUpdate.RewriteUseAfterInsertions(U);
  }
}
