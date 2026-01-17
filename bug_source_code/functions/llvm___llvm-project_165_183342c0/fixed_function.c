  }

  // Upgrade "Linker Options" module flag to "llvm.linker.options" module-level
  // metadata. Only upgrade if the new option doesn't exist to avoid upgrade
  // multiple times.
  if (!TheModule->getNamedMetadata("llvm.linker.options")) {
    if (Metadata *Val = TheModule->getModuleFlag("Linker Options")) {
      NamedMDNode *LinkerOpts =
          TheModule->getOrInsertNamedMetadata("llvm.linker.options");
      for (const MDOperand &MDOptions : cast<MDNode>(Val)->operands())
        LinkerOpts->addOperand(cast<MDNode>(MDOptions));
    }