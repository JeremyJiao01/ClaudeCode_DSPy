  }

  if (Inst->isTerminator() || isa<PHINode>(Inst) || Inst->isEHPad() ||
      Inst->mayThrow())
    return false;

  if (auto *Call = dyn_cast<CallBase>(Inst)) {