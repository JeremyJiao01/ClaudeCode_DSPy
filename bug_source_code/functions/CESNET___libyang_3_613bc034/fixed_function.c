            node = ((struct lysc_node_choice*)target)->cases->prev;
        } else {
            /* the compiled node is the last child of the target */
            node = (struct lysc_node*)lysc_node_children(target, flags);
            if (!node) {
                /* there is no data children (compiled nodes is e.g. notification or action or nothing) */
                break;