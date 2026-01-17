
  explicit chrono_formatter(FormatContext& ctx, OutputIt o,
                            std::chrono::duration<Rep, Period> d)
      : context(ctx),
        out(o),
        val(d.count()),
        s(std::chrono::duration_cast<seconds>(d)),
        ms(std::chrono::duration_cast<milliseconds>(d - s)) {}

  int hour() const { return to_int(mod((s.count() / 3600), 24)); }
