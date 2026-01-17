            compressed_disp_scale = decode_get_compressed_disp_scale(di);
            if (compressed_disp_scale == -1)
                return false;
            needs_full_disp = disp % compressed_disp_scale != 0;
        }
        force_full_disp = !needs_full_disp && di->has_disp && disp >= INT8_MIN &&
            disp <= INT8_MAX && di->mod == 2;
        if (di->seg_override != REG_NULL) {
            *rm_opnd = opnd_create_far_base_disp_ex(
                di->seg_override, base_reg, index_reg, scale, disp,
                resolve_variable_size(di, opsize, false), encode_zero_disp,
                force_full_disp, TEST(PREFIX_ADDR, di->prefixes));
        } else {
            /* Note that OP_{jmp,call}_far_ind does NOT have a far base disp
             * operand: it is a regular base disp containing 6 bytes that
             * specify a segment selector and address.  The opcode must be
             * examined to know how to interpret those 6 bytes.
             */
            if (di->evex_encoded) {
                if (di->mod == 1)
                    disp *= compressed_disp_scale;
            }
            *rm_opnd = opnd_create_base_disp_ex(base_reg, index_reg, scale, disp,
                                                resolve_variable_size(di, opsize, false),
                                                encode_zero_disp, force_full_disp,