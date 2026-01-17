      Bitfield.setBit((unsigned char)C);
    Value *BitfieldC = B.getInt(Bitfield);

    // Adjust width of "C" to the bitfield width, then mask off the high bits.
    Value *C = B.CreateZExtOrTrunc(CI->getArgOperand(1), BitfieldC->getType());
    C = B.CreateAnd(C, B.getIntN(Width, 0xFF));

    // First check that the bit field access is within bounds.