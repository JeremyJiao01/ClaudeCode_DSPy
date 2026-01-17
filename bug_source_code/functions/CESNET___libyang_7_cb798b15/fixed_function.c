            }

            assert(scnode);
            if ((axis != LYXP_AXIS_CHILD) && !lysc_data_parent(scnode)) {
                /* iterating over top-level nodes, find next */
                while (lysc_data_parent(*iter)) {
                    *iter = lysc_data_parent(*iter);