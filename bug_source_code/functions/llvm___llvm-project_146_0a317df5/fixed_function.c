                                    Options, ForCodeSize, Depth + 1))
      return V;

    // Ignore X * 2.0 because that is expected to be canonicalized to X + X.
    if (auto *C = isConstOrConstSplatFP(Op.getOperand(1)))
      if (C->isExactlyValue(2.0) && Op.getOpcode() == ISD::FMUL)