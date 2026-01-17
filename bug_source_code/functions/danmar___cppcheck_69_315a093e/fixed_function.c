                continue;

            if (!func.isExplicit() &&
                func.minArgCount() == 1 &&
                func.type != Function::eCopyConstructor &&
                func.type != Function::eMoveConstructor) {
                noExplicitConstructorError(func.tokenDef, scope->className, scope->type == Scope::eStruct);