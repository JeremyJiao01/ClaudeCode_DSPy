    if (op || !(options & LYXP_SCHEMA)) {
        /* general root that can access everything */
        return LYXP_NODE_ROOT;
    } else if (!ctx_node || (ctx_node->schema->flags & LYS_CONFIG_W)) {
        /* root context node can access only config data (because we said so, it is unspecified) */
        return LYXP_NODE_ROOT_CONFIG;
    }