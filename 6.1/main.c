
#include "time.h"
#include "stdio.h"
#include "pthread.h"

#include "timer.h"
#include "kbhit.h"

#include "alarm.h"

//CHECK PRESSED KEYS
void *checkInputsThread (void *arg)
{
	char pressedKey;
	while (1){
        if (kbhit()){
            pressedKey = kbread();
            switch(pressedKey){
                case 's' :
                    alarm_state_isr();
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
	struct timespec T = {0, 0.5*1000000000}; //Half a second

    //Initialze input and output pins
    //initializePins()

    /*
    * Finite States Machine
    * { OriginState, Trigger, DestinationState, Actions }
    */
    static fsm_trans_t alarm[] = {
        { INACTIVE,  check_alarm_state, ACTIVE, activate_alarm },
        { ACTIVE, check_alarm_state, INACTIVE, deactivate_alarm },
        { ACTIVE, check_presence, TRIGGERED, trigger_alarm },
        { TRIGGERED, check_alarm_state, INACTIVE, deactivate_alarm },
        {-1, NULL, -1, NULL },
    };
    fsm_t* alarm_fsm = fsm_new (alarm);
    
    while (1) {
        fsm_fire (alarm_fsm);
        timespec_add(&next,&next,&T);
        delay_until (&next);
    }
}