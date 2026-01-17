            line = tok->location.line;
        }

        if (tok->previous && line == tok->location.line)
            ret << ' ';
        bool newline = false;
        while (tok->location.line > line) {