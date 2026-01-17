			evcb_evfinalize = ev->ev_evcallback.evcb_cb_union.evcb_evfinalize;
			EVUTIL_ASSERT((evcb->evcb_flags & EVLIST_FINALIZING));
			EVBASE_RELEASE_LOCK(base, th_base_lock);
			evcb_evfinalize(ev, ev->ev_arg);
			event_debug_note_teardown_(ev);
			if (evcb_closure == EV_CLOSURE_EVENT_FINALIZE_FREE)
				mm_free(ev);
		}