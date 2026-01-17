    return getFrameIndexReference(MF, FI, FrameReg);

  FrameReg = TRI->getStackRegister();
  return alignDown(MFI.getMaxCallFrameSize(), getStackAlignment()) + it->second;
}

int X86FrameLowering::getFrameIndexReferenceSP(const MachineFunction &MF,