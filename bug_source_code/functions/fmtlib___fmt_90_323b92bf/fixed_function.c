      specs_.precision_, specs_.precision_ref, ctx);
    typedef output_range<typename FormatContext::iterator,
                         typename FormatContext::char_type> range_type;
    return visit(arg_formatter<range_type>(ctx, specs_),
                 internal::make_arg<FormatContext>(val));
  }

 private:
  internal::dynamic_format_specs<Char> specs_;