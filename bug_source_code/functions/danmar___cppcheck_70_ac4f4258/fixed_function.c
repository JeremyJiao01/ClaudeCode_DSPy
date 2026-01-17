            if (!values.empty()) {
                cond.vartok = tok->astOperand2();
            } else {
                values = getIteratorValues(tok->astOperand2()->values(), &kind);
                if (!values.empty())
                    cond.vartok = tok->astOperand1();
            }