  // Note that it is not safe to speculate into a malloc'd region because
  // malloc may return null.

  // bitcast instructions are no-ops as far as dereferenceability is concerned.
  if (const BitCastOperator *BC = dyn_cast<BitCastOperator>(V)) {
    if (BC->getSrcTy()->isPointerTy())