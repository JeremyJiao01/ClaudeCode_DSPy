  }

  // Upgrade "Linker Options" module flag to "llvm.linker.options" module-level
  // metadata.
  if (Metadata *Val = TheModule->getModuleFlag("Linker Options")) {
    NamedMDNode *LinkerOpts =
        TheModule->getOrInsertNamedMetadata("llvm.linker.options");
    for (const MDOperand &MDOptions : cast<MDNode>(Val)->operands())
      LinkerOpts->addOperand(cast<MDNode>(MDOptions));
  }

  DeferredMetadataInfo.clear();