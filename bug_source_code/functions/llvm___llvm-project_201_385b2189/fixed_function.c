  const unsigned PtrSize = DL.getPointerSize();

  for (const Argument &A : F->args()) {
    // For the purposes of the byte count suffix, structs returned by pointer
    // do not count as function arguments.
    if (A.hasStructRetAttr())