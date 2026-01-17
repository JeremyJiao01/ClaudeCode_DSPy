                }

                if(!value.empty()) {
                    if(!opt->get_fnames().empty()) {
                        value = opt->get_flag_value(name, value);
                    }