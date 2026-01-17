    return nullptr;
  }

  return std::unique_ptr<BlobFetcher>(new BlobFetcher(version, ReadOptions()));
}

std::unique_ptr<PrefetchBufferCollection>