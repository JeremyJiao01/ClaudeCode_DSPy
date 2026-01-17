                sameline(cmdtok->next, cmdtok->next->next) &&
                cmdtok->next->op == '#' &&
                cmdtok->next->next->str == "error") {
                const std::string &ifcfg = cfg(configs_if, userDefines);
                if (!ifcfg.empty()) {
                    if (!elseError.empty())
                        elseError += ';';
                    elseError += ifcfg;
                }