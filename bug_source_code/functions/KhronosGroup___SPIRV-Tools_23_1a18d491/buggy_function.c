    } else {
      assert(spvOpcodeIsComposite(element_inst->opcode()));
      element_inst = context()->get_def_use_mgr()->GetDef(
          element_inst->GetSingleWordInOperand(1u));
    }
  }
