
#include "time.h"
#include "stdio.h"
#include "pthread.h"

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
                //BOTON 1
                case 'b' :
                    button_isr();
                    break;
                //BOTON 2
                case 'v' :
                    button_isr();
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
    static fsm_trans_t switch_def[] = {
        { OFF,   check_pressed_button, ON, turn_on_light },
        { ON, check_pressed_button, ON, turn_off_light },
        {-1, NULL, -1, NULL },
    };
    fsm_t* switch_fsm = fsm_new (switch_def);
    
    while (1) {
        fsm_fire (switch_fsm);
        timespec_add(&next,&next,&T);
        delay_until (&next);
    }
}