  // Maximum valid cost increased in this function.
  int CostUpperBound = INT_MAX - InlineConstants::InstrCost - 1;

  unsigned JumpTableSize = 0;
  unsigned NumCaseCluster =
      TTI.getEstimatedNumberOfCaseClusters(SI, JumpTableSize);

  // If suitable for a jump table, consider the cost for the table size and
  // branch to destination.
  if (JumpTableSize) {
    int64_t JTCost = (int64_t)JumpTableSize * InlineConstants::InstrCost +
                     4 * InlineConstants::InstrCost;

    addCost(JTCost, (int64_t)CostUpperBound);
    return false;
  }

  // Considering forming a binary search, we should find the number of nodes
  // which is same as the number of comparisons when lowered. For a given