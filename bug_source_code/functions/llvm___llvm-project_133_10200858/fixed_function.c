  }

  // Otherwise, there is an index.  The computation we will do will be modulo
  // the pointer size.
  Offset = SignExtend64(Offset, IntPtrWidth);
  VariableScale = SignExtend64(VariableScale, IntPtrWidth);

  // To do this transformation, any constant index must be a multiple of the
  // variable scale factor.  For example, we can evaluate "12 + 4*i" as "3 + i",
  // but we can't evaluate "10 + 3*i" in terms of i.  Check that the offset is a
  // multiple of the variable scale.