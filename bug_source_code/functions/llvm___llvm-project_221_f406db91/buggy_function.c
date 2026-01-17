    uint32_t AlignInBytes = DT->getAlignInBytes();
    uint64_t OffsetInBytes;

    bool IsBitfield = FieldSize && Size != FieldSize;
    if (IsBitfield) {
      // Handle bitfield, assume bytes are 8 bits.
      if (DD->useDWARF2Bitfields())