  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);

  // Spill the BasePtr if it's used.
  if (TRI->hasBasePointer(MF))
    SavedRegs.set(TRI->getBaseRegister());
}

static bool