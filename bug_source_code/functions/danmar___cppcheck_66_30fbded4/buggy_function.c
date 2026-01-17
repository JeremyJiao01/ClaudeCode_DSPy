                }
                if (!continueB)
                    break;
                valueFlowGenericForward(assignTop->astOperand2(), analyzer, settings);
                tok = previousBeforeAstLeftmostLeaf(assignTop)->next();
                continue;
            }