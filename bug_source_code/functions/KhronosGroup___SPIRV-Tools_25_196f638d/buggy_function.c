  if (folded_inst != nullptr) {
    // We do not want to change the body of the function by adding new
    // instructions.  When folding we can only generate new constants.
    assert(folded_inst->IsConstant() &&
           "CCP is only interested in constant values.");
    uint32_t new_val = ComputeLatticeMeet(instr, folded_inst->result_id());
    values_[instr->result_id()] = new_val;