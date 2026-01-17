          section_arg);
    }
    // Ignore error as table factory deserialization is optional
    s = TableFactory::CreateFromString(
        config_options,
        section_title.substr(
            opt_section_titles[kOptionSectionTableOptions].size()),
        &(cf_opt->table_factory));
    if (s.ok()) {
      s = cf_opt->table_factory->ConfigureFromMap(config_options, opt_map);
      // Translate any errors (NotFound, NotSupported, to InvalidArgument
      if (s.ok() || s.IsInvalidArgument()) {