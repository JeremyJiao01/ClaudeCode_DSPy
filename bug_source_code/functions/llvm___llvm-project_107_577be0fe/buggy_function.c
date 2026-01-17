    DEBUG(dbgs() << "  Not unrolling loop with inlinable calls.\n");
    return false;
  }

  // Find trip count and trip multiple if count is not available
  unsigned TripCount = 0;