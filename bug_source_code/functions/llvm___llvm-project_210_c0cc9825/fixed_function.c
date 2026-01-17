      }

  MemoryUseOrDef *MA = MSSA->getMemoryAccess(M);
  // FIXME: Not using getClobberingMemoryAccess() here due to PR54682.
  MemoryAccess *AnyClobber = MA->getDefiningAccess();
  MemoryLocation DestLoc = MemoryLocation::getForDest(M);
  const MemoryAccess *DestClobber =