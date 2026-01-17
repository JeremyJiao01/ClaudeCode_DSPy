
    if (FieldSize && Size != FieldSize) {
      // Handle bitfield, assume bytes are 8 bits.
      if (DD->getDwarfVersion() < 4)
        addUInt(MemberDie, dwarf::DW_AT_byte_size, None, FieldSize/8);
      addUInt(MemberDie, dwarf::DW_AT_bit_size, None, Size);
