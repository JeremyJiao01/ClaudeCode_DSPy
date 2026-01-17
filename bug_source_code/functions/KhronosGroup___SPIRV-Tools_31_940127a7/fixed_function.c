        case SpvOpConstant:
          return MatchOpConstant(src_inst, dst_inst, flexibility);
        case SpvOpConstantComposite:
        case SpvOpSpecConstantComposite:
          // Composite constants must match in type and value.
          //
          // TODO: match OpConstantNull with OpConstantComposite with all zeros
          // at flexibility == 1
          // TODO: match constants from structs that have been flexibly-matched.
          if (src_inst->NumInOperandWords() != dst_inst->NumInOperandWords()) {
            return false;
          }
          return DoesOperandMatch(src_inst->GetOperand(0),
                                  dst_inst->GetOperand(0)) &&
                 DoOperandsMatch(src_inst, dst_inst, 0,
                                 src_inst->NumInOperandWords());
        case SpvOpConstantSampler:
          // Match sampler constants exactly.
          // TODO: Allow flexibility in parameters to better diff shaders where
          // the sampler param has changed.
          assert(src_inst->NumInOperandWords() ==
                 dst_inst->NumInOperandWords());
          return DoOperandsMatch(src_inst, dst_inst, 0,
                                 src_inst->NumInOperandWords());
        case SpvOpConstantNull:
          // Match null constants as long as the type matches.
          return DoesOperandMatch(src_inst->GetOperand(0),
                                  dst_inst->GetOperand(0));

        case SpvOpSpecConstantTrue:
        case SpvOpSpecConstantFalse:
        case SpvOpSpecConstant:
        case SpvOpSpecConstantOp:
          // Match spec constants by name if available, then by the SpecId
          // decoration.