
#ifndef NDEBUG
  if (status == opt::Pass::Status::SuccessWithoutChange) {
    auto changed = optimized_binary->size() != original_binary_size ||
                   memcmp(optimized_binary->data(), original_binary,
                          original_binary_size) != 0;
    assert(!changed &&
           "Binary unexpectedly changed despite optimizer saying there was no "