  align_spec as = spec;
  if (spec.align() == ALIGN_NUMERIC) {
    if (sign) {
      auto &&it = reserve(1);
      *it++ = sign;
      sign = 0;
      if (as.width_)