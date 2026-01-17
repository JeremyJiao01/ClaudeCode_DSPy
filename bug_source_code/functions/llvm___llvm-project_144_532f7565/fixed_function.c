      continue;
    }

    // If we end up with a pointer exit count, bail.  Note that we can end up
    // with a pointer exit count for one exiting block, and not for another in
    // the same loop.
    if (!ExitCount->getType()->isIntegerTy() ||
        !MaxExitCount->getType()->isIntegerTy())
      continue;
    