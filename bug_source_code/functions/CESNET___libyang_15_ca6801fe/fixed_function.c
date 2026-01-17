                    lys_node_addchild(parent, NULL, dev->orig_node);
                    if (reapply) {
                        /* augment is supposed to be applied, so fix pointers in target and the status of the original node */
                        parent->flags |= LYS_NOTAPPLIED; /* allow apply_aug() */
                        apply_aug((struct lys_node_augment *)parent, unres);
                    }
                } else {
                    /* non-augment, non-toplevel */
                    parent_path = strndup(dev->target_name, strrchr(dev->target_name, '/') - dev->target_name);
                    ret = resolve_schema_nodeid(parent_path, NULL, module, &set, 0, 1);
                    free(parent_path);
                    if (ret == -1) {
                        LOGINT;