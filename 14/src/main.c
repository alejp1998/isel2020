
#include "time.h"
#include "stdio.h"

#include "reactor.h"

#include "kbhit.h"

#include "switch.h"
#include "alarm.h"
#include "code.h"

//FSM Declarations
fsm_t* switch_fsm;
static const struct timeval switch_period = {0, 500*1000000};

fsm_t* alarm_fsm;
static const struct timeval alarm_period = {0, 250*1000000};

fsm_t* code_fsm;
static const struct timeval code_period = {0, 50*1000000};



//CHECK PRESSED KEYS
static const struct timeval key_period = {0, 50*1000000};
void processKey() {
    char pressedKey;
    if(kbhit()){
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
}


//REACTOR EVENTS
//Process key task and update timers state
static void processKey_task (struct event_handler_t* this) {
    processKey();

    update_code_timer();
    update_switch_timer();

    timeval_add (&this->next_activation, &this->next_activation, &key_period);
}

//Alarm fsm task
static void alarm_task (struct event_handler_t* this) {
    fsm_fire(alarm_fsm);
    timeval_add (&this->next_activation, &this->next_activation, &alarm_period);
}

//Code fsm task
static void code_task (struct event_handler_t* this) {
    fsm_fire(code_fsm);
    timeval_add (&this->next_activation, &this->next_activation, &code_period);
}

//Switch fsm task
static void switch_task (struct event_handler_t* this) {
    fsm_fire(switch_fsm);
    timeval_add (&this->next_activation, &this->next_activation, &switch_period);
}

//Initialize events handler
void reactor_events_init () {
    EventHandler task_processKey, task_alarm, task_code, task_switch;

    reactor_init();

    event_handler_init (&task_processKey, 4, processKey_task);
    reactor_add_handler (&task_processKey);

    event_handler_init (&task_code, 3, code_task);
    reactor_add_handler (&task_code);

    event_handler_init (&task_alarm, 2, alarm_task);
    reactor_add_handler (&task_alarm);

    event_handler_init (&task_switch, 1, switch_task);
    reactor_add_handler (&task_switch);
}


/*
void initializePins ()
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

    //Initialze input and output pins
    //initializePins()

    /*
    * Finite States Machine
    * { OriginState, Trigger, DestinationState, Actions }
    */
    fsm_t* switch_fsm = fsm_new (switch_def);
    fsm_t* alarm_fsm = fsm_new (alarm);
    fsm_t* code_fsm = fsm_new (alarm);

    //WELCOME MESSAGE
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("WELCOME!!! \n\n");
    printf("Controls: \n");
    printf("'v' or 'b' -> Turn light on (turned off automatically). \n");
    printf("    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("    'p'    -> Trigger presence sensor (PIR). \n");
    printf("    'q'    -> Exit program. \n");
    printf("-----------------------------------------------------------------------------------\n\n");

    //Initialize events
    reactor_events_init();

    //Start handling them
    while (1) {
        reactor_handle_events ();
    }

}