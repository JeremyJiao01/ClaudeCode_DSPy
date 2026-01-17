                       *predicate - start, start, src_prefix_len, src_prefix, path_context->name);
                goto cleanup;
            }
        } else {
            mod = start_node->module;
        }
        src_node = NULL;
        if (context_node->keys) {
            for (u = 0; u < LY_ARRAY_SIZE(context_node->keys); ++u) {
                if (!strncmp(src, context_node->keys[u]->name, src_len) && context_node->keys[u]->name[src_len] == '\0') {
                    src_node = (const struct lysc_node*)context_node->keys[u];
                    break;
                }
            }
        }
        if (!src_node) {
            LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                   "Invalid leafref path predicate \"%.*s\" - predicate's key node \"%.*s\" not found.",
                   *predicate - start, start, src_len, src, mod->name);
            goto cleanup;
        }

        /* check that there is only one predicate for the */
        c = keys.count;
        i = ly_set_add(&keys, (void*)src_node, 0);
        LY_CHECK_GOTO(i == -1, cleanup);
        if (keys.count == c) { /* node was already present in the set */
            LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                   "Invalid leafref path predicate \"%.*s\" - multiple equality tests for the key \"%s\".",
                   *predicate - start, start, src_node->name);
            goto cleanup;
        }

        /* destination */
        dest_parent_times = 0;
        dst_node = start_node;

        /* current-function-invocation *WSP "/" *WSP rel-path-keyexpr */
        if (strncmp(path_key_expr, "current()", 9)) {
            LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                   "Invalid leafref path predicate \"%.*s\" - missing current-function-invocation.",
                   *predicate - start, start);
            goto cleanup;
        }
        path_key_expr += 9;
        while (isspace(*path_key_expr)) {
            ++path_key_expr;
        }

        if (*path_key_expr != '/') {
            LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                   "Invalid leafref path predicate \"%.*s\" - missing \"/\" after current-function-invocation.",
                   *predicate - start, start);
            goto cleanup;
        }
        ++path_key_expr;
        while (isspace(*path_key_expr)) {
            ++path_key_expr;
        }

        /* rel-path-keyexpr:
         * 1*(".." *WSP "/" *WSP) *(node-identifier *WSP "/" *WSP) node-identifier */
        while (!strncmp(path_key_expr, "..", 2)) {
            ++dest_parent_times;
            path_key_expr += 2;
            while (isspace(*path_key_expr)) {
                ++path_key_expr;
            }
            if (*path_key_expr != '/') {
                LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                       "Invalid leafref path predicate \"%.*s\" - missing \"/\" in \"../\" rel-path-keyexpr pattern.",
                       *predicate - start, start);
                goto cleanup;
            }
            ++path_key_expr;
            while (isspace(*path_key_expr)) {
                ++path_key_expr;
            }

            /* path is supposed to be evaluated in data tree, so we have to skip
             * all schema nodes that cannot be instantiated in data tree */
            MOVE_PATH_PARENT(dst_node, !strncmp(path_key_expr, "..", 2), goto cleanup,
                             "Invalid leafref path predicate \"%.*s\" - too many \"..\" in rel-path-keyexpr.",
                             *predicate - start, start);
        }
        if (!dest_parent_times) {
            LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                   "Invalid leafref path predicate \"%.*s\" - at least one \"..\" is expected in rel-path-keyexpr.",
                   *predicate - start, start);
            goto cleanup;
        }
        if (path_key_expr == pke_end) {
            LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                   "Invalid leafref path predicate \"%.*s\" - at least one node-identifier is expected in rel-path-keyexpr.",
                   *predicate - start, start);
            goto cleanup;
        }

        while(path_key_expr != pke_end) {
            if (lys_parse_nodeid(&path_key_expr, &dst_prefix, &dst_prefix_len, &dst, &dst_len)) {
                LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_SYNTAX_YANG,
                       "Invalid node identifier in leafref path predicate - character %d (of %.*s).",
                       path_key_expr - start + 1, *predicate - start, start);
                goto cleanup;
            }

            if (dst_prefix) {
                mod = lys_module_find_prefix(path_context, dst_prefix, dst_prefix_len);
            } else {
                mod = start_node->module;
            }
            if (!mod) {
                LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                       "Invalid leafref path predicate \"%.*s\" - unable to find module of the node \"%.*s\" in rel-path_keyexpr.",
                       *predicate - start, start, dst_len, dst);
                goto cleanup;
            }

            dst_node = lys_child(dst_node, mod, dst, dst_len, 0, LYS_GETNEXT_NOSTATECHECK);
            if (!dst_node) {
                LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                       "Invalid leafref path predicate \"%.*s\" - unable to find node \"%.*s\" in the rel-path_keyexpr.",
                       *predicate - start, start, path_key_expr - pke_start, pke_start);
                goto cleanup;
            }
        }
        if (!(dst_node->nodetype & (dst_node->module->version < LYS_VERSION_1_1 ? LYS_LEAF : LYS_LEAF | LYS_LEAFLIST))) {
            LOGVAL(ctx->ctx, LY_VLOG_STR, ctx->path, LYVE_REFERENCE,
                   "Invalid leafref path predicate \"%.*s\" - rel-path_keyexpr \"%.*s\" refers %s instead of leaf.",
                   *predicate - start, start, path_key_expr - pke_start, pke_start, lys_nodetype2str(dst_node->nodetype));
            goto cleanup;
        }