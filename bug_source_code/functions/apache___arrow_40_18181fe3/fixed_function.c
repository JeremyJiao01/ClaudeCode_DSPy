
  if (opts.nans_equal()) {
    return BaseFloatingEquals<ArrowType>(left, right, [epsilon](T x, T y) -> bool {
      return (fabs(x - y) <= epsilon) || (x == y) || (std::isnan(x) && std::isnan(y));
    });
  } else {
    return BaseFloatingEquals<ArrowType>(left, right, [epsilon](T x, T y) -> bool {
      return (fabs(x - y) <= epsilon) || (x == y);
    });
  }
}