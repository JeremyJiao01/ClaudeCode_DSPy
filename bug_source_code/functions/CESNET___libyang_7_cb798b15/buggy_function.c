            }

            assert(scnode);
            if (!lysc_data_parent(scnode)) {
                /* iterating over top-level nodes, find next */
                while (lysc_data_parent(*iter)) {
                    *iter = lysc_data_parent(*iter);