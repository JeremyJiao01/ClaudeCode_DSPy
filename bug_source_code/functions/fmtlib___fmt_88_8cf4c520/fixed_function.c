    for (auto end = p + s.size() - block_size + 1; p < end;) p = transcode(p);
  }
  if (auto num_chars_left = s.data() + s.size() - p) {
    char buf[2 * block_size - 1] = {};
    memcpy(buf, p, num_chars_left);
    p = buf;
    do {
      p = transcode(p);
    } while (p - buf < num_chars_left);