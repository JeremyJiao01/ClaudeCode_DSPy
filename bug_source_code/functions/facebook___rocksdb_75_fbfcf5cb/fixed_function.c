    return nullptr;
  }

  ReadOptions read_options;
  read_options.fill_cache = false;

  return std::unique_ptr<BlobFetcher>(new BlobFetcher(version, read_options));