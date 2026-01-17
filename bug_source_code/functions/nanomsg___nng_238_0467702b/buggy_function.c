	}

	if (name == NULL) {
		return (ENOPROTOOPT);
	}

	if ((rv = nng_setopt((nng_socket) s, name, valp, sz)) != 0) {