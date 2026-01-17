
      if (adjustment != 0) {
        for (uint32_t i = 2; i < new_operands.size(); i++) {
          if (inst->GetSingleWordInOperand(i) >= op0_length) {
            new_operands[i].words[0] -= adjustment;
          }
        }