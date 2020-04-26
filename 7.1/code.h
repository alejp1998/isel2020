

#ifndef _CODE_H_
#define _CODE_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"
#include "timer.h"

#define GPIO_CODE_BUTTON 16

#define DEBOUNCE_TIME 50

#define TIMER_PERIOD 1
#define COUNT_LIMIT 10
#define CODE_LENGTH 3

//FSM STATES
enum code_fsm_state {
  IDLE,
  COUNT,
  BUFFER_DIGIT
};


//INTERRUPTIONS
void key_isr   (void);
void timer_isr (void);

//STATE CHECKING
int check_pressed_key (fsm_t* this);
int check_count_limit (fsm_t* this);
int check_timer_ended (fsm_t* this);
int check_code_ended (fsm_t* this);
int check_code_not_ended (fsm_t* this);

//OUTPUT FUNCTIONS
void increase_count (fsm_t* this);
void process_digit  (fsm_t* this);
void examine_code   (fsm_t* this);

//FSM DEFINITION
fsm_trans_t code[];

#endif