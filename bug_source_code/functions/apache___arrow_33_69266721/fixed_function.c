                                                  /*dest_offset=*/position, run.length,
                                                  transpose_map));
          } else {
            std::fill(out_data + (position * index_width),
                      out_data + (position + run.length) * index_width, 0x00);
          }

          position += run.length;