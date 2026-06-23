/*
 * event_queue.h
 *
 *  Created on: Feb 27, 2026
 *      Author: th3ka
 */

#ifndef INC_EVENT_QUEUE_H_
#define INC_EVENT_QUEUE_H_

#include "statemachine.h"
#include <stdbool.h>
#include "stm32f4xx_hal.h"
///////////CRITICAL SECTION DEFINITION///////////////

#define QF_CRIT_STAT_TYPE uint32_t



#define QF_CRIT_ENTRY(x_)  do { (x_) = __get_PRIMASK(); __disable_irq(); } while(0)



#define QF_CRIT_EXIT(x_)  __set_PRIMASK(x_)

/////////////////////////////////////////////////////

#define QUEUE_SIZE 16

typedef struct {

	Event buffer[QUEUE_SIZE];

	volatile uint16_t count;
	volatile uint16_t head;  //yazma
	volatile uint16_t tail;	 //okuma

}EventQueue;

void Queue_Init(EventQueue *q);
bool Queue_Push(EventQueue *q, const Event *e);
bool Queue_Pop(EventQueue *q, Event *e);

#endif /* INC_EVENT_QUEUE_H_ */
