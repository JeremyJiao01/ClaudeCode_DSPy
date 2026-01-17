    return NonSemanticShaderDebugInfo100InstructionsMax;
  }

  return NonSemanticShaderDebugInfo100Instructions(
      GetSingleWordInOperand(kExtInstInstructionInIdx));
}

CommonDebugInfoInstructions Instruction::GetCommonDebugOpcode() const {