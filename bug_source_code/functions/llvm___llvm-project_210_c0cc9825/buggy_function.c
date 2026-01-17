      }

  MemoryUseOrDef *MA = MSSA->getMemoryAccess(M);
  MemoryAccess *AnyClobber = MSSA->getWalker()->getClobberingMemoryAccess(MA);
  MemoryLocation DestLoc = MemoryLocation::getForDest(M);
  const MemoryAccess *DestClobber =
      MSSA->getWalker()->getClobberingMemoryAccess(AnyClobber, DestLoc);