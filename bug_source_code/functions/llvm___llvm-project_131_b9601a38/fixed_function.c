    for (MCRegAliasIterator AS(Reg, TRI, true); AS.isValid(); ++AS) {
      if (PhysRegDefs.test(*AS))
        PhysRegClobbers.set(*AS);
    }
    // Need a second loop because MCRegAliasIterator can visit the same
    // register twice.
    for (MCRegAliasIterator AS(Reg, TRI, true); AS.isValid(); ++AS)
      PhysRegDefs.set(*AS);