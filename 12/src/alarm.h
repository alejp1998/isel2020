

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

#define DEBOUNCE_TIME 200

enum alarm_fsm_states {
  INACTIVE,
  ACTIVE,
  TRIGGERED
};

//INTERRUPTIONS
void pir_isr         (void);
void alarm_state_isr (void);

//STATE CHECKING
static int check_presence    (fsm_t* this);
static int check_alarm_code (fsm_t* this);

//OUTPUT FUNCTIONS
static void activate_alarm    (fsm_t* this);
static void deactivate_alarm  (fsm_t* this);
static void trigger_alarm     (fsm_t* this);

//FSM DEFINITION
fsm_t* fsm_new_alarm (void);

#endif