  for (Use &U : Inst->uses()) {
    Instruction *UseInst = cast<Instruction>(U.getUser());
    BasicBlock *UseBlock = UseInst->getParent();
    // Don't worry about dead users.
    if (!DT.isReachableFromEntry(UseBlock))
      continue;