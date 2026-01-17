
  if (!isDereferenceableAndAlignedPointer(Str, 1, APInt(64, Len), DL))
    return false;
    
  return true;
}
