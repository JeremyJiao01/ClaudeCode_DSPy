    Name = getSectionPrefixForGlobal(Kind);
  }

  if (const auto *F = dyn_cast<Function>(GO)) {
    if (Optional<StringRef> Prefix = F->getSectionPrefix())
      Name += *Prefix;
  }

  if (UniqueSectionName) {
    Name.push_back('.');
    TM.getNameWithPrefix(Name, GO, Mang, /*MayAlwaysUsePrivate*/true);
  }
  return Name;
}
