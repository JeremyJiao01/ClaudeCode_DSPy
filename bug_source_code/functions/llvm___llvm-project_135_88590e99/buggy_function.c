    // Replace the return value with the argument.
    assert((IsForwarding(GetBasicARCInstKind(CI)) ||
            (IsNoopOnNull(GetBasicARCInstKind(CI)) &&
             isa<ConstantPointerNull>(OldArg))) &&
           "Can't delete non-forwarding instruction with users!");
    CI->replaceAllUsesWith(OldArg);
  }