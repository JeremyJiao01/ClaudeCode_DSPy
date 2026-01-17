
                const auto typeMinValue = (typeTok->valueType()->sign == ValueType::Sign::SIGNED) ? (-(1LL << (bits-1))) : 0;
                const auto unsignedTypeMaxValue = (1LL << bits) - 1LL;
                const auto typeMaxValue = (typeTok->valueType()->sign != ValueType::Sign::SIGNED || bits >= mSettings->int_bit) ?
                                          unsignedTypeMaxValue : // unsigned type. signed int/long/long long; comparing sign bit is ok. i.e. 'i == 0xffffffff'
                                          (unsignedTypeMaxValue / 2); // signed char/short
