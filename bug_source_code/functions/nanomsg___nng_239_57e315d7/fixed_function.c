		// better use nng_recvmsg() instead.
		void *nbuf;

		if (nng_msg_len(msg) != 0) {
			if ((nbuf = nni_alloc(nng_msg_len(msg))) == NULL) {
				nng_msg_free(msg);
				return (NNG_ENOMEM);
			}

			*(void **) buf = nbuf;
			memcpy(nbuf, nni_msg_body(msg), nni_msg_len(msg));
			*szp = nng_msg_len(msg);
		} else {
			*(void **)buf = NULL;