  //   - Second, walk loop forest to propagate registers crossing back-edges
  //   (add iterative values into the liveness set).
  void Compute() {
    cfg_.ForEachBlockInPostOrder(&*function_->begin(), [this](BasicBlock* bb) {
      ComputePartialLiveness(bb);
    });
    DoLoopLivenessUnification();
    EvaluateRegisterRequirements();
  }