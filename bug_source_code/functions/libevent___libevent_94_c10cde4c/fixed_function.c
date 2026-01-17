			continue;
#endif

		if (what & EPOLLERR) {
			ev = EV_READ | EV_WRITE;
		} else if ((what & EPOLLHUP) && !(what & EPOLLRDHUP)) {
			ev = EV_READ | EV_WRITE;