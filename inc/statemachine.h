/*
 * statemachine.h
 *
 *  Created on: Feb 27, 2026
 *      Author: th3ka
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <stdint.h>


typedef enum {
    INIT_SIG,
    SENSOR_READY_SIG,
    TELEMETRY_TICK_SIG,
    CMD_SIG,
	PRESS_TIMEOUT_SIG,
    BUZZER_TIMEOUT_SIG
} Signal;


typedef struct {
    Signal sig;
    uint8_t command;
} Event;


typedef struct StateMachine StateMachine;


typedef void (*StateHandler)(StateMachine * const me, Event const * const e);


struct StateMachine {
    StateHandler state;
};


void SM_Init(StateMachine * const me, StateHandler initial_state);
void SM_Dispatch(StateMachine * const me, Event const * const e);

#endif /* STATEMACHINE_H */
