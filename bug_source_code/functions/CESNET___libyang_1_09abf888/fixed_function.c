    /* print node name */
    ly_print_(pctx->out, "%*s<%s", INDENT, node->name.name);

    if (node->name.prefix || node->name.module_ns) {
        /* print default namespace */
        xml_print_ns_opaq(pctx, node->format, &node->name, LYXML_PREFIX_DEFAULT);
    }
