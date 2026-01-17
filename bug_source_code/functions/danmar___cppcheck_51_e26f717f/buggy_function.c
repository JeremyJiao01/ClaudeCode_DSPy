    }

    int argNr = 1;
    for (const Token *arg = tokFirstArg; arg; arg = arg->nextArgument()) {
        if (mTokenizer->isCPP() && arg->str() == "new") {
            arg = arg->next();
            if (Token::simpleMatch(arg, "( std :: nothrow )"))