
  auto NewFoldedConst = [&](bool IsTrueArm, Value *V) {
    bool IsCastOpRHS = (CastOp == RHS);
    bool IsZExt = isa<ZExtInst>(CastOp);
    Constant *C;

    if (IsTrueArm) {