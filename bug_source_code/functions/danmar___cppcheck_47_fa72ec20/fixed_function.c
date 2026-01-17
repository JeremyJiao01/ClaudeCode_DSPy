                continue;
            const Token *bodyStart = top->link()->next();
            const Token *errorToken1 = checkLoopBodyRecursive(bodyStart, var, alloc, membervar, bailout);
            if (!errorToken)
                errorToken = errorToken1;
            if (bailout)
                return nullptr;
        }