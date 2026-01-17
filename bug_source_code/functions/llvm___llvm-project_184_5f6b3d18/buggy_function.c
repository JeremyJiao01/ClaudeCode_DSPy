                  InnerLoopPreHeader, DTUpdates, /*MustUpdateOnce=*/false);
  // The outer loop header might or might not branch to the outer latch.
  // We are guaranteed to branch to the inner loop preheader.
  if (llvm::is_contained(OuterLoopHeaderBI->successors(), OuterLoopLatch))
    updateSuccessor(OuterLoopHeaderBI, OuterLoopLatch, LoopExit, DTUpdates,
                    /*MustUpdateOnce=*/false);
  updateSuccessor(OuterLoopHeaderBI, InnerLoopPreHeader,
                  InnerLoopHeaderSuccessor, DTUpdates,
                  /*MustUpdateOnce=*/false);