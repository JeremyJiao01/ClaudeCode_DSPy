  // instead of a select to synthesize the desired value.
  bool IsOneZero = false;
  bool EmitOneOrZero = true;
  if (ConstantInt *CI = dyn_cast<ConstantInt>(OtherVal)){
    IsOneZero = InitVal->isNullValue() && CI->isOne();

    if (ConstantInt *CIInit = dyn_cast<ConstantInt>(GV->getInitializer())){
      uint64_t ValInit = CIInit->getZExtValue();
      uint64_t ValOther = CI->getZExtValue();
      uint64_t ValMinus = ValOther - ValInit;