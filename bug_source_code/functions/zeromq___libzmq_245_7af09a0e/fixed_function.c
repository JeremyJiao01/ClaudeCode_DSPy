    protocol_flags = 0;
    if (in_progress ()->flags () & msg_t::more)
        protocol_flags |= v2_protocol_t::more_flag;
    if (in_progress ()->flags () & msg_t::command
        || in_progress ()->is_subscribe () || in_progress ()->is_cancel ()) {
        protocol_flags |= v2_protocol_t::command_flag;
        if (in_progress ()->is_subscribe ())
            size += zmq::msg_t::sub_cmd_name_size;
        else if (in_progress ()->is_cancel ())
            size += zmq::msg_t::cancel_cmd_name_size;
    }
    // Calculate large_flag after command_flag. Subscribe or cancel commands
    // increase the message size.
    if (size > UCHAR_MAX)
        protocol_flags |= v2_protocol_t::large_flag;
