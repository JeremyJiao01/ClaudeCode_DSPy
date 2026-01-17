      specs_.precision_, specs_.precision_ref, ctx);
    typedef output_range<typename FormatContext::iterator,
                         typename FormatContext::char_type> range_type;
    visit(arg_formatter<range_type>(ctx, specs_),
          internal::make_arg<FormatContext>(val));
    return ctx.out();
  }

 private: