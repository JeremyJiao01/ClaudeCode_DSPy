            ++i;
    }

    // no exact match so just return first function found
    if (!matches.empty()) {
        return matches[0];
    }

    return nullptr;
}