    // ranges for all subprogram DIEs for mach-o.
    DwarfCompileUnit &U = SkCU ? *SkCU : TheCU;

    if (unsigned NumRanges = TheCU.getRanges().size()) {
      if (NumRanges > 1 && useRangesSection())
        // A DW_AT_low_pc attribute may also be specified in combination with
        // DW_AT_ranges to specify the default base address for use in
        // location lists (see Section 2.6.2) and range lists (see Section
        // 2.17.3).
        U.addUInt(U.getUnitDie(), dwarf::DW_AT_low_pc, dwarf::DW_FORM_addr, 0);
      else
        U.setBaseAddress(TheCU.getRanges().front().getStart());
      U.attachRangesOrLowHighPC(U.getUnitDie(), TheCU.takeRanges());
    }

    // We don't keep track of which addresses are used in which CU so this
    // is a bit pessimistic under LTO.
    if (!AddrPool.isEmpty() &&
        (getDwarfVersion() >= 5 ||
         (SkCU && !empty(TheCU.getUnitDie().children()))))
      U.addAddrTableBase();

    if (getDwarfVersion() >= 5) {
      if (U.hasRangeLists())
        U.addRnglistsBase();