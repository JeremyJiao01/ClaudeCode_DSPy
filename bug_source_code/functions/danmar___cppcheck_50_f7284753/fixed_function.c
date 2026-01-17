    }

    // process uninstantiated templates
    const std::list<TokenAndName>::iterator it = std::find_if(mTemplateInstantiations.begin(),
            mTemplateInstantiations.end(),
            FindName(templateDeclaration.name));

    // TODO: remove the specialized check and handle all uninstantiated templates someday.