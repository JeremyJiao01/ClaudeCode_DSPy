            } else if (Token::simpleMatch(var->declEndToken(), "=")) {
                errorPath.emplace_back(var->declEndToken(), "Assigned to reference.");
                const Token *vartok = var->declEndToken()->astOperand2();
                const bool temporary = isTemporary(true, vartok, nullptr, true);
                const bool nonlocal = var->isStatic() || var->isGlobal();
                if (vartok == tok || (nonlocal && temporary) || (!escape && (var->isConst() || var->isRValueReference()) && temporary))
                    return {{tok, true, std::move(errorPath)}};