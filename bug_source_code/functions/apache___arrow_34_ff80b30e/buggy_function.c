      ARROW_RETURN_IF(*it < 0, Status::Invalid("Negative field index"));
      include_indices_list.push_back(*it);
    }
    opts->includeTypes(include_indices_list);
    return Status::OK();
  }
