

#include "switch.h"

static int button = 0;
static int timer_ended = 0;
static int debounceTime = DEBOUNCE_TIME;

//INTERRUPTIONS ROUTINES
void button_isr (void) { 

  //Debouncing procedure
  if (millis() < debounceTime){
    debounceTime = millis() + DEBOUNCE_TIME;
    return;
  }

  button = 1;

  debounceTime = millis() + DEBOUNCE_TIME;
}

void timer_isr(void) {
  timer_ended = 1;
}

//STATE CHECKING FUNCTIONS
int check_pressed_button (fsm_t* this) { 
  return button; 
}

int check_timer_ended (fsm_t* this) { 
  return timer_ended; 
}


//OUTPUT FUNCTIONS
void turn_on_light (fsm_t* this) {
  button = 0;
  timer_ended = 0;

  timer_start(TIMER_PERIOD);

  digitalWrite (GPIO_LIGHT, 1); 
}

void turn_off_light (fsm_t* this) { 
  timer_ended = 0;

  digitalWrite (GPIO_LIGHT, 0); 
}
