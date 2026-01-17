
  explicit chrono_formatter(FormatContext& ctx, OutputIt o,
                            std::chrono::duration<Rep, Period> d)
      : context(ctx), out(o), val(d.count()) {
    if (d.count() < 0) {
      d = -d;
      *out++ = '-';
    }
    s = std::chrono::duration_cast<seconds>(d);
    ms = std::chrono::duration_cast<milliseconds>(d - s);
  }