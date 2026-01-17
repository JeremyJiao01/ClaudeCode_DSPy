    std::swap(InnerSel.TrueVal, InnerSel.FalseVal);

  Value *AltCond = nullptr;
  auto matchOuterCond = [OuterSel, &AltCond](auto m_InnerCond) {
    return match(OuterSel.Cond, m_c_LogicalOp(m_InnerCond, m_Value(AltCond)));
  };

  // Finally, match the condition that was driving the outermost `select`,