    I0->getOperandUse(O).set(NewOperands[O]);
  I0->moveBefore(&*BBEnd->getFirstInsertionPt());

  // Update metadata.
  for (auto *I : Insts)
    if (I != I0)
      combineMetadataForCSE(I0, I);

  if (!isa<StoreInst>(I0)) {
    // canSinkLastInstruction checked that all instructions were used by