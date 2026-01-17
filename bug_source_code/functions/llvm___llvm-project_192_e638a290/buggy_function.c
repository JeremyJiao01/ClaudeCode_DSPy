
  // Record[16] is the address space number.

  // Check whether we have enough values to read a partition name.
  if (Record.size() > 18)
    Func->setPartition(StringRef(Strtab.data() + Record[17], Record[18]));

  ValueList.push_back(Func);
