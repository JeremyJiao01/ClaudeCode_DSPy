                }
                if (!continueB)
                    break;
                Analyzer::Action a = valueFlowGenericForward(assignTop->astOperand2(), analyzer, settings);
                if (a.isModified())
                    break;
                tok = previousBeforeAstLeftmostLeaf(assignTop)->next();