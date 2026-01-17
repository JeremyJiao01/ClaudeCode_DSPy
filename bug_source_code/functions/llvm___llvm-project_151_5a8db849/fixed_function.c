
    if (auto ParamValue = TII->describeLoadedValue(*I)) {
      if (ParamValue->first.isImm()) {
        int64_t Val = ParamValue->first.getImm();
        DbgValueLoc DbgLocVal(ParamValue->second, Val);
        finishCallSiteParam(DbgLocVal, Reg);
      } else if (ParamValue->first.isReg()) {