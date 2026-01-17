              __func__, __LINE__, pc->orig_line, pc->text());
      // Don't check inside PAREN or SQUARE groups
      if (  pc->type == CT_SPAREN_OPEN
         || pc->type == CT_FPAREN_OPEN
         || pc->type == CT_SQUARE_OPEN
         || pc->type == CT_PAREN_OPEN)
      {