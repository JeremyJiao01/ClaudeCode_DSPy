        }
        // check for section close
        if(item.name == "--") {
            if(configurable_) {
                _process_callbacks();
                _process_requirements();
                run_callback();