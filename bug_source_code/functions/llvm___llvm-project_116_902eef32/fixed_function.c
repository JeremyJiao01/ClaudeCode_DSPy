        if (!Op1CV->isNullValue() && (*Op1CV != KnownZeroMask)) {
          // (X&4) == 2 --> false
          // (X&4) != 2 --> true
          Constant *Res = ConstantInt::get(CI.getType(), isNE);
          return replaceInstUsesWith(CI, Res);
        }

        uint32_t ShAmt = KnownZeroMask.logBase2();
        Value *In = ICI->getOperand(0);