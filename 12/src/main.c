
#include "time.h"
#include "stdio.h"
#include "pthread.h"

#include "timer.h"
#include "kbhit.h"

#include "switch.h"
#include "alarm.h"
#include "code.h"

//CHECK PRESSED KEYS
void processKey()
{
    char pressedKey;
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

    //Clock registers
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	struct timespec T = {0, 0.050*1000000000};

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

    //Frame index
    int frame = 0;

    while (1) {
        //Read pressed keys
        if (kbhit()){
            processKey();
        }

        /* 
        CYCLIC EXECUTIVE
            CODE_FSM   -> T = 50ms
            ALARM_FSM  -> T = 250ms
            SWITCH_FSM -> T = 500ms
        */
        switch (frame) {
			case 0:
				fsm_fire (code_fsm);
                fsm_fire (alarm_fsm);
                fsm_fire (switch_fsm);
				break;
			case 1:
				fsm_fire (code_fsm);
				break;
			case 2:
				fsm_fire (code_fsm);
				break;
			case 3:
				fsm_fire (code_fsm);
				break;
            case 4:
				fsm_fire (code_fsm);
				break;
			case 5:
				fsm_fire (code_fsm);
                fsm_fire (alarm_fsm);
                break;
			case 6:
				fsm_fire (code_fsm);
				break;
            case 7:
				fsm_fire (code_fsm);
				break;
			case 8:
				fsm_fire (code_fsm);
				break;
			case 9:
				fsm_fire (code_fsm);
				break;
		}

        //Delay next execution
        timespec_add(&next,&next,&T);
        delay_until (&next);

        //Increment frame index
        frame = (frame+1)%10;
    }
}