          section_arg);
    }
    // Ignore error as table factory deserialization is optional
    cf_opt->table_factory.reset();
    s = TableFactory::CreateFromString(
        config_options,
        section_title.substr(
            opt_section_titles[kOptionSectionTableOptions].size()),
        &(cf_opt->table_factory));
    if (s.ok() && cf_opt->table_factory != nullptr) {
      s = cf_opt->table_factory->ConfigureFromMap(config_options, opt_map);
      // Translate any errors (NotFound, NotSupported, to InvalidArgument