
// Check if the name needs quotes to be safe for the linker to interpret.
static bool canBeUnquotedInDirective(char C) {
  return isAlnum(C) || C == '_' || C == '$' || C == '.' || C == '@';
}

static bool canBeUnquotedInDirective(StringRef Name) {