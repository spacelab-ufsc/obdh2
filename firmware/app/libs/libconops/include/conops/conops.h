#ifndef CONOPS_H_
#define CONOPS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Library ERRNO meanings */
#define ERRNO_CONOPS_INVALID_ARG 1
#define ERRNO_CONOPS_NO_TRANSITION_TABLE 2
#define ERRNO_CONOPS_INSUFFICIENT_BUFFER_SIZE 3
#define ERRNO_CONOPS_INVALID_STATE 4
#define ERRNO_CONOPS_INVALID_EV_ID 5
#define ERRNO_CONOPS_NO_MAPPER_FUNCTION 6
#define ERRNO_CONOPS_MAPPING_ERROR 7
#define ERRNO_CONOPS_INVALID_TRANSITION 8

/* Library FSM default config */
#define CONOPS_DEFAULT_N_STATES 256U
#define CONOPS_DEFAULT_MAX_EV_ID UINT16_MAX
#define CONOPS_DEFAULT_INIT_STATE 0U
#define CONOPS_DEFAULT_EV_NAME_SIZE 12U
#define CONOPS_DEFAULT_CALLBACK_IGNORE_SRC 0x5443

/* Foward declarations */
struct conops_fsm;
struct conops_event;

typedef int (*conops_transition_handler_t)(struct conops_fsm *ctx, const struct conops_event *ev,
					   const uint16_t transition_to);

typedef int32_t (*conops_event_mapper_t)(const struct conops_fsm *ctx,
					 const struct conops_event *ev);

typedef void (*conops_event_callback_t)(struct conops_fsm *ctx, const struct conops_event *ev);

typedef int (*conops_init_callback_t)(struct conops_fsm *ctx);

struct conops_fsm_config {
	uint16_t n_states;
	uint16_t max_ev_id;
	uint16_t init_state;
};

struct conops_fsm {
	struct conops_fsm_config conf;
	uint16_t state;
	uint16_t last_valid_ev_id;
	const void *transition_table;
	void *user_data;
	conops_event_mapper_t ev_mapper_fn;
};

struct conops_event {
	uint16_t src;
	uint16_t ev_id;
	char ev_name[CONOPS_DEFAULT_EV_NAME_SIZE];
	conops_event_callback_t callback;
};

int conops_fsm_init(struct conops_fsm *fsm, const void *transition_table, const uint16_t n_states,
		    const uint16_t max_ev_id, const uint16_t init_state,
		    conops_init_callback_t callback);

int conops_fsm_process_event(struct conops_fsm *fsm, const struct conops_event *ev);

int conops_register_mapper(struct conops_fsm *fsm, conops_event_mapper_t mapper);

int conops_register_fsm_user_data(struct conops_fsm *fsm, void *user_data);

#ifdef __cplusplus
}
#endif

#endif
