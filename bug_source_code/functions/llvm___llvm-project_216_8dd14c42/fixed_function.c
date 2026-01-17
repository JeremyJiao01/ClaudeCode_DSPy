  assert(DT || !MustDominate);
  assert(Verify() && "Invalid PHITransAddr!");
  if (DT && DT->isReachableFromEntry(PredBB))
    Addr = PHITranslateSubExpr(Addr, CurBB, PredBB, DT);
  else
    Addr = nullptr;
  assert(Verify() && "Invalid PHITransAddr!");
