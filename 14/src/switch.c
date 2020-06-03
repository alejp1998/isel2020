

#include "switch.h"

//FSM DEFINITION
fsm_trans_t switch_def[] = {
    { OFF,   check_pressed_button, ON, turn_on_light },
    { ON, check_pressed_button, ON, turn_on_light },
    { ON, check_switch_timer_ended, OFF, turn_off_light },
    {-1, NULL, -1, NULL },
};
  

static int ticks = 0;
static int button = 0;
static int timer_ended = 0;

//INTERRUPTIONS ROUTINES
void button_isr (void) { 
  button = 1;
}

void timer_switch_isr(void) {
  timer_ended = 1;
}

//STATE CHECKING FUNCTIONS
int check_pressed_button (fsm_t* this) { 
  return button; 
}

int check_switch_timer_ended (fsm_t* this) { 
  return timer_ended; 
}


//OUTPUT FUNCTIONS
void turn_on_light (fsm_t* this) {
  button = 0;
  timer_ended = 0;

  printf("LIGHT ON! \n");

  start_switch_timer();

  digitalWrite (GPIO_LIGHT, 1); 
}

void turn_off_light (fsm_t* this) { 
  timer_ended = 0;

  printf("LIGHT OFF! \n");

  digitalWrite (GPIO_LIGHT, 0); 
}


//TIMER UPDATE
void start_switch_timer(){
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
