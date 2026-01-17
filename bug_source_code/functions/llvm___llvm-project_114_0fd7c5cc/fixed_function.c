  MCSection *InstMap = nullptr;
  MCSection *FnSledIndex = nullptr;
  if (MF->getSubtarget().getTargetTriple().isOSBinFormatELF()) {
    auto Associated = dyn_cast<MCSymbolELF>(CurrentFnSym);
    assert(Associated != nullptr);
    auto Flags = ELF::SHF_WRITE | ELF::SHF_ALLOC | ELF::SHF_LINK_ORDER;
    std::string GroupName;