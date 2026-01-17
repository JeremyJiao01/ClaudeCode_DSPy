                                    Options, ForCodeSize, Depth + 1))
      return V;

    return isNegatibleForFree(Op.getOperand(1), LegalOperations, TLI, Options,
                              ForCodeSize, Depth + 1);
