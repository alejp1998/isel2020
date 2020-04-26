
#include "alarm.h"

static int alarm_state = 0;
static int pir_sensor = 0;
static int debounceTime = DEBOUNCE_TIME;

//INTERRUPTIONS ROUTINES
void pir_isr (void) { 

  //Debouncing procedure
  if (millis() < debounceTime){
    debounceTime = millis() + DEBOUNCE_TIME;
    return;
  }

  pir_sensor= 1;

  debounceTime = millis() + DEBOUNCE_TIME;
}

void alarm_state_isr (void) { 

  //Debouncing procedure
  if (millis() < debounceTime){
    debounceTime = millis() + DEBOUNCE_TIME;
    return;
  }

  //Switch bit state 
  alarm_state = 1;

  debounceTime = millis() + DEBOUNCE_TIME;
}


//STATE CHECKING FUNCTIONS
int check_presence (fsm_t* this) { 
  return pir_sensor; 
}

int check_alarm_state (fsm_t* this) { 
  return alarm_state; 
}


//OUTPUT FUNCTIONS
void activate_alarm (fsm_t* this) {
  pir_sensor = 0;
  alarm_state = 0;

  printf("ALARM ACTIVATED \n");
}
void deactivate_alarm (fsm_t* this) { 
  pir_sensor = 0;
  alarm_state = 0;

  printf("ALARM DEACTIVATED \n");

  //Turn light and buzzer off
  digitalWrite (GPIO_LIGHT, 0); 
  digitalWrite (GPIO_BUZZER, 0); 
}
void trigger_alarm (fsm_t* this) { 
  pir_sensor = 0;
  alarm_state = 0;

  printf("PRESENCE DETECTED: ALARM TRIGGERED!!! \n");

  //Turn light and buzzer on
  digitalWrite (GPIO_LIGHT, 1); 
  digitalWrite (GPIO_BUZZER, 1); 
}