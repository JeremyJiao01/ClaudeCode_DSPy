                    continue;

                tok2 = tok2->next();
                while (tok2 && tok2->isCast() && tok2->valueType() &&
                       (tok2->valueType()->pointer ||
                        (tok2->valueType()->typeSize(*mSettings) == 0) ||
                        (tok2->valueType()->typeSize(*mSettings) >= mSettings->sizeof_pointer)))