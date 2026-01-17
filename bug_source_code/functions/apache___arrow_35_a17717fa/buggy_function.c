    }
    length = fields[i]->length();
  }
  *batch = RecordBatch::Make(schema_, length, std::move(fields));
  if (reset_builders) {
    return InitBuilders();
  } else {