/*
 * statemachine.c
 *
 *  Created on: Feb 27, 2026
 *      Author: th3ka
 */


#include "statemachine.h"

void SM_Init(StateMachine * const me, StateHandler initial_state)
{
	me->state = initial_state;

	Event init_evt = {INIT_SIG, 0};
	(*me->state)(me, &init_evt);
}

void SM_Dispatch(StateMachine * const me, Event const * const e)
{
	(*me->state)(me, e);
}
