
      if (adjustment != 0) {
        for (uint32_t i = 2; i < new_operands.size(); i++) {
          uint32_t operand = inst->GetSingleWordInOperand(i);
          if (operand >= op0_length && operand != undef_literal) {
            new_operands[i].words[0] -= adjustment;
          }