
  Constant *C;
  if (D->hasOneUse() && match(M, m_Constant(C))) {
    // Propagating undef is unsafe. Clamp undef elements to -1.
    Type *EltTy = C->getType()->getScalarType();
    C = Constant::replaceUndefsWith(C, ConstantInt::getAllOnesValue(EltTy));