      // If we hit load/store with the same invariant.group metadata (and the
      // same pointer operand) we can assume that value pointed by pointer
      // operand didn't change.
      if ((isa<LoadInst>(U) || isa<StoreInst>(U)) &&
          U->hasMetadata(LLVMContext::MD_invariant_group))
        ClosestDependency = GetClosestDependency(ClosestDependency, U);
    }