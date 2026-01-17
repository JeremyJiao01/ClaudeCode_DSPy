              Twine(dwarf::AttributeEncodingString(Btr.Encoding) +
                    "_" + Twine(Btr.BitSize)).toStringRef(Str));
    addUInt(Die, dwarf::DW_AT_encoding, dwarf::DW_FORM_data1, Btr.Encoding);
    addUInt(Die, dwarf::DW_AT_byte_size, None, Btr.BitSize / 8);

    Btr.Die = &Die;
  }