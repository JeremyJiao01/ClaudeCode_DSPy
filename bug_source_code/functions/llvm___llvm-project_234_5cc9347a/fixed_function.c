    std::swap(InnerSel.TrueVal, InnerSel.FalseVal);

  Value *AltCond = nullptr;
  auto matchOuterCond = [OuterSel, IsAndVariant, &AltCond](auto m_InnerCond) {
    // An unsimplified select condition can match both LogicalAnd and LogicalOr
    // (select true, true, false). Since below we assume that LogicalAnd implies
    // InnerSel match the FVal and vice versa for LogicalOr, we can't match the
    // alternative pattern here.