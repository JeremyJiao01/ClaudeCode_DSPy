        if (!Op1CV->isNullValue() && (*Op1CV != KnownZeroMask)) {
          // (X&4) == 2 --> false
          // (X&4) != 2 --> true
          Constant *Res = ConstantInt::get(Type::getInt1Ty(CI.getContext()),
                                           isNE);
          Res = ConstantExpr::getZExt(Res, CI.getType());
          return replaceInstUsesWith(CI, Res);
        }
