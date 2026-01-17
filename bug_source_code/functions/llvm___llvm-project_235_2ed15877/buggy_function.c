  if (isa<UndefValue>(Token))
    return Token;

  // This takes care both of relocates for call statepoints and relocates
  // on normal path of invoke statepoint.
  if (!isa<LandingPadInst>(Token))