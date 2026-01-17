      StDisp2 += OverlapDelta;
      Size2 -= OverlapDelta;
    }
    Size1 = std::abs(std::abs(LdDisp2) - std::abs(LdDisp1));

    // Build a copy for the point until the current blocking store's
    // displacement.