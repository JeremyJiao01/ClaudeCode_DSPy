  Value *Offset =
      Builder.CreateShl(Index, ConstantInt::get(IntTy, 2), "reltable.shift");

  // Insert the call to load.relative instrinsic before LOAD.
  // GEP might not be immediately followed by a LOAD, like it can be hoisted
  // outside the loop or another instruction might be inserted them in between.