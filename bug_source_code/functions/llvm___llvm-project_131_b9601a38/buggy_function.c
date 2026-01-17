    for (MCRegAliasIterator AS(Reg, TRI, true); AS.isValid(); ++AS) {
      if (PhysRegDefs.test(*AS))
        PhysRegClobbers.set(*AS);
      PhysRegDefs.set(*AS);
    }
    if (PhysRegClobbers.test(Reg))
      // MI defined register is seen defined by another instruction in
      // the loop, it cannot be a LICM candidate.