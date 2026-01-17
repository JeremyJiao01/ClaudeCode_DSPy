                compileUnaryOp(tok, state, compileExpression);
            else
                compileBinOp(tok, state, compileExpression);
            if (Token::simpleMatch(tok, "}"))
                tok = tok->next();
        } else break;
    }