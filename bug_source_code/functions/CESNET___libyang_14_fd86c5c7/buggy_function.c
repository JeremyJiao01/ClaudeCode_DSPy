moveto_scnode_check(const struct lysc_node *node, const struct lysc_node *ctx_scnode, const struct lyxp_set *set,
        const char *node_name, const struct lys_module *moveto_mod)
{
    if (!moveto_mod && (!node_name || strcmp(node_name, "*"))) {
        switch (set->format) {
        case LY_VALUE_SCHEMA:
        case LY_VALUE_SCHEMA_RESOLVED: