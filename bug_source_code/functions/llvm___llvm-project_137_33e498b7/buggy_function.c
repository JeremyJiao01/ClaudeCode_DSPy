    BasicBlock *EdgeBB =
        BasicBlock::Create(BB->getContext(), RealDest->getName() + ".critedge",
                           RealDest->getParent(), RealDest);
    BranchInst::Create(RealDest, EdgeBB);

    // Update PHI nodes.
    AddPredecessorToBlock(RealDest, EdgeBB, BB);