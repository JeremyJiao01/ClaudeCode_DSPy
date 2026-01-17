  static Arity Ternary() { return Arity(3, false); }

  /// \brief A function taking a variable number of arguments
  static Arity VarArgs(int min_args = 1) { return Arity(min_args, true); }

  explicit Arity(int num_args, bool is_varargs = false)
      : num_args(num_args), is_varargs(is_varargs) {}