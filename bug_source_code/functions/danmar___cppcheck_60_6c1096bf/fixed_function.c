            line = tok->location.line;
        }

        if (tok->previous && line >= tok->location.line) // #7912
            ret << ' ';
        bool newline = false;
        while (tok->location.line > line) {