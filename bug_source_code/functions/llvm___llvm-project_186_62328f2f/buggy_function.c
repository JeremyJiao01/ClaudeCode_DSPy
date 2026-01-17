    }
    // PMOVMSKB(PACKSSBW(LO(X), HI(X)))
    // -> PMOVMSKB(BITCAST_v32i8(X)) & 0xAAAAAAAA.
    if (CmpBits == 16 && Subtarget.hasInt256() &&
        VecOp0.getOpcode() == ISD::EXTRACT_SUBVECTOR &&
        VecOp1.getOpcode() == ISD::EXTRACT_SUBVECTOR &&
        VecOp0.getOperand(0) == VecOp1.getOperand(0) &&