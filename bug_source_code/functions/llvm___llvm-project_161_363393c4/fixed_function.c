    Name = getSectionPrefixForGlobal(Kind);
  }

  bool HasPrefix = false;
  if (const auto *F = dyn_cast<Function>(GO)) {
    if (Optional<StringRef> Prefix = F->getSectionPrefix()) {
      Name += *Prefix;
      HasPrefix = true;
    }
  }

  if (UniqueSectionName) {
    Name.push_back('.');
    TM.getNameWithPrefix(Name, GO, Mang, /*MayAlwaysUsePrivate*/true);
  } else if (HasPrefix)