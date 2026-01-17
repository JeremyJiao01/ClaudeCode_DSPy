    MVT ShiftVT = MVT::getVectorVT(MVT::i16, NumElts / 2);

    // Simple i8 add case
    if (Op.getOpcode() == ISD::SHL && ShiftAmt == 1) {
      // R may be undef at run-time, but (shl R, 1) must be an even number (LSB
      // must be 0). (add undef, undef) however can be any value. To make this
      // safe, we must freeze R to ensure that register allocation uses the same
      // register for an undefined value. This ensures that the result will