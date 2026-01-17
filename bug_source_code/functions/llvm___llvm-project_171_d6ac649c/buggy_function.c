  if (ExpoF->isNegative() && (!Pow->hasApproxFunc() && !Pow->hasAllowReassoc()))
    return nullptr;

  Sqrt = getSqrtCall(Base, Attrs, Pow->doesNotAccessMemory(), Mod, B, TLI);
  if (!Sqrt)
    return nullptr;