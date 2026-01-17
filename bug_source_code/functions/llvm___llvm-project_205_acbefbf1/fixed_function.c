  }

  // Eliminate redundant IV cycles.
  NumElimIV += Rewriter.replaceCongruentIVs(L, DT, DeadInsts, TTI);

  // Try to convert exit conditions to unsigned and rotate computation
  // out of the loop.  Note: Handles invalidation internally if needed.