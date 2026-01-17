    } else {
        switch (format) {
        case LY_VALUE_SCHEMA:
            /* use local module */
            return ly_schema_resolve_prefix(ctx, prefix, prefix_len, prefix_data);
        case LY_VALUE_SCHEMA_RESOLVED:
            /* use local module */
            return ly_schema_resolved_resolve_prefix(ctx, prefix, prefix_len, prefix_data);
        case LY_VALUE_CANON: