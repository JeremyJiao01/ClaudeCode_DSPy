    //     4       3
    //     5       3
    //
    for (unsigned i = 0; i < RegCount; ++i) {
      int CUReg = getCompactUnwindRegNum(SavedRegs[i]);
      if (CUReg == -1) return ~0U;
      SavedRegs[i] = CUReg;