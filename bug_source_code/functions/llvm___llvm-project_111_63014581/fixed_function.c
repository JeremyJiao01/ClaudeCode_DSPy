    I0->getOperandUse(O).set(NewOperands[O]);
  I0->moveBefore(&*BBEnd->getFirstInsertionPt());

  // Update metadata and IR flags.
  for (auto *I : Insts)
    if (I != I0) {
      combineMetadataForCSE(I0, I);
      I0->andIRFlags(I);
    }
