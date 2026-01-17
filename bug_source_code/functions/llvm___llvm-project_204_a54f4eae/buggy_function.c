    if (const auto *ExactOp = dyn_cast<PossiblyExactOperator>(Op))
      if (ExactOp->isExact())
        return true;
    if (const auto *FP = dyn_cast<FPMathOperator>(Op)) {
      auto FMF = FP->getFastMathFlags();
      if (FMF.noNaNs() || FMF.noInfs())
        return true;
    }
  }

  unsigned Opcode = Op->getOpcode();