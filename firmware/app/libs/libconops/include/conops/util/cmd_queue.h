#ifndef CONOPS_CMD_QUEUE_H
#define CONOPS_CMD_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define PAYLOAD_SIZE 20U
#define CMD_QUEUE_CAPACITY 10U

typedef void (*cmd_queue_lock_t)(void *lock);
typedef void (*cmd_queue_unlock_t)(void *lock);

struct conops_cmd {
	uint32_t timestamp;
	uint8_t payload[PAYLOAD_SIZE];
};

struct conops_cmd_queue {
	struct conops_cmd buffer[CMD_QUEUE_CAPACITY];
	uint16_t size;
	void *lock;
	cmd_queue_lock_t queue_lock;
	cmd_queue_unlock_t queue_unlock;
};

void cmd_queue_init(struct conops_cmd_queue *queue, void *lock, cmd_queue_lock_t queue_lock,
		    cmd_queue_unlock_t queue_unlock);

int cmd_queue_dequeue(struct conops_cmd_queue *queue, uint32_t current_time,
		      struct conops_cmd *out);

int cmd_queue_enqueue(struct conops_cmd_queue *queue, const struct conops_cmd *cmd);

void cmd_queue_reset(struct conops_cmd_queue *queue);

bool cmd_queue_is_empty(const struct conops_cmd_queue *queue);

void heapify_cmd_buffer(struct conops_cmd *buffer, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif
