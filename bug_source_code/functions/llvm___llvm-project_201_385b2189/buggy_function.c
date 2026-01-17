  const unsigned PtrSize = DL.getPointerSize();

  for (const Argument &A : F->args()) {
    // 'Dereference' type in case of byval or inalloca parameter attribute.
    uint64_t AllocSize = A.hasPassPointeeByValueCopyAttr() ?
      A.getPassPointeeByValueCopySize(DL) :