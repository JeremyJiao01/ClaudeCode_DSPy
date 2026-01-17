        bool suppressed = false;
        for (const Suppressions::Suppression &s2 : unmatched) {
            if (s2.errorId == "unmatchedSuppression") {
                if ((s2.fileName == "*" || s2.fileName == s.fileName) &&
                    (s2.lineNumber == Suppressions::Suppression::NO_LINE || s2.lineNumber == s.lineNumber)) {
                    suppressed = true;
                    break;