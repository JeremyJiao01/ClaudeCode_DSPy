
  Constant *C;
  if (D->hasOneUse() && match(M, m_Constant(C))) {
    // Unfold.
    Value *LHS = Builder.CreateAnd(X, C);
    Value *NotC = Builder.CreateNot(C);