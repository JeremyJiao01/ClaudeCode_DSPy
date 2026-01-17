        }
        // check for section close
        if(item.name == "--") {
            if(configurable_ && parse_complete_callback_) {
                _process_callbacks();
                _process_requirements();
                run_callback();