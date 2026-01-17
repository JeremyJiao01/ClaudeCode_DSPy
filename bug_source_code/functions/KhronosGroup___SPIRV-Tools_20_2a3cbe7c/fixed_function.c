
#ifndef NDEBUG
  if (status == opt::Pass::Status::SuccessWithoutChange) {
    std::vector<uint32_t> optimized_binary_with_nop;
    context->module()->ToBinary(&optimized_binary_with_nop,
                                /* skip_nop = */ false);
    auto changed = optimized_binary_with_nop.size() != original_binary_size ||
                   memcmp(optimized_binary_with_nop.data(), original_binary,