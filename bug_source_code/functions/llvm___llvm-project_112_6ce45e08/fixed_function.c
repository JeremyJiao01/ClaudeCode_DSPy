          SI.getContext(), C.getCaseValue()->getValue().trunc(NewWidth)));
  }

  Value *Op0 = nullptr;
  ConstantInt *AddRHS = nullptr;
  if (match(Cond, m_Add(m_Value(Op0), m_ConstantInt(AddRHS)))) {
    // Change 'switch (X+4) case 1:' into 'switch (X) case -3'.
    for (SwitchInst::CaseIt i = SI.case_begin(), e = SI.case_end(); i != e;
         ++i) {
      ConstantInt *CaseVal = i.getCaseValue();
      Constant *LHS = CaseVal;
      if (TruncCond) {
        LHS = LeadingKnownZeros
                  ? ConstantExpr::getZExt(CaseVal, Cond->getType())
                  : ConstantExpr::getSExt(CaseVal, Cond->getType());
      }
      Constant *NewCaseVal = ConstantExpr::getSub(LHS, AddRHS);
      assert(isa<ConstantInt>(NewCaseVal) &&
             "Result of expression should be constant");
      i.setValue(cast<ConstantInt>(NewCaseVal));
    }
    SI.setCondition(Op0);
    if (auto *CondI = dyn_cast<Instruction>(Cond))
      Worklist.Add(CondI);
    return &SI;
  }