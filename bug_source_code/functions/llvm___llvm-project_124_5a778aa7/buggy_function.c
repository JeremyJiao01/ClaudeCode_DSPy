
  const Value *Object = GetUnderlyingObject(Loc.Ptr, DL);

  // If this is a tail call and Loc.Ptr points to a stack location, we know that
  // the tail call cannot access or modify the local stack.
  // We cannot exclude byval arguments here; these belong to the caller of
  // the current function not to the current function, and a tail callee
  // may reference them.
  if (isa<AllocaInst>(Object))
    if (const CallInst *CI = dyn_cast<CallInst>(CS.getInstruction()))
      if (CI->isTailCall())
        return ModRefInfo::NoModRef;

  // If the pointer is to a locally allocated object that does not escape,