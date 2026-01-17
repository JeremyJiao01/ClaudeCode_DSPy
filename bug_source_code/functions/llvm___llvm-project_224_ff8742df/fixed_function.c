
  bool isConditionImplied(SmallVector<int64_t, 8> R) const;

  ArrayRef<int64_t> getLastConstraint() { return Constraints.back(); }
  void popLastConstraint() { Constraints.pop_back(); }
  void popLastNVariables(unsigned N) {
    for (auto &C : Constraints) {