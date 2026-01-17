                        DAG.getDataLayout().getAllocaAddrSpace()),
                    PtrValueVTs);

    SDValue RetPtr =
        DAG.getCopyFromReg(Chain, getCurSDLoc(), DemoteReg, PtrValueVTs[0]);
    SDValue RetOp = getValue(I.getOperand(0));

    SmallVector<EVT, 4> ValueVTs, MemVTs;