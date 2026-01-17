        if (CallBase *CB = dyn_cast<CallBase>(U.getUser()))
          if (CB->isCallee(&U)) {
            Constant *RVCCast =
                ConstantExpr::getTruncOrBitCast(RVC, CB->getType());
            Changed = ReplaceCallSiteUsersWith(*CB, *RVCCast) | Changed;
          }
    } else {
      assert(isa<CallBase>(AnchorValue) &&
             "Expcected a function or call base anchor!");
      Constant *RVCCast =
          ConstantExpr::getTruncOrBitCast(RVC, AnchorValue.getType());
      Changed = ReplaceCallSiteUsersWith(cast<CallBase>(AnchorValue), *RVCCast);
    }
    if (Changed == ChangeStatus::CHANGED)