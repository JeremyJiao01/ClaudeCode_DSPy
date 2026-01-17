  // If we're negating an FMA node, then we can adjust the
  // instruction to include the extra negation.
  unsigned NewOpcode = 0;
  if (Arg.hasOneUse()) {
    switch (Arg.getOpcode()) {
    case ISD::FMA:             NewOpcode = X86ISD::FNMSUB;       break;
    case X86ISD::FMSUB:        NewOpcode = X86ISD::FNMADD;       break;