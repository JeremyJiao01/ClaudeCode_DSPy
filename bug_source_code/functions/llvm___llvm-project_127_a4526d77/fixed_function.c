
  if (!isDereferenceableAndAlignedPointer(Str, 1, APInt(64, Len), DL))
    return false;

  if (CI->getFunction()->hasFnAttribute(Attribute::SanitizeMemory))
    return false;
