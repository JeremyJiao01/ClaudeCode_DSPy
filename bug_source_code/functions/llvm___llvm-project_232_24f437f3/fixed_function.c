               NewEndOffset == NewAllocaEndOffset &&
               (canConvertValue(DL, NewAllocaTy, TargetTy) ||
                (IsLoadPastEnd && NewAllocaTy->isIntegerTy() &&
                 TargetTy->isIntegerTy() && !LI.isVolatile()))) {
      Value *NewPtr =
          getPtrToNewAI(LI.getPointerAddressSpace(), LI.isVolatile());
      LoadInst *NewLI = IRB.CreateAlignedLoad(NewAI.getAllocatedType(), NewPtr,