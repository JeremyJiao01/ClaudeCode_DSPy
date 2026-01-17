                    continue;

                const auto typeMinValue = (typeTok->valueType()->sign == ValueType::Sign::SIGNED) ? (-(1LL << (bits-1))) : 0;
                const auto unsignedTypeMaxValue = (1LL << (bits-1)) - 1LL;
                const auto typeMaxValue = (typeTok->valueType()->sign == ValueType::Sign::SIGNED) ? (unsignedTypeMaxValue / 2) : unsignedTypeMaxValue;

                if (valueTok->getKnownIntValue() < typeMinValue)