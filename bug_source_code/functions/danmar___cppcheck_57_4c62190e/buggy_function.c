    Token::assignProgressValues(list.front());

    list.createAst();
    // skipping this here may help improve performance. Might be enabled later on demand. #7208
    // list.validateAst();

    // Create symbol database and then remove const keywords
    createSymbolDatabase();