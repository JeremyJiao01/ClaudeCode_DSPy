    Token::assignProgressValues(list.front());

    list.createAst();
    // needed for #7208 (garbage code) and #7724 (ast max depth limit)
    list.validateAst();

    // Create symbol database and then remove const keywords
    createSymbolDatabase();