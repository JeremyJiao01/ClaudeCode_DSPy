
  // If the last (struct) indices are constants and are equal, the other indices
  // might be also be dynamically equal, so the GEPs can alias.
  if (C1 && C2 && C1 == C2)
    return MayAlias;

  // Find the last-indexed type of the GEP, i.e., the type you'd get if