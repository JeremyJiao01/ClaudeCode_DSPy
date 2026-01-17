      LOG_FMT(LSPLIT, " ** ONCE LINER SPLIT **\n");
      undo_one_liner(start);
      newlines_cleanup_braces(false);
      // Issue #1352
      cpd.changes++;
      return(false);