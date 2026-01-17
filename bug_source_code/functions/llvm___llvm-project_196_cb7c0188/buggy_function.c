  Changed |= EliminateDuplicatePHINodes(BB);

  // Check for and remove branches that will always cause undefined behavior.
  Changed |= removeUndefIntroducingPredecessor(BB, DTU);

  // Merge basic blocks into their predecessor if there is only one distinct
  // pred, and if there is only one distinct successor of the predecessor, and