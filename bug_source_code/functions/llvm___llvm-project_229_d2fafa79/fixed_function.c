  // per-function, we are able to create an index entry that will represent the
  // range of sleds associated with a function.
  auto &Ctx = OutContext;
  MCSymbol *SledsStart =
      OutContext.createLinkerPrivateSymbol("xray_sleds_start");
  OutStreamer->switchSection(InstMap);
  OutStreamer->emitLabel(SledsStart);