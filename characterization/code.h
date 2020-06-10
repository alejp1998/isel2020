

#ifndef _CODE_H_
#define _CODE_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"
#include "timer.h"

#include "alarm.h"

#define GPIO_CODE_BUTTON 16

enum code_fsm_states {
  IDLE,
  COUNT,
  BUFFER_DIGIT
};

//INTERRUPTIONS
void key_isr   (void);
void timer_code_isr (void);

//FSM CREATION
fsm_t* fsm_new_code (void);

#endif