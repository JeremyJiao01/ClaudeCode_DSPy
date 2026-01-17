            node = ((struct lysc_node_choice*)target)->cases->prev;
        } else {
            /* the compiled node is the last child of the target */
            node = lysc_node_children(target, flags)->prev;
        }

        if (!allow_mandatory && (node->flags & LYS_CONFIG_W) && (node->flags & LYS_MAND_TRUE)) {