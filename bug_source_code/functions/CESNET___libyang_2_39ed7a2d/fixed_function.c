            LOGVAL_ERRITEM(ctx, err);
            ly_err_free(err);
        } else {
            LOGVAL(ctx, LYVE_OTHER, "Storing value failed.");
        }
        return ret;
    }