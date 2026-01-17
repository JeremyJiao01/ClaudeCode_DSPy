    // attribute logic on all calls to declarations (as declarations aren't
    // explicitly visited by CGSCC passes in the new pass manager.)
    if (F.isDeclaration() && !F.hasOptNone()) {
      Changed |= inferLibFuncAttributes(F, GetTLI(F));
      Changed |= inferAttributesFromOthers(F);
    }
