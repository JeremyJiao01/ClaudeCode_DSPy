        Function *CalledFn = CI->getCalledFunction();
        if (!CalledFn)
          return false;
        if (!CalledFn->hasLocalLinkage())
          return false;
        // Skip over intrinsics since they won't remain as function calls.
        if (CalledFn->getIntrinsicID() != Intrinsic::not_intrinsic)
          continue;
        // Check if it's valid to use coldcc calling convention.
        if (!hasChangeableCC(CalledFn) || CalledFn->isVarArg() ||
            CalledFn->hasAddressTaken())