    BFI.reset(new BlockFrequencyInfo(F, *BPI, LI));
  }

  bool Changed = runImpl(F, &TLI, &LVI, &AA, &DTU, F.hasProfileData(),
                         std::move(BFI), std::move(BPI));

  if (!Changed)