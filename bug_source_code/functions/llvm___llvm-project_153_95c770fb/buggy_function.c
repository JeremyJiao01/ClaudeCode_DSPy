    return getFrameIndexReference(MF, FI, FrameReg);

  FrameReg = TRI->getStackRegister();
  return alignTo(MFI.getMaxCallFrameSize(), getStackAlignment()) + it->second;
}

int X86FrameLowering::getFrameIndexReferenceSP(const MachineFunction &MF,