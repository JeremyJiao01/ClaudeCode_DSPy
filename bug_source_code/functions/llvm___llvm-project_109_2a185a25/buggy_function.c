    MachineOperand &MO = MI->getOperand(i);
    if (!MO.isReg())
      continue;
    if (MO.isImplicit())
      break;
    if (MO.isUse())
      continue;
    for (int rx : regIndices(MO.getReg())) {