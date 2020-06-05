

#include "switch.h"
  

static int ticks = 0;
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

void timer_switch_isr(void) {
  timer_ended = 1;
}


//TIMER UPDATE
static void start_switch_timer(){
  ticks = 0;
}

void update_switch_timer(){
  if ((ticks > -1) & (ticks < SWITCH_TMR_TICKS)){
    //Increase ticks
    ticks++;
  } else if (ticks >= SWITCH_TMR_TICKS) {
    //Set ticks to -1
    ticks = -1;
    //Trigger function
    timer_switch_isr();
  }
  //printf("%d",ticks);
}


//STATE CHECKING FUNCTIONS
static int check_pressed_button (fsm_t* this) { 
  return button; 
}

static int check_switch_timer_ended (fsm_t* this) { 
  return timer_ended; 
}


//OUTPUT FUNCTIONS
static void turn_on_light (fsm_t* this) {
  button = 0;
  timer_ended = 0;

  printf("\rLIGHT ON! \n");

  start_switch_timer();

  digitalWrite (GPIO_LIGHT, 1); 
}

static void turn_off_light (fsm_t* this) { 
  timer_ended = 0;

  printf("\rLIGHT OFF! \n");

  digitalWrite (GPIO_LIGHT, 0); 
}


//FSM CREATION
fsm_t* fsm_new_switch (void) {
	static fsm_trans_t tt[] = {
    { OFF,   check_pressed_button, ON, turn_on_light },
    { ON, check_pressed_button, ON, turn_on_light },
    { ON, check_switch_timer_ended, OFF, turn_off_light },
    {-1, NULL, -1, NULL },
  };
	return fsm_new(tt);
}