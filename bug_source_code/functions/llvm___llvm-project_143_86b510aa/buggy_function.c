
bool ShrinkWrap::useOrDefCSROrFI(const MachineInstr &MI,
                                 RegScavenger *RS) const {
  if (MI.getOpcode() == FrameSetupOpcode ||
      MI.getOpcode() == FrameDestroyOpcode) {
    LLVM_DEBUG(dbgs() << "Frame instruction: " << MI << '\n');