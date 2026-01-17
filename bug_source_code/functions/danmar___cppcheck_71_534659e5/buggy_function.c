                    case ValueType::Type::UNKNOWN_TYPE:
                    case ValueType::Type::NONSTD:
                    case ValueType::Type::RECORD:
                        check = tok->valueType()->typeScope && tok->valueType()->typeScope->getDestructor();
                        break;
                    case ValueType::Type::CONTAINER:
                    case ValueType::Type::ITERATOR: