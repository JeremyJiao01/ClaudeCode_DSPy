    // locations.
    ;
  } else {
    // Assert that this is the entry block, or an EH pad. If it isn't, then
    // there is some code construct we don't recognise that deals with physregs
    // across blocks.
    assert(!State.first.isVirtual());
    assert(&*InsertBB.getParent()->begin() == &InsertBB || InsertBB.isEHPad());
  }

  // Create DBG_PHI for specified physreg.