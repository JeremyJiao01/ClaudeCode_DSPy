                continue;
            const Token *bodyStart = top->link()->next();
            const Token *errorToken1 = checkLoopBodyRecursive(bodyStart, var, alloc, membervar, bailout);
            if (errorToken1)
                return errorToken1;
            if (bailout)
                return nullptr;
        }