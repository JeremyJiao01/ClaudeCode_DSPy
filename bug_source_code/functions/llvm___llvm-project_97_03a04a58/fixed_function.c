      continue;
    }

    uint64_t PermuteOp = (M >> 5) & 0x7;
    if (PermuteOp == 4) {
      ShuffleMask.push_back(SM_SentinelZero);
      continue;