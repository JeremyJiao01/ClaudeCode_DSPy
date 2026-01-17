            if (!(constIfWhileExpression || constValExpr || compExpr || returnStatement))
                continue;

            if (returnStatement && (!scope->function || !Token::simpleMatch(scope->function->retDef, "bool")))
                continue;

            if (returnStatement && isConstVarExpression(tok))