/*
 * blinky.h
 *
 *  Created on: Mar 1, 2026
 *      Author: th3ka
 */

#ifndef INC_BLINKY_H_
#define INC_BLINKY_H_

#include "statemachine.h"

typedef struct {
	StateMachine super;

}blinky_sm;

void blinky_sm_init(blinky_sm* me);


#endif /* INC_BLINKY_H_ */
