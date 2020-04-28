
#include "alarm.h"

//FSM DEFINITION
fsm_trans_t alarm[] = {
    { INACTIVE,  check_alarm_code, ACTIVE, activate_alarm },
    { ACTIVE, check_alarm_code, INACTIVE, deactivate_alarm },
    { ACTIVE, check_presence, TRIGGERED, trigger_alarm },
    { TRIGGERED, check_alarm_code, INACTIVE, deactivate_alarm },
    {-1, NULL, -1, NULL },
};


static int alarm_code = 0;
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

void alarm_code_isr (void) { 
  alarm_code = 1;
}


//STATE CHECKING FUNCTIONS
int check_presence (fsm_t* this) { 
  return pir_sensor; 
}

int check_alarm_code (fsm_t* this) { 
  return alarm_code; 
}


//OUTPUT FUNCTIONS
void activate_alarm (fsm_t* this) {
  pir_sensor = 0;
  alarm_code = 0;

  printf("ALARM ACTIVATED \n");
}
void deactivate_alarm (fsm_t* this) { 
  pir_sensor = 0;
  alarm_code = 0;

  printf("ALARM DEACTIVATED \n");

  //Turn light and buzzer off
  digitalWrite (GPIO_LIGHT, 0); 
  digitalWrite (GPIO_BUZZER, 0); 
}
void trigger_alarm (fsm_t* this) { 
  pir_sensor = 0;
  alarm_code = 0;

  printf("PRESENCE DETECTED: ALARM TRIGGERED!!! \n");

  //Turn light and buzzer on
  digitalWrite (GPIO_LIGHT, 1); 
  digitalWrite (GPIO_BUZZER, 1); 
}