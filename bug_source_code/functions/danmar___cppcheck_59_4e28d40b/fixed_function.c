            ++i;
    }

    // no exact match, but only one candidate left
    if (matches.size() == 1)
        return matches[0];

    return nullptr;
}
