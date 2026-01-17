}

Optimizer& Optimizer::RegisterVulkanToWebGPUPasses() {
  return RegisterPass(CreateStripAtomicCounterMemoryPass())
      .RegisterPass(CreateGenerateWebGPUInitializersPass())
      .RegisterPass(CreateLegalizeVectorShufflePass())
      .RegisterPass(CreateSplitInvalidUnreachablePass())
      .RegisterPass(CreateEliminateDeadConstantPass())