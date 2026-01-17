    if (CastInst::isBitOrNoopPointerCastable(Val->getType(), AccessTy, DL))
      return Val;

    TypeSize StoreSize = DL.getTypeStoreSize(Val->getType());
    TypeSize LoadSize = DL.getTypeStoreSize(AccessTy);
    if (TypeSize::isKnownLE(LoadSize, StoreSize))
      if (auto *C = dyn_cast<Constant>(Val))
        return ConstantFoldLoadFromConst(C, AccessTy, DL);