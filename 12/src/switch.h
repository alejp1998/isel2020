

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"

#define GPIO_BUTTON1	2
#define GPIO_BUTTON2	3
#define GPIO_LIGHT	  4

#define DEBOUNCE_TIME 200

#define SWITCH_TMR_TICKS 10000/50 //(Updated every Hs=50ms)

enum fsm_state {
  ON,
  OFF,
};

//TIMER UPDATE
static void start_switch_timer();
void update_switch_timer();

//INTERRUPTIONS ROUTINES
void button_isr (void);
void timer_switch_isr (void);

//STATE CHECKING FUNCTIONS
static int check_pressed_button (fsm_t* this);
static int check_switch_timer_ended(fsm_t* this);

//OUTPUT FUNCTIONS
static void turn_on_light (fsm_t* this);
static void turn_off_light (fsm_t* this);

//FSM CREATION
fsm_t* fsm_new_switch (void);

#endif