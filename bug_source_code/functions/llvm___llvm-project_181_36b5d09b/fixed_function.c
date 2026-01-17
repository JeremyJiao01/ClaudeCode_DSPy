  const VariableGEPIndex &Var0 = VarIndices[0], &Var1 = VarIndices[1];

  if (Var0.ZExtBits != Var1.ZExtBits || Var0.SExtBits != Var1.SExtBits ||
      Var0.Scale != -Var1.Scale || Var0.V->getType() != Var1.V->getType())
    return false;

  // We'll strip off the Extensions of Var0 and Var1 and do another round