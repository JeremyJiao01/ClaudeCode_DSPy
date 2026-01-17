                /* adding zeros at the end */
                num_len = exponent + e_val;
                dp_position = num_len; /* decimal point is behind the actual value */
            } else if ((size_t)labs(e_val) < (exponent - minus)) {
                /* adding decimal point between the integer's digits */
                num_len = exponent + 1;
                dp_position = exponent + e_val;