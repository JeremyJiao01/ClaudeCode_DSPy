      // field_index <-> path nesting relation
      segments[i] = name + "=" + hive_options_.null_fallback;
    } else {
      segments[i] = name + "=" + arrow::internal::UriEscape(values[i]->ToString());
    }
  }
