
    void on_oct() {
      int num_digits = internal::count_digits<3>(abs_value);
      if (specs.alt && specs.precision <= num_digits) {
        // Octal prefix '0' is counted as a digit, so only add it if precision
        // is not greater than the number of digits.
        prefix[prefix_size++] = '0';