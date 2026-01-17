  assert(DT || !MustDominate);
  assert(Verify() && "Invalid PHITransAddr!");
  if (DT && DT->isReachableFromEntry(PredBB))
    Addr =
        PHITranslateSubExpr(Addr, CurBB, PredBB, MustDominate ? DT : nullptr);
  else
    Addr = nullptr;
  assert(Verify() && "Invalid PHITransAddr!");