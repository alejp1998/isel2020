

#ifndef _INTERRUPTOR_H_
#define _INTERRUPTOR_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"

#define CLK_MS 100

#define GPIO_PIR	    12
#define GPIO_ALARM_ST	14
#define GPIO_LIGHT	  4
#define GPIO_BUZZER	  2

#define DEBOUNCE_TIME 200

#define TIMER_PERIOD 10

enum fsm_state {
  IDLE,
  ALARM,
};

//INTERRUPTIONS
void pir_isr (void);
void alarm_state_isr (void);

//STATE CHECKING
int check_presence (fsm_t* this);
int check_alarm_off(fsm_t* this);

//OUTPUT FUNCTIONS
void start_alarm (fsm_t* this);
void stop_alarm  (fsm_t* this);
void ignore_presence  (fsm_t* this);

#endif