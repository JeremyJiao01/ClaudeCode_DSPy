    }

    int argNr = 1;
    for (const Token *funcArg = tokFirstArg; funcArg; funcArg = funcArg->nextArgument()) {
        const Token* arg = funcArg;
        if (mTokenizer->isCPP() && arg->str() == "new") {
            arg = arg->next();