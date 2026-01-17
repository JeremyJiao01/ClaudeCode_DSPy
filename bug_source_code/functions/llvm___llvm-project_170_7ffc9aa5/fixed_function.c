
        Sqrt = getSqrtCall(Base, Pow->getCalledFunction()->getAttributes(),
                           Pow->doesNotAccessMemory(), M, B, TLI);
        if (!Sqrt)
          return nullptr;
      }