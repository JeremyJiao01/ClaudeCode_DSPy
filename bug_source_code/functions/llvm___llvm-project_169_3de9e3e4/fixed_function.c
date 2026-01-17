      }

      MemoryAccess *UseAccess = WorkList[I];
      // Simply adding the users of MemoryPhi to the worklist is not enough,
      // because we might miss read clobbers in different iterations of a loop,
      // for example.
      // TODO: Add support for phi translation to handle the loop case.
      if (isa<MemoryPhi>(UseAccess))
        return false;
