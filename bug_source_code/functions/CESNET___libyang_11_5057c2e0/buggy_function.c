
    /* remember current number of parents */
    parents_count = xmlctx->elements.count;

    /* skip after the content */
    while (xmlctx->status != LYXML_ELEM_CONTENT) {
        LY_CHECK_RET(lyxml_ctx_next(xmlctx));
    }
    LY_CHECK_RET(lyxml_ctx_next(xmlctx));

    /* skip all children elements, recursively, if any */
    while (parents_count < xmlctx->elements.count) {
        LY_CHECK_RET(lyxml_ctx_next(xmlctx));
    }
