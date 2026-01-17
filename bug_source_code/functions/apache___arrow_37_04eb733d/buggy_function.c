  if (TakeState::Get(ctx).boundscheck) {
    KERNEL_RETURN_IF_ERROR(ctx, CheckIndexBounds(*batch[1].array(), batch[0].length()));
  }
  out->value = std::make_shared<NullArray>(batch.length)->data();
}

void NullFilter(KernelContext* ctx, const ExecBatch& batch, Datum* out) {