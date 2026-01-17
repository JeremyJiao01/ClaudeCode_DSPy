  return true;
}

/// \returns True if all of the values in \p VL are constants.
static bool allConstant(ArrayRef<Value *> VL) {
  for (Value *i : VL)
    if (!isa<Constant>(i))
      return false;
  return true;
}