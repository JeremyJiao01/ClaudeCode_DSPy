template <typename Char>
inline typename basic_context<Char>::format_arg
  basic_context<Char>::get_arg(basic_string_view<Char> name) {
  if (this->check_no_auto_index()) {
    map_.init(this->args());
    if (const format_arg *arg = map_.find(name))
      return *arg;
    this->on_error("argument not found");
  }
  return format_arg();
}
