    if (!DivDominates && !DT.dominates(RemInst, DivInst)) {
      // We have matching div-rem pair, but they are in two different blocks,
      // neither of which dominates one another.
      // FIXME: We could hoist both ops to the common predecessor block?
      continue;
    }

    // The target does not have a single div/rem operation,