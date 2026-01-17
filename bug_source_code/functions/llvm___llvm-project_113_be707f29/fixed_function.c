           "codeview doesn't support subranges with lower bounds");
    int64_t Count = Subrange->getCount();

    // Variable length arrays and forward declarations of arrays without a size
    // use a count of -1. Emit a count (and overall size) or zero in these cases
    // to match what MSVC does for array declarations with no count.
    // FIXME: Make front-end support VLA subrange and emit LF_DIMVARLU.
    if (Count == -1)
      Count = 0;

    // Update the element size and element type index for subsequent subranges.