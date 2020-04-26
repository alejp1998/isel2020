/* FSM example

   Copyright (C) 2017 by Jose M. Moya

   This file is part of GreenLSI Unlessons.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
  alarm_state = ~alarm_state;

  debounceTime = millis() + DEBOUNCE_TIME;
}


//STATE CHECKING FUNCTIONS
int check_presence (fsm_t* this) { 
  return pir_sensor & alarm_state; 
}

int check_alarm_off (fsm_t* this) { 
  return ~alarm_state; 
}

//OUTPUT FUNCTIONS
void start_alarm (fsm_t* this) {
  pir_sensor = 0;

  printf("ALARM ON \n");

  //Turn light and alarm off
  digitalWrite (GPIO_LIGHT, 1); 
  digitalWrite (GPIO_BUZZER, 1); 
}
void stop_alarm (fsm_t* this) { 
  pir_sensor = 0;

  printf("ALARM STOPPED \n");

  //Turn light and alarm on
  digitalWrite (GPIO_LIGHT, 0); 
  digitalWrite (GPIO_BUZZER, 0); 
}
void ignore_presence (fsm_t* this) { 
  pir_sensor = 0;
}