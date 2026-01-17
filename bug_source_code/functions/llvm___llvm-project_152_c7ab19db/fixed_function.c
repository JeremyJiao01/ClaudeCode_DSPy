        if (CallBase *CB = dyn_cast<CallBase>(U.getUser()))
          if (CB->isCallee(&U)) {
            Constant *RVCCast =
                CB->getType() == RVC->getType()
                    ? RVC
                    : ConstantExpr::getTruncOrBitCast(RVC, CB->getType());
            Changed = ReplaceCallSiteUsersWith(*CB, *RVCCast) | Changed;
          }
    } else {
      assert(isa<CallBase>(AnchorValue) &&
             "Expcected a function or call base anchor!");
      Constant *RVCCast =
          AnchorValue.getType() == RVC->getType()
              ? RVC