        Function *CalledFn = CI->getCalledFunction();
        if (!CalledFn)
          return false;
        // Skip over intrinsics since they won't remain as function calls.
        // Important to do this check before the linkage check below so we
        // won't bail out on debug intrinsics, possibly making the generated
        // code dependent on the presence of debug info.
        if (CalledFn->getIntrinsicID() != Intrinsic::not_intrinsic)
          continue;
        if (!CalledFn->hasLocalLinkage())
          return false;