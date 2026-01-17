    // For stores we already checked if we can follow them, if they make it
    // here we give up.
    if (isa<StoreInst>(UInst))
      return isCapturedIn(State, /* Memory */ true, /* Integer */ true,
                          /* Return */ true);

    // Explicitly catch return instructions.
    if (isa<ReturnInst>(UInst)) {