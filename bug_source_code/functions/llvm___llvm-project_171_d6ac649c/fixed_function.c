  if (ExpoF->isNegative() && (!Pow->hasApproxFunc() && !Pow->hasAllowReassoc()))
    return nullptr;

  // If we have a pow() library call (accesses memory) and we can't guarantee
  // that the base is not an infinity, give up:
  // pow(-Inf, 0.5) is optionally required to have a result of +Inf (not setting