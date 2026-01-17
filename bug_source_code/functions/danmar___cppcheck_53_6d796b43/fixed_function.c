                    if (!condTok->hasKnownIntValue()) {
                        if (!analyzer->lowerToPossible())
                            return Progress::Break;
                    } else if (condTok->values().front().intvalue == inElse) {
                        return Progress::Break;
                    }
                    analyzer->assume(condTok, !inElse, tok);