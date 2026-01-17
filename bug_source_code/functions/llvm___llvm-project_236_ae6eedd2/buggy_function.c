
    if (LAR.MSSA && !PassPA.getChecker<MemorySSAAnalysis>().preserved())
      report_fatal_error("Loop pass manager using MemorySSA contains a pass "
                         "that does not preserve MemorySSA");

#ifndef NDEBUG
    // LoopAnalysisResults should always be valid.