        continue;

      auto *V = CS.getArgOperand(A->getArgNo());
      // TrackValueOfGlobalVariable only tracks scalar global variables.
      if (auto *GV = dyn_cast<GlobalVariable>(V)) {
        // Check if we want to specialize on the address of non-constant