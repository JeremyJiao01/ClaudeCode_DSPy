      // Print human readable probabilities as comments.
      OS << "; ";
      for (auto I = succ_begin(), E = succ_end(); I != E; ++I) {
        const BranchProbability &BP = *getProbabilityIterator(I);
        if (I != succ_begin())
          OS << ", ";
        OS << printMBBReference(**I) << '('