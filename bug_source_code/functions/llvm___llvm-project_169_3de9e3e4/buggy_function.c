      }

      MemoryAccess *UseAccess = WorkList[I];
      if (isa<MemoryPhi>(UseAccess)) {
        PushMemUses(UseAccess);
        continue;
      }

      // TODO: Checking for aliasing is expensive. Consider reducing the amount
      // of times this is called and/or caching it.