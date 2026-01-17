      ReturnTy = TypeArray[0];
  }

  // Add CxxReturnUdt option to functions that return nontrivial record types
  // or methods that return record types.
  if (auto *ReturnDCTy = dyn_cast_or_null<DICompositeType>(ReturnTy))
    if (isNonTrivial(ReturnDCTy) || ClassTy)
      FO |= FunctionOptions::CxxReturnUdt;

  // DISubroutineType is unnamed. Use DISubprogram's i.e. SPName in comparison.