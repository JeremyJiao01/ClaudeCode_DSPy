    if (HasSafePathToCall &&
        GetBasicARCInstKind(Retain) == ARCInstKind::RetainRV &&
        GetBasicARCInstKind(Autorelease) == ARCInstKind::AutoreleaseRV &&
        !cast<CallInst>(*DependingInstructions.begin())->isTailCall())
      continue;

    DependingInstructions.clear();
    Visited.clear();