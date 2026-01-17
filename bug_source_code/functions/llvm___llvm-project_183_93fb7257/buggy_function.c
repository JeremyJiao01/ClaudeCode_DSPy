      // profile will be used to prorate callsites from the callee if
      // inlined. Once not inlined, the direct callsite distribution should
      // be prorated so that the it will reflect the real callsite counts.
      setProbeDistributionFactor(CI, Candidate.CallsiteDistribution * Sum /
                                         SumOrigin);
      Candidate.CallInstr = DI;
      if (isa<CallInst>(DI) || isa<InvokeInst>(DI)) {
        bool Inlined = tryInlineCandidate(Candidate, InlinedCallSite);
        if (!Inlined) {
          // Prorate the direct callsite distribution so that it reflects real
          // callsite counts.
          setProbeDistributionFactor(*DI, Candidate.CallsiteDistribution *
                                              Candidate.CallsiteCount /
                                              SumOrigin);
        }
        return Inlined;
      }