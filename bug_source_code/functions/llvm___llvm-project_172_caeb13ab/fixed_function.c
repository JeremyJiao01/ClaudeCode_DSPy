  if (!SI->isUnordered())
    return LegalStoreKind::None;

  // Avoid merging nontemporal stores.
  if (SI->getMetadata(LLVMContext::MD_nontemporal))
    return LegalStoreKind::None;

  Value *StoredVal = SI->getValueOperand();
  Value *StorePtr = SI->getPointerOperand();

  // Don't convert stores of non-integral pointer types to memsets (which stores
  // integers).
  if (DL->isNonIntegralPointerType(StoredVal->getType()->getScalarType()))
    return LegalStoreKind::None;

  // Reject stores that are so large that they overflow an unsigned.
  // When storing out scalable vectors we bail out for now, since the code
  // below currently only works for constant strides.