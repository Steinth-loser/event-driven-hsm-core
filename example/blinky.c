/*
 * blinky.c
 *
 *  Created on: Mar 1, 2026
 *      Author: th3ka
 */


#include "blinky.h"
#include "stm32f4xx_hal.h"
static void State_LedOn(StateMachine * const me, Event const * const e);
static void State_LedOff(StateMachine * const me, Event const * const e);

void blinky_sm_init(blinky_sm* me)
{
	SM_Init((StateMachine*)me, &State_LedOn);
}

static void State_LedOn(StateMachine * const me, Event const * const e)
{
	blinky_sm * const led = (blinky_sm *)me;

	switch(e->sig) {
		case INIT_SIG:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
			break;

		case PRESS_TIMEOUT_SIG:
			me->state = &State_LedOff;
			Event Init_Evt = {INIT_SIG, 0};
			SM_Dispatch(me, &Init_Evt);
			break;
	}
}

static void State_LedOff(StateMachine * const me, Event const * const e)
{
	blinky_sm * const led = (blinky_sm *)me;

	switch(e->sig) {
		case INIT_SIG:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			break;

		case PRESS_TIMEOUT_SIG:
			me->state = &State_LedOn;
			Event Init_Evt = {INIT_SIG, 0};
			SM_Dispatch(me, &Init_Evt);
			break;
	}
}
