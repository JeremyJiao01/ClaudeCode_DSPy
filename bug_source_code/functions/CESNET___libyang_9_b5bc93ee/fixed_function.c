    /* the dflt member is just filled to avoid getting the default value from the type */
    leaf->dflt = (void*)leaf_p->dflt;
    ret = lys_compile_node_type(ctx, node_p, &leaf_p->type, leaf);
    if (ret) {
        leaf->dflt = NULL;
        return ret;
    }
