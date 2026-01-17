                sameline(cmdtok->next, cmdtok->next->next) &&
                cmdtok->next->op == '#' &&
                cmdtok->next->next->str == "error") {
                if (!elseError.empty())
                    elseError += ';';
                elseError += cfg(configs_if, userDefines);
            }
            if (!configs_if.empty())
                configs_if.pop_back();