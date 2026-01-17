        const ScopeInfo3 *scopeInfo = memberClassScope ? memberClassScope : currentScope;

        // check in base types
        if (scopeInfo->findTypeInBase(scope))
            return true;

        // check using namespace