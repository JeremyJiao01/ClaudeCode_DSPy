    }

    // strip the program name
    vals.second = (esp != std::string::npos) ? commandline.substr(esp + 1) : std::string{};
    ltrim(vals.second);
    return vals;
}