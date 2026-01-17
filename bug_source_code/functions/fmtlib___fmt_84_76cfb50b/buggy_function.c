                              make_checked(p, s.size()));
    }
    if (prefix_size != 0) p[-1] = static_cast<Char>('-');
    out = write(out, basic_string_view<Char>(buffer.data(), buffer.size()),
                specs);
  }

  void on_chr() { *out++ = static_cast<Char>(abs_value); }