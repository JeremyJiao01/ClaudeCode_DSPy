    MVT ShiftVT = MVT::getVectorVT(MVT::i16, NumElts / 2);

    // Simple i8 add case
    if (Op.getOpcode() == ISD::SHL && ShiftAmt == 1)
      return DAG.getNode(ISD::ADD, dl, VT, R, R);

    // ashr(R, 7)  === cmp_slt(R, 0)
    if (Op.getOpcode() == ISD::SRA && ShiftAmt == 7) {