      Bitfield.setBit((unsigned char)C);
    Value *BitfieldC = B.getInt(Bitfield);

    // First check that the bit field access is within bounds.
    Value *C = B.CreateZExtOrTrunc(CI->getArgOperand(1), BitfieldC->getType());
    Value *Bounds = B.CreateICmp(ICmpInst::ICMP_ULT, C, B.getIntN(Width, Width),
                                 "memchr.bounds");
