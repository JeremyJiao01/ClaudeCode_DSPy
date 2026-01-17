    protocol_flags = 0;
    if (in_progress ()->flags () & msg_t::more)
        protocol_flags |= v2_protocol_t::more_flag;
    if (in_progress ()->size () > UCHAR_MAX)
        protocol_flags |= v2_protocol_t::large_flag;
    if (in_progress ()->flags () & msg_t::command
        || in_progress ()->is_subscribe () || in_progress ()->is_cancel ()) {
        protocol_flags |= v2_protocol_t::command_flag;
        if (in_progress ()->is_subscribe ())
            size += zmq::msg_t::sub_cmd_name_size;
        else if (in_progress ()->is_cancel ())
            size += zmq::msg_t::cancel_cmd_name_size;
    }

    //  Encode the message length. For messages less then 256 bytes,
    //  the length is encoded as 8-bit unsigned integer. For larger