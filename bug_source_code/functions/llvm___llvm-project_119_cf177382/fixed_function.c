        NGV->setAlignment(NewAlign);

      // Copy over the debug info for the variable.
      uint64_t Size = DL.getTypeAllocSizeInBits(NGV->getValueType());
      uint64_t FragmentOffsetInBits = Layout.getElementOffsetInBits(i);
      transferSRADebugInfo(GV, NGV, FragmentOffsetInBits, Size, NumElements);
    }
  } else if (SequentialType *STy = dyn_cast<SequentialType>(Ty)) {
    unsigned NumElements = STy->getNumElements();
    if (NumElements > 16 && GV->hasNUsesOrMore(16))
      return nullptr; // It's not worth it.
    NewGlobals.reserve(NumElements);
    auto ElTy = STy->getElementType();
    uint64_t EltSize = DL.getTypeAllocSize(ElTy);
    unsigned EltAlign = DL.getABITypeAlignment(ElTy);
    uint64_t FragmentSizeInBits = DL.getTypeAllocSizeInBits(ElTy);
    for (unsigned i = 0, e = NumElements; i != e; ++i) {
      Constant *In = Init->getAggregateElement(i);
      assert(In && "Couldn't get element of initializer?");