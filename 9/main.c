
#include "time.h"
#include "stdio.h"
#include "pthread.h"

#include "timer.h"
#include "kbhit.h"

#include "switch.h"
#include "alarm.h"
#include "code.h"

//CHECK PRESSED KEYS
void *checkInputsThread (void *arg)
{
	char pressedKey;
	while (1){
        if (kbhit()){
            pressedKey = kbread();
            switch(pressedKey){
                case 'b' :
                    button_isr();
                    break;
                case 'v' :
                    button_isr();
                    break;
                case 'k' :
                    key_isr();
                    break;
                case 'p' :
                    pir_isr();
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
	struct timespec T = {0, 0.025*1000000000};

    //Initialize timers
    tmr_t* tmr1 = tmr_new(timer_code_isr);
    code_timer = tmr1;
    tmr_t* tmr2 = tmr_new(timer_switch_isr);
    switch_timer = tmr2;

    //Initialze input and output pins
    //initializePins()

    /*
    * Finite States Machine
    * { OriginState, Trigger, DestinationState, Actions }
    */
    fsm_t* switch_fsm = fsm_new (switch_def);
    fsm_t* alarm_fsm = fsm_new (alarm);
    fsm_t* code_fsm = fsm_new (code);

    //WELCOME MESSAGE
    printf("\n---------------------------------------------------------------------------------\n");
    printf("WELCOME!!! \n\n");
    printf("Controls: \n");
    printf("'v' or 'b' -> Turn light on (turned off automatically). \n");
    printf("    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("    'p'    -> Trigger presence sensor (PIR). \n");
    printf("    'q'    -> Exit program. \n");
    printf("-----------------------------------------------------------------------------------\n");


    while (1) {
        fsm_fire (switch_fsm);
        fsm_fire (alarm_fsm);
        fsm_fire (code_fsm);

        timespec_add(&next,&next,&T);
        delay_until (&next);
    }
}