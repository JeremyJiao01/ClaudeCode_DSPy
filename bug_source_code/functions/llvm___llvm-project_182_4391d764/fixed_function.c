    Opts.RoundToAlign = false;
    Opts.NullIsUnknownSize = true;
    uint64_t ObjSize;
    if (getObjectSize(V, ObjSize, DL, TLI, Opts)) {
      APInt KnownDerefBytes(Size.getBitWidth(), ObjSize);
      if (KnownDerefBytes.getBoolValue() && KnownDerefBytes.uge(Size) &&
          isKnownNonZero(V, DL, 0, nullptr, CtxI, DT) && !V->canBeFreed()) {
        // As we recursed through GEPs to get here, we've incrementally