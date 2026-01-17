  if (!S)
    return nullptr;

  // If the pointer is invariant then there is no stride and it makes no
  // sense to add it here.
  if (Lp != S->getLoop())