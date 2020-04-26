

#ifndef _INTERRUPTOR_H_
#define _INTERRUPTOR_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"

#define CLK_MS 100

#define GPIO_BUTTON1	2
#define GPIO_BUTTON2	3
#define GPIO_LIGHT	4 //LED WeMos

#define DEBOUNCE_TIME 200

#define TIMER_PERIOD 10

enum fsm_state {
  ENCENDIDO,
  APAGADO,
};

//Prototipos atencion interrupciones
void boton_isr (void);
void timer_isr (void);

//Prototipos funciones de entrada
int boton_pulsado (fsm_t* this);
int timer_acabado (fsm_t* this);

//Prototipos funciones de salida
void encender (fsm_t* this);
void apagar   (fsm_t* this);

#endif