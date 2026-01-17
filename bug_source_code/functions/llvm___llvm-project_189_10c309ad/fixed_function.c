      PHINode *NewPhi = dyn_cast<PHINode>(P.clone());
      NewPhi->setIncomingValue(0, P.getIncomingValue(0));
      NewPhi->setIncomingBlock(0, OuterLatch);
      // We might have incoming edges from other BBs, i.e., the original outer
      // header.
      for (auto *Pred : predecessors(InnerLatch)) {