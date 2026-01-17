    SeekForPrev(*iterate_upper_bound_);
    const bool is_ikey = (timestamp_size_ > 0 && timestamp_lb_ != nullptr);
    Slice k = Valid() ? key() : Slice();
    if (is_ikey) {
      k.remove_suffix(kNumInternalBytes + timestamp_size_);
    }
    while (Valid() && 0 == user_comparator_.CompareWithoutTimestamp(