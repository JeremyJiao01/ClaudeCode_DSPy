  if (F->hasLocalLinkage() && F->hasComdat())
    return false;
  // Check uses of this function for other than direct calls or invokes to it.
  return F->hasAddressTaken();
}

static inline bool needsComdatForCounter(Function &F, Module &M) {