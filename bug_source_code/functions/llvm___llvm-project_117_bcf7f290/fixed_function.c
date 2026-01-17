                 "new fragment is outside of original fragment");
          Start -= OrigFragment->OffsetInBits;
        }

        // The alloca may be larger than the variable.
        if (VarSize) {