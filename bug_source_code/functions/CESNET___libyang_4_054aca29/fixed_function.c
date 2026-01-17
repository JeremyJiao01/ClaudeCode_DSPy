                name = ((struct lys_node *)elem)->name;
            }

            if (!((struct lys_node *)elem)->parent || lys_node_module((struct lys_node *)elem) != top_smodule) {
                prefix = lys_node_module((struct lys_node *)elem)->name;
            } else {
                prefix = NULL;