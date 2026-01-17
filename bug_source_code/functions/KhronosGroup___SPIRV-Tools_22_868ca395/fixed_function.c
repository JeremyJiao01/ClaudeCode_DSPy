  //   - Second, walk loop forest to propagate registers crossing back-edges
  //   (add iterative values into the liveness set).
  void Compute() {
    for (BasicBlock& start_bb : *function_) {
      if (reg_pressure_->Get(start_bb.id()) != nullptr) {
        continue;
      }
      cfg_.ForEachBlockInPostOrder(&start_bb, [this](BasicBlock* bb) {
        if (reg_pressure_->Get(bb->id()) == nullptr) {