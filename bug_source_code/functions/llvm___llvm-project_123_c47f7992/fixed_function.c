  if (Flags.isByVal()) {
    unsigned Bytes = Flags.getByValSize();
    if (Bytes == 0) Bytes = 1; // Don't create zero-sized stack objects.

    // FIXME: For now, all byval parameter objects are marked as aliasing. This
    // can be improved with deeper analysis.
    int FI = MFI.CreateFixedObject(Bytes, VA.getLocMemOffset(), isImmutable,