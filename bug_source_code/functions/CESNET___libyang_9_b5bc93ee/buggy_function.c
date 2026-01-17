    /* the dflt member is just filled to avoid getting the default value from the type */
    leaf->dflt = (void*)leaf_p->dflt;
    ret = lys_compile_node_type(ctx, node_p, &leaf_p->type, leaf);
    leaf->dflt = NULL;
    LY_CHECK_RET(ret);

    if (leaf_p->dflt) {
        struct ly_err_item *err = NULL;