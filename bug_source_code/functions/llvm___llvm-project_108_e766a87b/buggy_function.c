    // Find any intervals ending at, or after, LaterIntStart which start
    // before LaterIntEnd.
    auto ILI = IM.lower_bound(LaterIntStart);
    if (ILI != IM.end() && ILI->second < LaterIntEnd) {
      // This existing interval ends in the middle of
      // [LaterIntStart, LaterIntEnd), erase it adjusting our start.
      LaterIntStart = std::min(LaterIntStart, ILI->second);
      LaterIntEnd = std::max(LaterIntEnd, ILI->first);
      ILI = IM.erase(ILI);

      while (ILI != IM.end() && ILI->first <= LaterIntEnd)
        ILI = IM.erase(ILI);

      if (ILI != IM.end() && ILI->second < LaterIntEnd)
        LaterIntEnd = std::max(LaterIntEnd, ILI->first);
    }

    IM[LaterIntEnd] = LaterIntStart;