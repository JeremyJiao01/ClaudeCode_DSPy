  }

  template <typename Char> int get_id(basic_string_view<Char> name) const {
    if (!has_named_args()) return {};
    const auto& named_args =
        (is_packed() ? values_[-1] : args_[-1].value_).named_args;
    for (size_t i = 0; i < named_args.size; ++i) {