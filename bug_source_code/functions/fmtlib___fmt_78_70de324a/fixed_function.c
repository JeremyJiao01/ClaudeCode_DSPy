  FMT_CONSTEXPR void on_sign(sign_t s) {
    require_numeric_argument();
    if (is_integral_type(arg_type_) && arg_type_ != type::int_type &&
        arg_type_ != type::long_long_type && arg_type_ != type::int128_type &&
        arg_type_ != type::char_type) {
      this->on_error("format specifier requires signed argument");
    }