    const Token* tok = _start;
    while (tok && tok->previous() && tok->previous()->isName())
        tok = tok->previous();
    const Token* end = _end;
    if (end)
        end = end->next();
    while (tok != end) {