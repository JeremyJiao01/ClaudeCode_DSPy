        continue;

      // The  low half of the 128-bit result must choose from A.
      // The high half of the 128-bit result must choose from B,
      // unless B is undef. In that case, we are always choosing from A.
      // TODO: Using a horizontal op on a single input is likely worse for
      // performance on many CPUs, so this should be limited here or reversed
      // in a later pass.
      unsigned NumEltsPer64BitChunk = NumEltsPer128BitChunk / 2;