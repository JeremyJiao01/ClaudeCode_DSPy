                if (arg1 && arg1->hasKnownValue()) {
                    const ValueFlow::Value &value = arg1->values().back();
                    if (value.isTokValue() && value.tokvalue->tokType() == Token::eString)
                        sizeValue = Token::getStrLength(value.tokvalue);
                }
                break;
            };