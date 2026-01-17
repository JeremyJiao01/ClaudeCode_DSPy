  }
  // "Objective-C Class Properties" is recently added for Objective-C. We
  // upgrade ObjC bitcodes to contain a "Objective-C Class Properties" module
  // flag of value 0, so we can correclty downgrade this flag when trying to
  // link an ObjC bitcode without this module flag with an ObjC bitcode with
  // this module flag.
  if (HasObjCFlag && !HasClassProperties) {
    M.addModuleFlag(llvm::Module::Override, "Objective-C Class Properties",
                    (uint32_t)0);
    return true;
  }