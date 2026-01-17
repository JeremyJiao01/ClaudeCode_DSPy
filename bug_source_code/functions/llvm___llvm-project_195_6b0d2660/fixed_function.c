  // When ClearDSOLocalOnDeclarations is true, clear dso_local if GV is
  // converted to a declaration, to disable direct access. Don't do this if GV
  // is implicitly dso_local due to a non-default visibility.
  if (ClearDSOLocalOnDeclarations &&
      (GV.isDeclarationForLinker() ||
       (isPerformingImport() && !doImportAsDefinition(&GV))) &&
      !GV.isImplicitDSOLocal()) {