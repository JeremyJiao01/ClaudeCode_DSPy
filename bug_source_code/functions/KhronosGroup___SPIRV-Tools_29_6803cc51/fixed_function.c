    return NonSemanticShaderDebugInfo100InstructionsMax;
  }

  uint32_t opcode = GetSingleWordInOperand(kExtInstInstructionInIdx);
  if (opcode >= NonSemanticShaderDebugInfo100InstructionsMax) {
    return NonSemanticShaderDebugInfo100InstructionsMax;
  }
