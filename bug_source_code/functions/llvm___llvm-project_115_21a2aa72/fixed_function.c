  bool HasLineAttributes = false;
  // Print the successors
  bool canPredictProbs = canPredictBranchProbabilities(MBB);
  if (!SimplifyMIR || !canPredictProbs || !canPredictSuccessors(MBB)) {
    OS.indent(2) << "successors: ";
    for (auto I = MBB.succ_begin(), E = MBB.succ_end(); I != E; ++I) {
      if (I != MBB.succ_begin())
        OS << ", ";