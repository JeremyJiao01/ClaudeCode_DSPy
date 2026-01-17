      upper = &upper_store;
    }
    denominator.assign_pow10(exp10);
    denominator <<= 1;
  } else if (exp10 < 0) {
    numerator.assign_pow10(-exp10);
    lower.assign(numerator);