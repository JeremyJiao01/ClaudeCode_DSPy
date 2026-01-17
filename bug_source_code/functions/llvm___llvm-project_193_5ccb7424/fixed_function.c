
    // Finally merge both checks and cast to pointer type. The inttoptr
    // implicitly zexts the i1 to intptr type.
    return B.CreateIntToPtr(B.CreateLogicalAnd(Bounds, Bits, "memchr"),
                            CI->getType());
  }
