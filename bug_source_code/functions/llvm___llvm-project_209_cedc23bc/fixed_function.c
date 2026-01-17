  createFlow();
  insertConditions(false);
  insertConditions(true);
  setPhiValues();
  simplifyConditions();
  simplifyAffectedPhis();
  rebuildSSA();
