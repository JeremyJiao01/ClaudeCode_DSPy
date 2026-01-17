static const TargetRegisterClass *getDstRC(const TargetRegisterClass *SrcRC,
                                           RegDomain Domain) {
  assert(Domain == MaskDomain && "add domain");
  if (SrcRC == &X86::GR8RegClass)
    return &X86::VK8RegClass;
  if (SrcRC == &X86::GR16RegClass)
    return &X86::VK16RegClass;
  if (SrcRC == &X86::GR32RegClass)
    return &X86::VK32RegClass;
  if (SrcRC == &X86::GR64RegClass)
    return &X86::VK64RegClass;
  llvm_unreachable("add register class");
  return nullptr;