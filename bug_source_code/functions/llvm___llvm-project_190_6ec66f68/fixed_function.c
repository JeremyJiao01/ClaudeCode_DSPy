
    // Find missing lines.
    for (Instruction &I : instructions(F)) {
      if (isa<DbgValueInst>(&I))
        continue;

      auto DL = I.getDebugLoc();
      if (DL && DL.getLine() != 0) {
        MissingLines.reset(DL.getLine() - 1);
        continue;
      }

      if (!isa<PHINode>(&I) && !DL) {
        dbg() << "WARNING: Instruction with empty DebugLoc in function ";
        dbg() << F.getName() << " --";
        I.print(dbg());