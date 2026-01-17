            return Ld;
        }
      }
      // If lower/upper loads are the same and there's no other use of the lower
      // load, then splat the loaded value with a broadcast.
      if (auto *Ld = dyn_cast<LoadSDNode>(peekThroughOneUseBitcasts(SubVec2)))
        if (SubVec2 == SubVec && ISD::isNormalLoad(Ld) && Vec.hasOneUse())
          return DAG.getNode(X86ISD::SUBV_BROADCAST, dl, OpVT, SubVec);

      // If this is subv_broadcast insert into both halves, use a larger
      // subv_broadcast.