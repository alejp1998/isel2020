

#ifndef _ALARM_H_
#define _ALARM_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"

#define GPIO_PIR	    12
#define GPIO_ALARM_ST	14
#define GPIO_LIGHT	  4
#define GPIO_BUZZER	  2

enum alarm_fsm_states {
  INACTIVE,
  ACTIVE,
  TRIGGERED
};

//INTERRUPTIONS
void pir_isr         (void);
void alarm_code_isr (void);

//FSM DEFINITION
fsm_t* fsm_new_alarm (void);

#endif