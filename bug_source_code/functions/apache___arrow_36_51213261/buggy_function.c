      // field_index <-> path nesting relation
      segments[i] = name + "=" + hive_options_.null_fallback;
    } else {
      segments[i] = name + "=" + values[i]->ToString();
    }
  }
