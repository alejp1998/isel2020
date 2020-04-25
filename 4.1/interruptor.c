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

#include "interruptor.h"

static int boton = 0;
static int debounceTime = DEBOUNCE_TIME;

//Button interruption
void boton_isr (void) { 

  //Debouncing procedure
  if (millis() < debounceTime){
    debounceTime = millis() + DEBOUNCE_TIME;
    return;
  }

  boton = 1; 
  debounceTime = millis() + DEBOUNCE_TIME;
}

//Funcion de estado de las entradas
int boton_pulsado (fsm_t* this) { 
  return boton; 
}

//Funciones de salida
void encender (fsm_t* this) { 
  boton = 0; 
  digitalWrite (GPIO_LIGHT, 1); 
}
void apagar (fsm_t* this) { 
  boton = 0; 
  digitalWrite (GPIO_LIGHT, 0); 
}
