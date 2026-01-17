
  const Value *Object = GetUnderlyingObject(Loc.Ptr, DL);

  // Calls marked 'tail' cannot read or write allocas from the current frame
  // because the current frame might be destroyed by the time they run. However,
  // a tail call may use an alloca with byval. Calling with byval copies the
  // contents of the alloca into argument registers or stack slots, so there is
  // no lifetime issue.
  if (isa<AllocaInst>(Object))
    if (const CallInst *CI = dyn_cast<CallInst>(CS.getInstruction()))
      if (CI->isTailCall() &&
          !CI->getAttributes().hasAttrSomewhere(Attribute::ByVal))
        return ModRefInfo::NoModRef;
