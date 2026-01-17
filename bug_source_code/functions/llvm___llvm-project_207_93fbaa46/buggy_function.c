  Value *Offset =
      Builder.CreateShl(Index, ConstantInt::get(IntTy, 2), "reltable.shift");

  Function *LoadRelIntrinsic = llvm::Intrinsic::getDeclaration(
      &M, Intrinsic::load_relative, {Index->getType()});
  Value *Base = Builder.CreateBitCast(RelLookupTable, Builder.getInt8PtrTy());