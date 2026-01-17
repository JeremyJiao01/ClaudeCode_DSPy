    } else {
        switch (format) {
        case LY_VALUE_SCHEMA:
        case LY_VALUE_SCHEMA_RESOLVED:
            /* use context node module, handles augments */
            return ctx_node->module;
        case LY_VALUE_CANON:
        case LY_VALUE_JSON:
        case LY_VALUE_LYB: