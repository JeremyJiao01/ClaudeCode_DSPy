  SDValue Op1 = Op.getOperand(1);
  SDValue Amt = Op.getOperand(2);

  // Expand slow SHLD/SHRD cases.
  // TODO - can we be more selective here: OptSize/RMW etc.?
  if (Subtarget.isSHLDSlow())
    return SDValue();

  bool IsFSHR = Op.getOpcode() == ISD::FSHR;