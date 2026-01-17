        LY_CHECK_ARG_RET(ctx, format == LYD_XML, parent, parent->schema->nodetype & (LYS_RPC | LYS_ACTION), tree, !op,
                LY_EINVAL);
    }
    parse_opts = LYD_PARSE_ONLY | LYD_PARSE_OPAQ;
    val_opts = 0;

    /* parse the data */