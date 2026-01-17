  if (TakeState::Get(ctx).boundscheck) {
    KERNEL_RETURN_IF_ERROR(ctx, CheckIndexBounds(*batch[1].array(), batch[0].length()));
  }
  // batch.length doesn't take into account the take indices
  auto new_length = batch[1].array()->length;
  out->value = std::make_shared<NullArray>(new_length)->data();
}