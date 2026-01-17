        return true;
    }

    if (match(U, m_IDiv(m_Value(), m_Specific(V))) &&
        isValidAssumeForContext(cast<Instruction>(U), CtxI, DT))
      return true;
