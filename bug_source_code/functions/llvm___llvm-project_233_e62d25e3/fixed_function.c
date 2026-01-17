                    MDB.createBranchWeights(Info.Weights));
  for (auto [Idx, SubWeight] : enumerate(Info.SubWeights))
    if (SubWeight != 0)
      // Don't set the probability of taking the edge from latch to loop header
      // to less than 1:1 ratio (meaning Weight should not be lower than
      // SubWeight), as this could significantly reduce the loop's hotness,
      // which would be incorrect in the case of underestimating the trip count.
      Info.Weights[Idx] =
          Info.Weights[Idx] > SubWeight