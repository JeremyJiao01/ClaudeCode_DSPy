template <typename Char>
inline typename basic_context<Char>::format_arg
  basic_context<Char>::get_arg(basic_string_view<Char> name) {
  map_.init(this->args());
  if (const format_arg *arg = map_.find(name))
    return *arg;
  this->on_error("argument not found");
  return format_arg();
}

/** Formats arguments and writes the output to the buffer. */
template <typename ArgFormatter, typename Char, typename Context>