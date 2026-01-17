        return (*this)(static_cast<int>(value));
      fmt_specs.sign = sign::none;
      fmt_specs.alt = false;
      fmt_specs.align = align::right;
      return base::operator()(value);
    } else {
      return base::operator()(value);