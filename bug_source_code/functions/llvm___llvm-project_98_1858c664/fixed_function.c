    }
  }

  // Verify that each inlinable callsite of a debug-info-bearing function in a
  // debug-info-bearing function has a debug location attached to it. Failure to
  // do so causes assertion failures when the inliner sets up inline scope info.