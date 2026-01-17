  void write(long double value) { write_double(value, format_specs()); }

  /** Writes a character to the buffer. */
  void write(char value) { *reserve(1) = value; }
  void write(wchar_t value) {
    static_assert(std::is_same<char_type, wchar_t>::value, "");
    *reserve(1) = value;
  }

  /**