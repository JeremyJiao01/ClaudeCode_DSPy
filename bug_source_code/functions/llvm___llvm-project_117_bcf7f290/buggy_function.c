                 "new fragment is outside of original fragment");
          Start -= OrigFragment->OffsetInBits;
        }
        // Avoid creating a fragment expression that covers the entire variable.
        if (!VarSize || *VarSize != Size) {
          if (auto E =