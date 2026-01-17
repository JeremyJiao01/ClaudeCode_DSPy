      continue;
    }

    // If we end up with a pointer exit count, bail.
    if (!ExitCount->getType()->isIntegerTy() ||
        !MaxExitCount->getType()->isIntegerTy())
      return false;
    
    Type *WiderType =
      SE->getWiderType(MaxExitCount->getType(), ExitCount->getType());