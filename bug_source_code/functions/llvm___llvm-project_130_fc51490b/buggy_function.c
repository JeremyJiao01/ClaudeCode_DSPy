        continue;

      // The  low half of the 128-bit result must choose from A.
      // The high half of the 128-bit result must choose from B.
      unsigned NumEltsPer64BitChunk = NumEltsPer128BitChunk / 2;
      unsigned Src = i >= NumEltsPer64BitChunk;

      // Check that successive elements are being operated on. If not, this is
      // not a horizontal operation.