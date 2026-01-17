    }

    // process uninstantiated templates
    // TODO: remove the specialized check and handle all uninstantiated templates someday.
    if (mTemplateInstantiations.empty() && specialized) {
        simplifyCalculations();

        Token * tok2 = const_cast<Token *>(tok->tokAt(namepos));