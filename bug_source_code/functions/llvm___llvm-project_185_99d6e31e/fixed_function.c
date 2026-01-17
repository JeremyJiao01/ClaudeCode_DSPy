  // Note that it is not safe to speculate into a malloc'd region because
  // malloc may return null.

  // Recurse into both hands of select.
  if (const SelectInst *Sel = dyn_cast<SelectInst>(V)) {
    return isDereferenceableAndAlignedPointer(Sel->getTrueValue(), Alignment,