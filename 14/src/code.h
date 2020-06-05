

#ifndef _CODE_H_
#define _CODE_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"

#define GPIO_CODE_BUTTON 16

#define DEBOUNCE_TIME 50

#define CODE_TMR_TICKS 1000/50 //(Updated every Hs=50ms)
#define COUNT_LIMIT 10
#define CODE_LENGTH 3

//FSM STATES
enum code_fsm_state {
  IDLE,
  COUNT,
  BUFFER_DIGIT
};

//TIMER UPDATE
void update_code_timer();

//INTERRUPTIONS
void key_isr   (void);
void timer_code_isr (void);

//FSM CREATION
fsm_t* fsm_new_code (void);

#endif