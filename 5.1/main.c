
#include "time.h"
#include "stdio.h"
#include "pthread.h"

#include "timer.h"
#include "kbhit.h"

#include "interruptor.h"


//CHECK PRESSED KEYS
void *checkInputsThread (void *arg)
{
	char pressedKey;
	while (1){
        if (kbhit()){
            pressedKey = kbread();
            switch(pressedKey){
                case 'b' :
                    boton_isr();
                    break;
                case 'v' :
                    boton_isr();
                    break;
                case 'q' :
                    exit(-1);
                    break;
                default : 
                    break;
            }
        }   
        else 
            continue;
    }
	pthread_exit(NULL);
}

/*void initializePins ()
{
	wiringPiSetup();
    pinMode (GPIO_BUTTON1, INPUT);
    pinMode (GPIO_BUTTON2, INPUT);
    wiringPiISR (GPIO_BUTTON1, INT_EDGE_FALLING, boton_isr);
    wiringPiISR (GPIO_BUTTON2, INT_EDGE_FALLING, boton_isr);
    pinMode (GPIO_LIGHT, OUTPUT);
    apagar (interruptor_fsm);
}
*/

int main () {

    //Thread that checks inputs in keyboard and triggers interruption functions
	pthread_t thInputs;
	if ( 0 != pthread_create(&thInputs, NULL, checkInputsThread, NULL) ) {
		printf("Thread not started\n");
	}

    //Clock registers
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	struct timespec T = {0, 0.5*1000000000}; //Half a second

    //Initialze input and output pins
    //initializePins()

    /*
    * Máquina de estados: lista de transiciones
    * { EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
    */
    static fsm_trans_t interruptor[] = {
        { APAGADO,   boton_pulsado, ENCENDIDO, encender },
        { ENCENDIDO, boton_pulsado, ENCENDIDO, encender },
        { ENCENDIDO, timer_acabado, APAGADO,   apagar },
        {-1, NULL, -1, NULL },
    };
    fsm_t* interruptor_fsm = fsm_new (interruptor);
    
    while (1) {
        fsm_fire (interruptor_fsm);
        timespec_add(&next,&next,&T);
        delay_until (&next);

        //Check if the timer has finished
        if (timer_finished()){
            timer_isr();
        }
    }
}