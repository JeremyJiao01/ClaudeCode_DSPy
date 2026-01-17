    uint32_t AlignInBytes = DT->getAlignInBytes();
    uint64_t OffsetInBytes;

    bool IsBitfield = DT->isBitField();
    if (IsBitfield) {
      // Handle bitfield, assume bytes are 8 bits.
      if (DD->useDWARF2Bitfields())