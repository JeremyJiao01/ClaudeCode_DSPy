    if (HasSafePathToCall &&
        GetBasicARCInstKind(Retain) == ARCInstKind::RetainRV &&
        GetBasicARCInstKind(Autorelease) == ARCInstKind::AutoreleaseRV &&
        !cast<CallInst>(*DependingInstructions.begin())->isTailCall()) {
      DependingInstructions.clear();
      Visited.clear();
      continue;
    }