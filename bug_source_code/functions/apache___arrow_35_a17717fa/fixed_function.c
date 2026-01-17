    }
    length = fields[i]->length();
  }

  // For certain types like dictionaries, types may not be fully
  // determined before we have flushed. Make sure that the RecordBatch
  // gets the correct types in schema.