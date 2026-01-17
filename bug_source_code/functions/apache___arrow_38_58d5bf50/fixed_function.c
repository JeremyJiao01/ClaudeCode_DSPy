  static Arity Ternary() { return Arity(3, false); }

  /// \brief A function taking a variable number of arguments
  ///
  /// \param[in] min_args the minimum number of arguments required when
  /// invoking the function
  static Arity VarArgs(int min_args = 0) { return Arity(min_args, true); }