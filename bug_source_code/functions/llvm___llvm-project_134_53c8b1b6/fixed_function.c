  SDValue Op1 = Op.getOperand(1);
  SDValue Amt = Op.getOperand(2);

  // Expand slow SHLD/SHRD cases if we are not optimizing for size.
  bool OptForSize = DAG.getMachineFunction().getFunction().optForSize();
  if (!OptForSize && Subtarget.isSHLDSlow())
    return SDValue();

  bool IsFSHR = Op.getOpcode() == ISD::FSHR;