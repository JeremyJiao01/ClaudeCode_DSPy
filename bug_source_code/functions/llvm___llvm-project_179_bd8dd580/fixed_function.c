      return false;
  }

  // Make sure no potentially eflags clobbering phi moves can be inserted in
  // between.
  auto HasPhis = [](const BasicBlock *Succ) {