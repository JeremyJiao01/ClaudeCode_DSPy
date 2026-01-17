  if (isa<UndefValue>(Token))
    return Token;

  // Treat none token as if it was undef here
  if (isa<ConstantTokenNone>(Token))
    return UndefValue::get(Token->getType());