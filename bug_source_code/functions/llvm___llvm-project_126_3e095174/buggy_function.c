            return Ld;
        }
      }
      // If lower/upper loads are the same and the only users of the load, then
      // lower to a VBROADCASTF128/VBROADCASTI128/etc.
      if (auto *Ld = dyn_cast<LoadSDNode>(peekThroughOneUseBitcasts(SubVec2)))
        if (SubVec2 == SubVec && ISD::isNormalLoad(Ld) &&
            SDNode::areOnlyUsersOf({N, Vec.getNode()}, SubVec2.getNode()))
          return DAG.getNode(X86ISD::SUBV_BROADCAST, dl, OpVT, SubVec);

      // If this is subv_broadcast insert into both halves, use a larger