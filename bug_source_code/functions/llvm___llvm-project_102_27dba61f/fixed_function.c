  }

  // The usual vector types are ConstantDataVector. Exotic vector types are
  // ConstantVector. Zeros are special. They all derive from Constant.
  if (isa<ConstantDataVector>(Op1) || isa<ConstantVector>(Op1) ||
      isa<ConstantAggregateZero>(Op1)) {
    Constant *Op1C = cast<Constant>(Op1);
    Type *Op1Type = Op1->getType();