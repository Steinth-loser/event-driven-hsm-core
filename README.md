# Event-Driven HSM Core

A lightweight, bare-metal event-driven state machine framework written in C, designed primarily for microcontrollers (like STM32). 

This repository provides a highly efficient way to manage complex embedded software logic by decoupling hardware interrupts from application logic. Instead of handling heavy processing inside Interrupt Service Routines (ISRs), interrupts merely post events to a thread-safe queue. The main application loop then pulls these events and dispatches them to a Finite State Machine (FSM / HSM).

## 📂 Directory Structure
* **`src/` & `inc/`**: Contains the core library files (`statemachine.c/h`, `event_queue.c/h`).
* **`example/`**: Contains a practical implementation (`blinky.c/h`, `main.c`) demonstrating how to integrate the core into an STM32 HAL project.

---

## 🎯 Purpose and Core Concepts

In embedded systems, executing complex logic directly inside an interrupt can cause missed deadlines and stack overflows. This framework solves that by dividing the architecture into two main components:

1.  **Event Queue (`event_queue.h`)**: A lightweight, thread-safe circular buffer. It uses architecture-specific critical sections (disabling interrupts via `__disable_irq()`) to safely push events from ISRs and pop them from the main `while(1)` loop without data corruption.
2.  **State Machine (`statemachine.h`)**: A function-pointer-based state machine. It processes signals (events) and transitions between different state functions cleanly.

---

## 🚀 How to Use (Based on the Example)

Because C does not have native Object-Oriented features like classes or inheritance, this framework achieves **inheritance through composition**. 

### 1. Inheriting the StateMachine Class
To create your own custom state machine, define a `struct` and place the base `StateMachine` as its **first member**. This allows you to safely cast your custom struct to a `StateMachine` pointer.

```c
// blinky.h
#include "statemachine.h"

typedef struct {
    StateMachine super; // MUST be the first member to simulate inheritance
    // Add your custom variables here (e.g., timers, counters, flags)
} blinky_sm;

void blinky_sm_init(blinky_sm* me);
```

### 2. Defining States and Transitions
Each state is simply a function that takes a pointer to the state machine and the incoming event. Use a `switch` statement to handle different signals (`INIT_SIG`, `PRESS_TIMEOUT_SIG`, etc.).

To change states, simply reassign the `me->state` pointer and optionally dispatch an `INIT_SIG` to trigger entry actions for the new state.

```c
// blinky.c
static void State_LedOn(StateMachine * const me, Event const * const e) {
    blinky_sm * const led = (blinky_sm *)me; // Downcast to access custom variables

    switch(e->sig) {
        case INIT_SIG:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
            break;

        case PRESS_TIMEOUT_SIG:
            // State Transition
            me->state = &State_LedOff; 
            Event Init_Evt = {INIT_SIG, 0};
            SM_Dispatch(me, &Init_Evt);
            break;
    }
}
```

### 3. Initialization
Initialize your custom state machine by passing it to `SM_Init` along with the initial state handler.

```c
// blinky.c
void blinky_sm_init(blinky_sm* me) {
    SM_Init((StateMachine*)me, &State_LedOn); // Pass the initial state
}
```

### 4. Integration: The Event-Driven Architecture
The true power of this framework is seen in `main.c`. ISRs (like Timer callbacks or GPIO EXTI) only do one thing: **Generate Events and Push them to the Queue**.

```c
// Inside an Interrupt Service Routine (e.g., Timer or Button Press)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM1) {
        Event btn_event;
        btn_event.sig = PRESS_TIMEOUT_SIG;
        Queue_Push(&queue, &btn_event); // Safely push to queue
    }
}
```

The main `while(1)` loop acts as the dispatcher. It constantly checks the queue and dispatches pending events to the state machine. 

```c
// main.c
int main(void) {
    Queue_Init(&queue);
    blinky_sm_init(&blinky);
    
    // ... HAL Inits ...

    while (1) {
        Event current_event;
        // If there is an event in the queue, pop it and dispatch it
        if(Queue_Pop(&queue, &current_event)) {
            SM_Dispatch((StateMachine*)&blinky, &current_event);
        }
    }
}
```

---

## ⚙️ Portability Note
The `EventQueue` uses STM32 CMSIS core functions (`__get_PRIMASK()` and `__disable_irq()`) to create atomic critical sections. If you are porting this to a different architecture (e.g., AVR, PIC, or an RTOS environment like FreeRTOS), you only need to update the `QF_CRIT_ENTRY` and `QF_CRIT_EXIT` macros inside `event_queue.h`.
