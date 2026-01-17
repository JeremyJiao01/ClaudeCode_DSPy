  // Check whether the analysis, all analyses on functions, or the function's
  // CFG have been preserved.
  auto PAC = PA.getChecker<llvm::LazyCallGraphAnalysis>();
  return !(PAC.preserved() || PAC.preservedSet<AllAnalysesOn<Module>>());
}

LazyCallGraph &LazyCallGraph::operator=(LazyCallGraph &&G) {
  BPA = std::move(G.BPA);