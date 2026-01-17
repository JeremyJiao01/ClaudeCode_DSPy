static const TargetRegisterClass *getDstRC(const TargetRegisterClass *SrcRC,
                                           RegDomain Domain) {
  assert(Domain == MaskDomain && "add domain");
  if (X86::GR8RegClass.hasSubClassEq(SrcRC))
    return &X86::VK8RegClass;
  if (X86::GR16RegClass.hasSubClassEq(SrcRC))
    return &X86::VK16RegClass;
  if (X86::GR32RegClass.hasSubClassEq(SrcRC))
    return &X86::VK32RegClass;
  if (X86::GR64RegClass.hasSubClassEq(SrcRC))
    return &X86::VK64RegClass;
  llvm_unreachable("add register class");
  return nullptr;