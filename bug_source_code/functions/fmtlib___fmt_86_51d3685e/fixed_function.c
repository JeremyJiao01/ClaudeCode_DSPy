      } else {
        write(second(), 2);
        write_fractional_seconds<char_type>(
            out, std::chrono::duration<rep, Period>(val));
      }
      return;
    }