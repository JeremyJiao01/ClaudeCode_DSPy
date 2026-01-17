
    const std::string::size_type pos = sourcefile.rfind('/');
    std::string filename;
    if (pos != std::string::npos)
        filename = sourcefile;
    else
        filename = sourcefile.substr(pos + 1);