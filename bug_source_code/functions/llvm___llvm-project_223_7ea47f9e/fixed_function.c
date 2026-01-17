    LoopVectorizeHints Hints(L, true, *ORE);
    Hints.setAlreadyVectorized();
  }
  AddRuntimeUnrollDisableMetaData(L);

  // 3. Fix the vectorized code: take care of header phi's, live-outs,
  //    predication, updating analyses.
  ILV.fixVectorizedLoop(State, BestVPlan);
