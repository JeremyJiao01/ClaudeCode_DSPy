  // points to a member of the type identifier %md. Group calls by (type ID,
  // offset) pair (effectively the identity of the virtual function) and store
  // to CallSlots.
  DenseSet<CallBase *> SeenCallSites;
  for (auto I = TypeTestFunc->use_begin(), E = TypeTestFunc->use_end();
       I != E;) {
    auto CI = dyn_cast<CallInst>(I->getUser());
    ++I;
    if (!CI)
      continue;

    // Search for virtual calls based on %p and add them to DevirtCalls.
    SmallVector<DevirtCallSite, 1> DevirtCalls;
    SmallVector<CallInst *, 1> Assumes;
    auto &DT = LookupDomTree(*CI->getFunction());
    findDevirtualizableCallsForTypeTest(DevirtCalls, Assumes, CI, DT);

    // If we found any, add them to CallSlots.
    if (!Assumes.empty()) {
      Metadata *TypeId =
          cast<MetadataAsValue>(CI->getArgOperand(1))->getMetadata();
      Value *Ptr = CI->getArgOperand(0)->stripPointerCasts();
      for (DevirtCallSite Call : DevirtCalls) {
        // Only add this CallSite if we haven't seen it before. The vtable
        // pointer may have been CSE'd with pointers from other call sites,
        // and we don't want to process call sites multiple times. We can't
        // just skip the vtable Ptr if it has been seen before, however, since
        // it may be shared by type tests that dominate different calls.
        if (SeenCallSites.insert(&Call.CB).second)
          CallSlots[{TypeId, Call.Offset}].addCallSite(Ptr, Call.CB, nullptr);
      }
    }

    // We no longer need the assumes or the type test.