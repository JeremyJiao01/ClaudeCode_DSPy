{
    auto getStrTok = [](Token* tok, bool addLength, Token** endStmt) -> Token* {
        if (addLength) {
            *endStmt = tok->tokAt(6);
            return tok->tokAt(4);
        }
        if (Token::Match(tok, "%var% [ ] =")) {