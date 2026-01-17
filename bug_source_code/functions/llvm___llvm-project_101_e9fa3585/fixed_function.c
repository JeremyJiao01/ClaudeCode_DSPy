  // size. If so, check to see whether we will end up actually reducing the
  // number of stores used.
  unsigned Bytes = unsigned(End-Start);
  unsigned MaxIntSize = DL.getLargestLegalIntTypeSize() / 8;
  if (MaxIntSize == 0)
    MaxIntSize = 1;
  unsigned NumPointerStores = Bytes / MaxIntSize;