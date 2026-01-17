
    if (Cond.getOpcode() == X86ISD::SETCC && Cond.hasOneUse()) {
      if (auto *CN = dyn_cast<ConstantSDNode>(N1)) {
        unsigned Val = CN->getZExtValue();
        if (Val == 1 || Val == 2 || Val == 3 || Val == 4 || Val == 7 || Val == 8) {
          X86::CondCode CCode = (X86::CondCode)Cond.getConstantOperandVal(0);
          CCode = X86::GetOppositeBranchCondition(CCode);