#include <conops/conops.h>
#include <string.h>

int conops_fsm_init(struct conops_fsm *fsm, const void *transition_table, const uint16_t n_states,
		    const uint16_t max_ev_id, const uint16_t init_state,
		    conops_init_callback_t callback)
{
	if (!fsm || !transition_table)
		return -ERRNO_CONOPS_INVALID_ARG;

	if (init_state >= n_states)
		return -ERRNO_CONOPS_INVALID_STATE;

	fsm->ev_mapper_fn = NULL;
	fsm->user_data = NULL;
	fsm->transition_table = transition_table;
	fsm->conf.n_states = n_states;
	fsm->conf.max_ev_id = (max_ev_id != 0) ? max_ev_id : CONOPS_DEFAULT_MAX_EV_ID;
	fsm->conf.init_state = init_state;
	fsm->state = init_state;

	if (callback) {
		return callback(fsm);
	}

	return 0;
}

int conops_fsm_process_event(struct conops_fsm *fsm, const struct conops_event *ev)
{
	int err = 0;

	if (!fsm || !ev)
		return -ERRNO_CONOPS_INVALID_ARG;

	if (!fsm->transition_table)
		return -ERRNO_CONOPS_NO_TRANSITION_TABLE;

	if (ev->ev_id > fsm->conf.max_ev_id)
		return -ERRNO_CONOPS_INVALID_EV_ID;

	if (!fsm->ev_mapper_fn)
		return -ERRNO_CONOPS_NO_MAPPER_FUNCTION;

	int32_t transition_to = fsm->ev_mapper_fn(fsm, ev);

	if (transition_to < 0)
		return -ERRNO_CONOPS_MAPPING_ERROR;

	const conops_transition_handler_t(*table)[fsm->conf.n_states] =
		(const conops_transition_handler_t(*)[fsm->conf.n_states])fsm->transition_table;

	conops_transition_handler_t handler = table[fsm->state][transition_to];

	if (transition_to == fsm->state)
		goto ev_callback_run;

	if (handler) {
		err = handler(fsm, ev, transition_to);

		if (err < 0)
			return err;
	} else {
		return -ERRNO_CONOPS_INVALID_TRANSITION;
	}

ev_callback_run:
	if (ev->callback)
		ev->callback(fsm, ev);

	fsm->last_valid_ev_id = ev->ev_id;

	return 0;
}

int conops_register_mapper(struct conops_fsm *fsm, conops_event_mapper_t mapper)
{
	if (!fsm || !mapper)
		return -ERRNO_CONOPS_INVALID_ARG;

	fsm->ev_mapper_fn = mapper;

	return 0;
}

int conops_register_fsm_user_data(struct conops_fsm *fsm, void *user_data)
{
	if (!fsm || !user_data)
		return -ERRNO_CONOPS_INVALID_ARG;

	fsm->user_data = user_data;

	return 0;
}
