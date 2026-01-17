    BasicBlock *EdgeBB =
        BasicBlock::Create(BB->getContext(), RealDest->getName() + ".critedge",
                           RealDest->getParent(), RealDest);
    BranchInst *CritEdgeBranch = BranchInst::Create(RealDest, EdgeBB);
    CritEdgeBranch->setDebugLoc(BI->getDebugLoc());

    // Update PHI nodes.