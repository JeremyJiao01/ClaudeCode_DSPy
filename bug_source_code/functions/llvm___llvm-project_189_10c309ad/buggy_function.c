      PHINode *NewPhi = dyn_cast<PHINode>(P.clone());
      NewPhi->setIncomingValue(0, P.getIncomingValue(0));
      NewPhi->setIncomingBlock(0, OuterLatch);
      NewPhi->insertBefore(InnerLatch->getFirstNonPHI());
      P.setIncomingValue(0, NewPhi);
    }