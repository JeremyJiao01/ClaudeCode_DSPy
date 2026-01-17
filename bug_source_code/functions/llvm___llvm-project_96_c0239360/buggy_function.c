    //     4       3
    //     5       3
    //
    for (unsigned i = 0; i != CU_NUM_SAVED_REGS; ++i) {
      int CUReg = getCompactUnwindRegNum(SavedRegs[i]);
      if (CUReg == -1) return ~0U;
      SavedRegs[i] = CUReg;