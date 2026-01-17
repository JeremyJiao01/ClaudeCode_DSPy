    uint64_t Size = DT->getSizeInBits();
    uint64_t FieldSize = getBaseTypeSize(DD, DT);
    uint64_t OffsetInBytes;

    if (FieldSize && Size != FieldSize) {
      // Handle bitfield, assume bytes are 8 bits.
      if (DD->getDwarfVersion() < 4)
        addUInt(MemberDie, dwarf::DW_AT_byte_size, None, FieldSize/8);
      addUInt(MemberDie, dwarf::DW_AT_bit_size, None, Size);

      uint64_t Offset = DT->getOffsetInBits();
      uint64_t Align = DT->getAlignInBits() ? DT->getAlignInBits() : FieldSize;
      uint64_t AlignMask = ~(Align - 1);
      // The bits from the start of the storage unit to the start of the field.
      uint64_t StartBitOffset = Offset - (Offset & AlignMask);
      // The byte offset of the field's aligned storage unit inside the struct.
      OffsetInBytes = (Offset - StartBitOffset) / 8;

      if (DD->getDwarfVersion() >= 4)
        addUInt(MemberDie, dwarf::DW_AT_data_bit_offset, None, Offset);
      else {
        uint64_t HiMark = (Offset + FieldSize) & AlignMask;
        uint64_t FieldOffset = (HiMark - FieldSize);
        Offset -= FieldOffset;

        // Maybe we need to work from the other end.
        if (Asm->getDataLayout().isLittleEndian())
          Offset = FieldSize - (Offset + Size);

        addUInt(MemberDie, dwarf::DW_AT_bit_offset, None, Offset);
        OffsetInBytes = FieldOffset >> 3;
      }
    } else
      // This is not a bitfield.
      OffsetInBytes = DT->getOffsetInBits() / 8;

    if (DD->getDwarfVersion() <= 2) {
      DIELoc *MemLocationDie = new (DIEValueAllocator) DIELoc;
      addUInt(*MemLocationDie, dwarf::DW_FORM_data1, dwarf::DW_OP_plus_uconst);
      addUInt(*MemLocationDie, dwarf::DW_FORM_udata, OffsetInBytes);
      addBlock(MemberDie, dwarf::DW_AT_data_member_location, MemLocationDie);
    } else
      addUInt(MemberDie, dwarf::DW_AT_data_member_location, None,
              OffsetInBytes);
  }