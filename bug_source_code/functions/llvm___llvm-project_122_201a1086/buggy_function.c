  // is a common case, and can save some work, check it now.
  if (Inst.getParent() == CurLoop->getHeader())
    // If there's a throw in the header block, we can't guarantee we'll reach
    // Inst.
    return !SafetyInfo->HeaderMayThrow;

  // Somewhere in this loop there is an instruction which may throw and make us
  // exit the loop.