
        Sqrt = getSqrtCall(Base, Pow->getCalledFunction()->getAttributes(),
                           Pow->doesNotAccessMemory(), M, B, TLI);
      }

      // We will memoize intermediate products of the Addition Chain.