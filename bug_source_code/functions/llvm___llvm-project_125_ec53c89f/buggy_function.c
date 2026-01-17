
  // Preserve Debug Info.
  for (unsigned i = 0, e = From->getNumValues(); i != e; ++i)
    transferDbgValues(SDValue(From, i), *To);

  // Iterate over just the existing users of From. See the comments in
  // the ReplaceAllUsesWith above.