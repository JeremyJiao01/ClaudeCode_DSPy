  }

  if (Inst->isTerminator() || isa<PHINode>(Inst) || Inst->isEHPad() ||
      Inst->mayThrow() || !Inst->willReturn())
    return false;

  if (auto *Call = dyn_cast<CallBase>(Inst)) {