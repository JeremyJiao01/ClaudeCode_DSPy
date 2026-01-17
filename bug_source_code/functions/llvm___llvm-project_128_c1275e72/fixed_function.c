      llvm_unreachable("DbgEntity must be DbgVariable or DbgLabel.");
  }

  if (Label)
    if (const auto *Sym = Label->getSymbol())
      addLabelAddress(*Die, dwarf::DW_AT_low_pc, Sym);
}

DbgEntity *DwarfCompileUnit::getExistingAbstractEntity(const DINode *Node) {
  auto &AbstractEntities = getAbstractEntities();