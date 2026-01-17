                }

                if(!value.empty()) {
                    if(write_description && opt->has_description()) {
                        out << '\n';
                        out << commentLead << detail::fix_newlines(commentLead, opt->get_description()) << '\n';