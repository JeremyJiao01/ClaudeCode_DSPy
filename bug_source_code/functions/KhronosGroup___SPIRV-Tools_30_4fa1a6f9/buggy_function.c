            // fold.
            return false;
          }
        } else {
          if (new_feeder_id == 0) {
            // First time through, save the id of the operand the element comes
            // from.
            new_feeder_id = feeding_shuffle_inst->GetSingleWordInOperand(1);
          } else if (new_feeder_id !=
                     feeding_shuffle_inst->GetSingleWordInOperand(1)) {
            // We need both elements of the feeding_shuffle_inst, so we cannot
            // fold.
            return false;
          }
          component_index -= feeder_op0_length;
        }

        if (!feeder_is_op0) {
          component_index += op0_length;
        }
      }