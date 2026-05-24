#include <conops/util/cmd_queue.h>

#include <string.h>

static void default_queue_lock(void *lock)
{
	(void)lock;
	return;
}

static void default_queue_unlock(void *lock)
{
	(void)lock;
	return;
}

static void swap(struct conops_cmd *a, struct conops_cmd *b)
{
	struct conops_cmd temp;
	(void)memcpy(&temp, a, sizeof(*a));
	(void)memcpy(a, b, sizeof(*a));
	(void)memcpy(b, &temp, sizeof(*b));
}

static void bubble_up(struct conops_cmd *buffer, uint16_t index)
{
	while (index > 0U) {
		uint16_t parent_idx = (index - 1U) >> 1U;

		if (buffer[parent_idx].timestamp <= buffer[index].timestamp)
			break;

		swap(&buffer[parent_idx], &buffer[index]);

		index = parent_idx;
	}
}

static void bubble_down(struct conops_cmd *buffer, uint16_t size, uint16_t index)
{
	while (true) {
		uint16_t left = 2U * index + 1U;
		uint16_t right = 2U * index + 2U;
		uint16_t smallest = index;

		if ((left < size) && (buffer[left].timestamp < buffer[smallest].timestamp))
			smallest = left;

		if ((right < size) && (buffer[right].timestamp < buffer[smallest].timestamp))
			smallest = right;

		if (smallest == index)
			break;

		swap(&buffer[index], &buffer[smallest]);

		index = smallest;
	}
}

void heapify_cmd_buffer(struct conops_cmd *buffer, uint16_t size)
{
	for (int32_t i = (int32_t)((size >> 1U) - 1U); i >= 0; i--) {
		bubble_down(buffer, size, (uint16_t)i);
	}
}

void cmd_queue_init(struct conops_cmd_queue *queue, void *lock, cmd_queue_lock_t queue_lock,
		    cmd_queue_unlock_t queue_unlock)
{
	if ((lock == NULL) || (queue_lock == NULL) || (queue_unlock == NULL)) {
		queue->queue_lock = default_queue_lock;
		queue->queue_unlock = default_queue_unlock;
		queue->lock = NULL;
	} else {
		queue->queue_lock = queue_lock;
		queue->queue_unlock = queue_unlock;
		queue->lock = lock;
	}
	queue->size = 0;
}

int cmd_queue_enqueue(struct conops_cmd_queue *queue, const struct conops_cmd *cmd)
{
	int err = -1;

	if (queue->size < CMD_QUEUE_CAPACITY) {
		queue->queue_lock(queue->lock);

		(void)memcpy(&queue->buffer[queue->size], cmd, sizeof(*cmd));

		queue->size++;

		bubble_up(queue->buffer, queue->size - 1U);

		queue->queue_unlock(queue->lock);

		err = 0;
	}

	return err;
}

int cmd_queue_dequeue(struct conops_cmd_queue *queue, uint32_t current_time, struct conops_cmd *out)
{
	int retval = 0;

	if ((queue->size != 0U) && (queue->buffer[0].timestamp <= current_time)) {
		queue->queue_lock(queue->lock);

		(void)memcpy(out, &queue->buffer[0], sizeof(*out));

		queue->size--;

		if (queue->size > 0U) {
			(void)memcpy(&queue->buffer[0], &queue->buffer[queue->size],
				     sizeof(queue->buffer[0]));
			bubble_down(queue->buffer, queue->size, 0);
		}

		queue->queue_unlock(queue->lock);

		retval = 1;
	}

	return retval;
}

void cmd_queue_reset(struct conops_cmd_queue *queue)
{
    queue->queue_lock(queue->lock);
	queue->size = 0U;
    queue->queue_unlock(queue->lock);
}

bool cmd_queue_is_empty(const struct conops_cmd_queue *queue)
{
	return (queue->size == 0U);
}
