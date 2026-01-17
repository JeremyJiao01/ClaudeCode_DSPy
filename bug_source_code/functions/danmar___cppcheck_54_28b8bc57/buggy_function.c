    const Token* tok = _start;
    while (tok && tok->previous() && tok->previous()->isName())
        tok = tok->previous();
    for (const Token* const end = _name?_name:_end; tok != end;) {
        if (tok->str() == "static")
            setFlag(fIsStatic, true);
        else if (tok->str() == "extern")