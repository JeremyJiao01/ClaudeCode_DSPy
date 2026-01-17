
  fixupBranchWeights(Header, LatchBR, ExitWeight, FallThroughWeight);

  // Update Metadata for count of peeled off iterations.
  unsigned AlreadyPeeled = 0;
  if (auto Peeled = getOptionalIntLoopAttribute(L, PeeledCountMetaData))
    AlreadyPeeled = *Peeled;
  addStringMetadataToLoop(L, PeeledCountMetaData, AlreadyPeeled + PeelCount);

  if (Loop *ParentLoop = L->getParentLoop())
    L = ParentLoop;

  // We modified the loop, update SE.
  SE->forgetTopmostLoop(L);

  // Finally DomtTree must be correct.
  assert(DT->verify(DominatorTree::VerificationLevel::Fast));

  // FIXME: Incrementally update loop-simplify
  simplifyLoop(L, DT, LI, SE, AC, nullptr, PreserveLCSSA);

  NumPeeled++;

  return true;
}