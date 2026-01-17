  if (!S)
    return nullptr;

  V = S->getStepRecurrence(*SE);
  if (!V)
    return nullptr;