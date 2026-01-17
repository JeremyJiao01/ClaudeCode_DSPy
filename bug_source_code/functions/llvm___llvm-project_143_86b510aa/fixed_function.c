
bool ShrinkWrap::useOrDefCSROrFI(const MachineInstr &MI,
                                 RegScavenger *RS) const {
  // This prevents premature stack popping when occurs a indirect stack
  // access. It is overly aggressive for the moment.
  // TODO: - Obvious non-stack loads and store, such as global values,