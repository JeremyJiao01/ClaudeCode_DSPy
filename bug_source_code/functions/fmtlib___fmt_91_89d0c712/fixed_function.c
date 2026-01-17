    int num_digits = count_digits(abs_value);
    int size = num_digits, n = num_digits;
    std::string::const_iterator group = groups.cbegin();
    while (group != groups.cend() && n > *group && *group > 0 &&
           *group != max_value<char>()) {
      size += sep_size;
      n -= *group;