  // conditions 1) one-use, 2) does not produce poison, and 3) has all but one
  // guaranteed-non-poison operands (or is a BUILD_VECTOR or similar) then push
  // the freeze through to the operands that are not guaranteed non-poison.
  // NOTE: we will strip poison-generating flags, so ignore them here.
  if (DAG.canCreateUndefOrPoison(N0, /*PoisonOnly*/ false,
                                 /*ConsiderFlags*/ false) ||
      N0->getNumValues() != 1 || !N0->hasOneUse())
    return SDValue();

  bool AllowMultipleMaybePoisonOperands = N0.getOpcode() == ISD::BUILD_VECTOR;

  SmallSetVector<SDValue, 8> MaybePoisonOperands;
  for (SDValue Op : N0->ops()) {
    if (DAG.isGuaranteedNotToBeUndefOrPoison(Op, /*PoisonOnly*/ false,
                                             /*Depth*/ 1))
      continue;
    bool HadMaybePoisonOperands = !MaybePoisonOperands.empty();
    bool IsNewMaybePoisonOperand = MaybePoisonOperands.insert(Op);
    if (!HadMaybePoisonOperands)
      continue;
    if (IsNewMaybePoisonOperand && !AllowMultipleMaybePoisonOperands) {
      // Multiple maybe-poison ops when not allowed - bail out.
      return SDValue();
    }
  }
  // NOTE: the whole op may be not guaranteed to not be undef or poison because
  // it could create undef or poison due to it's poison-generating flags.
  // So not finding any maybe-poison flags is fine.

  for (SDValue MaybePoisonOperand : MaybePoisonOperands) {
    // Don't replace every single UNDEF everywhere with frozen UNDEF, though.
    if (MaybePoisonOperand.getOpcode() == ISD::UNDEF)
      continue;
    // First, freeze each offending operand.
    SDValue FrozenMaybePoisonOperand = DAG.getFreeze(MaybePoisonOperand);
    // Then, change all other uses of unfrozen operand to use frozen operand.
    DAG.ReplaceAllUsesOfValueWith(MaybePoisonOperand, FrozenMaybePoisonOperand);
    // But, that also updated the use in the freeze we just created, thus
    // creating a cycle in a DAG. Let's undo that by mutating the freeze.
    DAG.UpdateNodeOperands(FrozenMaybePoisonOperand.getNode(),
                           MaybePoisonOperand);
  }

  // Finally, recreate the node, it's operands were updated to use
  // frozen operands, so we just need to use it's "original" operands.
  SmallVector<SDValue> Ops(N0->op_begin(), N0->op_end());
  // Special-handle ISD::UNDEF, each single one of them can be it's own thing.
  for (SDValue &Op : Ops) {
    if (Op.getOpcode() == ISD::UNDEF)
      Op = DAG.getFreeze(Op);
  }
  // NOTE: this strips poison generating flags.
  SDValue R = DAG.getNode(N0.getOpcode(), SDLoc(N0), N0->getVTList(), Ops);