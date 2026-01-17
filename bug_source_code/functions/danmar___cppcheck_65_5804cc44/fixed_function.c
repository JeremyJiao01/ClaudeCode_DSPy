            ++indentlevel;
        else if (indentlevel > 0 && Token::Match(tok3, "> ,|>|::"))
            --indentlevel;
        else if (indentlevel == 0 && Token::Match(tok3->previous(), "[<,]")) {
            mTypesUsedInTemplateInstantiation.emplace_back(tok3, "");
        }
        if (Token::Match(tok3, "(|["))