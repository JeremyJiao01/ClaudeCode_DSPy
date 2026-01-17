                        DAG.getDataLayout().getAllocaAddrSpace()),
                    PtrValueVTs);

    SDValue RetPtr = DAG.getCopyFromReg(DAG.getEntryNode(), getCurSDLoc(),
                                        DemoteReg, PtrValueVTs[0]);
    SDValue RetOp = getValue(I.getOperand(0));

    SmallVector<EVT, 4> ValueVTs, MemVTs;