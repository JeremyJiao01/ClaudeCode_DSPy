  align_spec as = spec;
  if (spec.align() == ALIGN_NUMERIC) {
    if (sign) {
      *reserve(1) = sign;
      sign = 0;
      if (as.width_)
        --as.width_;