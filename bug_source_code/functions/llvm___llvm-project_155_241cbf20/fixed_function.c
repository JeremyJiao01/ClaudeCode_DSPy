  Type *Ty = Add->getType();
  CmpInst::Predicate Pred = Cmp.getPredicate();

  // If the add does not wrap, we can always adjust the compare by subtracting
  // the constants. Equality comparisons are handled elsewhere. SGE/SLE/UGE/ULE
  // are canonicalized to SGT/SLT/UGT/ULT.
  if ((Add->hasNoSignedWrap() &&
       (Pred == ICmpInst::ICMP_SGT || Pred == ICmpInst::ICMP_SLT)) ||
      (Add->hasNoUnsignedWrap() &&
       (Pred == ICmpInst::ICMP_UGT || Pred == ICmpInst::ICMP_ULT))) {
    bool Overflow;
    APInt NewC =
        Cmp.isSigned() ? C.ssub_ov(*C2, Overflow) : C.usub_ov(*C2, Overflow);
    // If there is overflow, the result must be true or false.
    // TODO: Can we assert there is no overflow because InstSimplify always
    // handles those cases?
    if (!Overflow)
      // icmp Pred (add nsw X, C2), C --> icmp Pred X, (C - C2)
      return new ICmpInst(Pred, X, ConstantInt::get(Ty, NewC));
  }

  auto CR = ConstantRange::makeExactICmpRegion(Pred, C).subtract(*C2);
  const APInt &Upper = CR.getUpper();
  const APInt &Lower = CR.getLower();
  if (Cmp.isSigned()) {
    if (Lower.isSignMask())
      return new ICmpInst(ICmpInst::ICMP_SLT, X, ConstantInt::get(Ty, Upper));
    if (Upper.isSignMask())
      return new ICmpInst(ICmpInst::ICMP_SGE, X, ConstantInt::get(Ty, Lower));
  } else {
    if (Lower.isMinValue())
      return new ICmpInst(ICmpInst::ICMP_ULT, X, ConstantInt::get(Ty, Upper));
    if (Upper.isMinValue())
      return new ICmpInst(ICmpInst::ICMP_UGE, X, ConstantInt::get(Ty, Lower));
  }

  if (!Add->hasOneUse())
    return nullptr;

  // X+C <u C2 -> (X & -C2) == C
  //   iff C & (C2-1) == 0
  //       C2 is a power of 2