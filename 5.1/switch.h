

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"

#define GPIO_BUTTON1	2
#define GPIO_BUTTON2	3
#define GPIO_LIGHT	  4

#define DEBOUNCE_TIME 200

#define TIMER_PERIOD 10

enum fsm_state {
  ON,
  OFF,
};

//INTERRUPTIONS ROUTINES
void button_isr (void);
void timer_isr (void);

//STATE CHECKING FUNCTIONS
int check_pressed_button (fsm_t* this);
int check_timer_ended(fsm_t* this);

//OUTPUT FUNCTIONS
void turn_on_light (fsm_t* this);
void turn_off_light (fsm_t* this);

#endif