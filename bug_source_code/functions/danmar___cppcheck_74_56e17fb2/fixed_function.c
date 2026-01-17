                compileUnaryOp(tok, state, compileExpression);
            else
                compileBinOp(tok, state, compileExpression);
            while (Token::simpleMatch(tok, "}"))
                tok = tok->next();
        } else break;
    }