  Changed |= EliminateDuplicatePHINodes(BB);

  // Check for and remove branches that will always cause undefined behavior.
  if (removeUndefIntroducingPredecessor(BB, DTU))
    return requestResimplify();

  // Merge basic blocks into their predecessor if there is only one distinct