    Value *A = Op0->getOperand(0), *B = Op0->getOperand(1), *C = RHS;
    Instruction::BinaryOps InnerOpcode = Op0->getOpcode(); // op'

    // Disable the use of undef because it's not safe to distribute undef.
    auto SQDistributive = SQ.getWithInstruction(&I).getWithoutUndef();
    Value *L = SimplifyBinOp(TopLevelOpcode, A, C, SQDistributive);
    Value *R = SimplifyBinOp(TopLevelOpcode, B, C, SQDistributive);

    // Do "A op C" and "B op C" both simplify?
    if (L && R) {
      // They do! Return "L op' R".
      ++NumExpand;
      C = Builder.CreateBinOp(InnerOpcode, L, R);
      C->takeName(&I);
      return C;
    }

    // Does "A op C" simplify to the identity value for the inner opcode?
    if (L && L == ConstantExpr::getBinOpIdentity(InnerOpcode, L->getType())) {
      // They do! Return "B op C".
      ++NumExpand;
      C = Builder.CreateBinOp(TopLevelOpcode, B, C);
      C->takeName(&I);
      return C;
    }

    // Does "B op C" simplify to the identity value for the inner opcode?
    if (R && R == ConstantExpr::getBinOpIdentity(InnerOpcode, R->getType())) {
      // They do! Return "A op C".
      ++NumExpand;
      C = Builder.CreateBinOp(TopLevelOpcode, A, C);
      C->takeName(&I);
      return C;
    }
  }

  if (Op1 && leftDistributesOverRight(TopLevelOpcode, Op1->getOpcode())) {
    // The instruction has the form "A op (B op' C)".  See if expanding it out
    // to "(A op B) op' (A op C)" results in simplifications.
    Value *A = LHS, *B = Op1->getOperand(0), *C = Op1->getOperand(1);
    Instruction::BinaryOps InnerOpcode = Op1->getOpcode(); // op'

    // Disable the use of undef because it's not safe to distribute undef.
    auto SQDistributive = SQ.getWithInstruction(&I).getWithoutUndef();
    Value *L = SimplifyBinOp(TopLevelOpcode, A, B, SQDistributive);