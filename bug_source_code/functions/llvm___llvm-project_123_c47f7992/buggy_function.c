  if (Flags.isByVal()) {
    unsigned Bytes = Flags.getByValSize();
    if (Bytes == 0) Bytes = 1; // Don't create zero-sized stack objects.
    int FI = MFI.CreateFixedObject(Bytes, VA.getLocMemOffset(), isImmutable);
    // Adjust SP offset of interrupt parameter.
    if (CallConv == CallingConv::X86_INTR) {
      MFI.setObjectOffset(FI, Offset);