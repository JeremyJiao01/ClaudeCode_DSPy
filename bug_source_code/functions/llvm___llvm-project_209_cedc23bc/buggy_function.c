  createFlow();
  insertConditions(false);
  insertConditions(true);
  simplifyConditions();
  setPhiValues();
  simplifyAffectedPhis();
  rebuildSSA();
