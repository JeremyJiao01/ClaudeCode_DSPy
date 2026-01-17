  }

  // If we aren't dealing with a constant on the RHS, exit early.
  auto *CI = dyn_cast<ConstantInt>(ICI.getOperand(1));
  if (!CI)
    return nullptr;
