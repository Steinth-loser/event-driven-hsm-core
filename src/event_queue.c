/*
 * event_queue.c
 *
 *  Created on: Feb 28, 2026
 *      Author: th3ka
 */
#include "event_queue.h"

void Queue_Init(EventQueue *q)
{
	q->count = 0;
	q->head  = 0;
	q->tail  = 0;
}

bool Queue_Push(EventQueue *q, const Event *e)
{
	bool isSuccesful = false;

	QF_CRIT_STAT_TYPE crit_stat;

	QF_CRIT_ENTRY(crit_stat);

	if(q->count < QUEUE_SIZE) {
		q->buffer[q->head] = *e;
		q->head = (q->head + 1) & (QUEUE_SIZE - 1);
		q->count++;
		isSuccesful = true;
	}

	QF_CRIT_EXIT(crit_stat);

	return isSuccesful;
}

bool Queue_Pop(EventQueue *q, Event *e)
{
	bool has_event = false;

	QF_CRIT_STAT_TYPE crit_stat;

	QF_CRIT_ENTRY(crit_stat);

	if(q->count > 0) {
		*e = q->buffer[q->tail];
		q->tail = (q->tail + 1) & (QUEUE_SIZE - 1);
		q->count--;
		has_event = true;
	}

	QF_CRIT_EXIT(crit_stat);

	return has_event;
}
