      ReturnTy = TypeArray[0];
  }

  if (auto *ReturnDCTy = dyn_cast_or_null<DICompositeType>(ReturnTy)) {
    if (isNonTrivial(ReturnDCTy))
      FO |= FunctionOptions::CxxReturnUdt;
  }

  // DISubroutineType is unnamed. Use DISubprogram's i.e. SPName in comparison.
  if (ClassTy && isNonTrivial(ClassTy) && SPName == ClassTy->getName()) {