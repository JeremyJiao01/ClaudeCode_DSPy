
    // Finally merge both checks and cast to pointer type. The inttoptr
    // implicitly zexts the i1 to intptr type.
    return B.CreateIntToPtr(B.CreateAnd(Bounds, Bits, "memchr"), CI->getType());
  }

  // Check if all arguments are constants.  If so, we can constant fold.