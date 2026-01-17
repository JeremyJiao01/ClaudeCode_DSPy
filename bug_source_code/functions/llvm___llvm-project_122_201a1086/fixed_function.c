  // is a common case, and can save some work, check it now.
  if (Inst.getParent() == CurLoop->getHeader())
    // If there's a throw in the header block, we can't guarantee we'll reach
    // Inst unless we can prove that Inst comes before the potential implicit
    // exit.  At the moment, we use a (cheap) hack for the common case where
    // the instruction of interest is the first one in the block.
    return !SafetyInfo->HeaderMayThrow ||
      Inst.getParent()->getFirstNonPHI() == &Inst;