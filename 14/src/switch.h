

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"
#include "timer.h"

#define GPIO_BUTTON1	2
#define GPIO_BUTTON2	3
#define GPIO_LIGHT	  4

enum switch_fsm_states {
  ON,
  OFF,
};

//INTERRUPTIONS
void button_isr (void);
void timer_switch_isr (void);

//FSM CREATION
fsm_t* fsm_new_switch (void);

#endif