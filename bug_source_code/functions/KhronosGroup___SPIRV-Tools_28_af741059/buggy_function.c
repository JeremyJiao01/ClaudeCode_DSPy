}

Optimizer& Optimizer::RegisterVulkanToWebGPUPasses() {
  return RegisterPass(CreateStripDebugInfoPass())
      .RegisterPass(CreateStripAtomicCounterMemoryPass())
      .RegisterPass(CreateGenerateWebGPUInitializersPass())
      .RegisterPass(CreateLegalizeVectorShufflePass())
      .RegisterPass(CreateSplitInvalidUnreachablePass())