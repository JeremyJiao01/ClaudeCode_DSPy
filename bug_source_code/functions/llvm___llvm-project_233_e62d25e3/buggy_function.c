                    MDB.createBranchWeights(Info.Weights));
  for (auto [Idx, SubWeight] : enumerate(Info.SubWeights))
    if (SubWeight != 0)
      Info.Weights[Idx] = Info.Weights[Idx] > SubWeight
                              ? Info.Weights[Idx] - SubWeight
                              : 1;
}

/// Initialize the weights for all exiting blocks.