    // Find any intervals ending at, or after, LaterIntStart which start
    // before LaterIntEnd.
    auto ILI = IM.lower_bound(LaterIntStart);
    if (ILI != IM.end() && ILI->second <= LaterIntEnd) {
      // This existing interval is overlapped with the current store somewhere
      // in [LaterIntStart, LaterIntEnd]. Merge them by erasing the existing
      // intervals and adjusting our start and end.
      LaterIntStart = std::min(LaterIntStart, ILI->second);
      LaterIntEnd = std::max(LaterIntEnd, ILI->first);
      ILI = IM.erase(ILI);

      // Continue erasing and adjusting our end in case other previous
      // intervals are also overlapped with the current store.
      //
      // |--- ealier 1 ---|  |--- ealier 2 ---|
      //     |------- later---------|
      //
      while (ILI != IM.end() && ILI->second <= LaterIntEnd) {