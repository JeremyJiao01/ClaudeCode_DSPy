
  /// \brief Convert any integer value into a BasicDecimal128.
  template <typename T,
            typename = typename std::enable_if<
                std::is_integral<T>::value && (sizeof(T) <= sizeof(uint64_t)), T>::type>
  constexpr BasicDecimal128(T value) noexcept
      : BasicDecimal128(value >= T{0} ? 0 : -1, static_cast<uint64_t>(value)) {  // NOLINT
  }

  /// \brief Create a BasicDecimal128 from an array of bytes. Bytes are assumed to be in