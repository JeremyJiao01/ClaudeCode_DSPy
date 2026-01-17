  return true;
}

/// \returns True if all of the values in \p VL are constants (but not
/// globals/constant expressions).
static bool allConstant(ArrayRef<Value *> VL) {
  // Constant expressions and globals can't be vectorized like normal integer/FP
  // constants.
  for (Value *i : VL)
    if (!isa<Constant>(i) || isa<ConstantExpr>(i) || isa<GlobalValue>(i))