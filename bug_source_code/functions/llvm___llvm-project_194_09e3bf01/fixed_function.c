  // If the false value simplified to false, then the result of the compare
  // is equal to "Cond && TCmp".  This also catches the case when the false
  // value simplified to false and the true value to true, returning "Cond".
  // Folding select to and/or isn't poison-safe in general; impliesPoison
  // checks whether folding it does not convert a well-defined value into
  // poison.
  if (match(FCmp, m_Zero()) && impliesPoison(TCmp, Cond))
    if (Value *V = SimplifyAndInst(Cond, TCmp, Q, MaxRecurse))
      return V;
  // If the true value simplified to true, then the result of the compare
  // is equal to "Cond || FCmp".
  if (match(TCmp, m_One()) && impliesPoison(FCmp, Cond))