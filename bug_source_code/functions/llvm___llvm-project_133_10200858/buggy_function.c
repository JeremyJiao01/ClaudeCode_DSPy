  }

  // Otherwise, there is an index.  The computation we will do will be modulo
  // the pointer size, so get it.
  uint64_t PtrSizeMask = ~0ULL >> (64-IntPtrWidth);

  Offset &= PtrSizeMask;
  VariableScale &= PtrSizeMask;

  // To do this transformation, any constant index must be a multiple of the
  // variable scale factor.  For example, we can evaluate "12 + 4*i" as "3 + i",