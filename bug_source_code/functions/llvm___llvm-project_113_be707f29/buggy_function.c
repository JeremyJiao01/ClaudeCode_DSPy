           "codeview doesn't support subranges with lower bounds");
    int64_t Count = Subrange->getCount();

    // Variable Length Array (VLA) has Count equal to '-1'.
    // Replace with Count '1', assume it is the minimum VLA length.
    // FIXME: Make front-end support VLA subrange and emit LF_DIMVARLU.
    if (Count == -1)
      Count = 1;

    // Update the element size and element type index for subsequent subranges.
    ElementSize *= Count;