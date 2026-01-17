    return Constant::getAllOnesValue(Op1->getType());

  // A | ~(A & ?) = -1
  if (match(Op1, m_Not(m_c_And(m_Specific(Op1), m_Value()))))
    return Constant::getAllOnesValue(Op0->getType());

  Value *A, *B;