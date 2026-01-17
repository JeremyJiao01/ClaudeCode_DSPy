    /* print node name */
    ly_print_(pctx->out, "%*s<%s", INDENT, node->name.name);

    /* print default namespace */
    xml_print_ns_opaq(pctx, node->format, &node->name, LYXML_PREFIX_DEFAULT);

    /* print attributes */
    LY_CHECK_RET(xml_print_attr(pctx, node));