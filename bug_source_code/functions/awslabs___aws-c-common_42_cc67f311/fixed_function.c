    return AWS_OP_SUCCESS;
}

static inline int s_base64_get_decoded_value(unsigned char to_decode, uint8_t *value, int8_t allow_sentinal) {

    uint8_t decode_value = BASE64_DECODING_TABLE[(size_t)to_decode];
    if (decode_value != 0xDD && (decode_value != BASE64_SENTIANAL_VALUE || allow_sentinal)) {