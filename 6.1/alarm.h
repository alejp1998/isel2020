

#ifndef _INTERRUPTOR_H_
#define _INTERRUPTOR_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"

#define GPIO_PIR	    12
#define GPIO_ALARM_ST	14
#define GPIO_LIGHT	  4
#define GPIO_BUZZER	  2

#define DEBOUNCE_TIME 200

#define TIMER_PERIOD 10

enum fsm_state {
  INACTIVE,
  ACTIVE,
  TRIGGERED
};

//INTERRUPTIONS
void pir_isr         (void);
void alarm_state_isr (void);

//STATE CHECKING
int check_presence    (fsm_t* this);
int check_alarm_state (fsm_t* this);

//OUTPUT FUNCTIONS
void activate_alarm    (fsm_t* this);
void deactivate_alarm  (fsm_t* this);
void trigger_alarm     (fsm_t* this);

#endif