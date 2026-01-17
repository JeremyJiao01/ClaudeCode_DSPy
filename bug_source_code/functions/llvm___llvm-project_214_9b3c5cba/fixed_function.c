  bool SkipOutermostLoad = !isa<DbgValueInst>(DVI);
  Value *Storage = DVI->getVariableLocationOp(0);
  Value *OriginalStorage = Storage;

  while (auto *Inst = dyn_cast_or_null<Instruction>(Storage)) {
    if (auto *LdInst = dyn_cast<LoadInst>(Inst)) {
      Storage = LdInst->getOperand(0);
      // FIXME: This is a heuristic that works around the fact that
      // LLVM IR debug intrinsics cannot yet distinguish between
      // memory and value locations: Because a dbg.declare(alloca) is
      // implicitly a memory location no DW_OP_deref operation for the
      // last direct load from an alloca is necessary.  This condition
      // effectively drops the *last* DW_OP_deref in the expression.
      if (!SkipOutermostLoad)
        Expr = DIExpression::prepend(Expr, DIExpression::DerefBefore);
    } else if (auto *StInst = dyn_cast<StoreInst>(Inst)) {
      Storage = StInst->getOperand(0);
    } else {
      SmallVector<uint64_t, 16> Ops;
      SmallVector<Value *, 0> AdditionalValues;
      Value *Op = llvm::salvageDebugInfoImpl(
          *Inst, Expr ? Expr->getNumLocationOperands() : 0, Ops,
          AdditionalValues);
      if (!Op || !AdditionalValues.empty()) {
        // If salvaging failed or salvaging produced more than one location
        // operand, give up.
        break;
      }
      Storage = Op;
      Expr = DIExpression::appendOpsToArg(Expr, Ops, 0, /*StackValue*/ false);
    }
    SkipOutermostLoad = false;
  }
  if (!Storage)
    return;

  // Store a pointer to the coroutine frame object in an alloca so it
  // is available throughout the function when producing unoptimized
  // code. Extending the lifetime this way is correct because the
  // variable has been declared by a dbg.declare intrinsic.
  //
  // Avoid to create the alloca would be eliminated by optimization
  // passes and the corresponding dbg.declares would be invalid.
  if (!OptimizeFrame)
    if (auto *Arg = dyn_cast<llvm::Argument>(Storage)) {
      auto &Cached = DbgPtrAllocaCache[Storage];
      if (!Cached) {
        Cached = Builder.CreateAlloca(Storage->getType(), 0, nullptr,
                                      Arg->getName() + ".debug");
        Builder.CreateStore(Storage, Cached);
      }
      Storage = Cached;
      // FIXME: LLVM lacks nuanced semantics to differentiate between
      // memory and direct locations at the IR level. The backend will
      // turn a dbg.declare(alloca, ..., DIExpression()) into a memory
      // location. Thus, if there are deref and offset operations in the
      // expression, we need to add a DW_OP_deref at the *start* of the
      // expression to first load the contents of the alloca before
      // adjusting it with the expression.
      Expr = DIExpression::prepend(Expr, DIExpression::DerefBefore);
    }

  DVI->replaceVariableLocationOp(OriginalStorage, Storage);