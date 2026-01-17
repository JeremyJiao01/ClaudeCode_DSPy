      // If the path doesn't exist then continue
      return Status::OK();
    }
    if (maybe_files->size() > 0) {
      return Status::Invalid(
          "Could not write to ", options.base_dir,
          " as the directory is not empty and existing_data_behavior is to error");