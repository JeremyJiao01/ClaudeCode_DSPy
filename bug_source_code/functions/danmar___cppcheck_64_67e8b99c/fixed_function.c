            const Token *parent = tok;
            while (parent && !Token::simpleMatch(parent->astParent(), "="))
                parent = parent->astParent();
            if (!parent || parent == parent->astParent()->astOperand1())
                continue;
        }
