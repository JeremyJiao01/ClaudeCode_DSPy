            break;
        case operation::DEREF:
            if constexpr(KeyOnly) {
                static_cast<std::pair<meta_any, meta_any> *>(to)->first = std::cref(*any_cast<const It &>(from));
            } else {
                *static_cast<std::pair<meta_any, meta_any> *>(to) = std::make_pair<meta_any, meta_any>(std::cref(any_cast<const It &>(from)->first), std::reference_wrapper{any_cast<const It &>(from)->second});
            }
            break;
        }