  LPM1.addPass(LoopRotatePass(Level != OptimizationLevel::Oz));
  // TODO: Investigate promotion cap for O1.
  LPM1.addPass(LICMPass(PTO.LicmMssaOptCap, PTO.LicmMssaNoAccForPromotionCap));
  LPM1.addPass(SimpleLoopUnswitchPass());
  LPM2.addPass(LoopIdiomRecognizePass());
  LPM2.addPass(IndVarSimplifyPass());
