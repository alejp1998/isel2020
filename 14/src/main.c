
#include "time.h"
#include "stdio.h"

#include "reactor.h"

#include "kbd.h"
#include "switch.h"
#include "alarm.h"
#include "code.h"

//FSM Declarations
static fsm_t* kbd_fsm;
static const struct timeval kbd_period = {0, 50*1000000};

static fsm_t* switch_fsm;
static const struct timeval switch_period = {0, 500*1000000};

static fsm_t* alarm_fsm;
static const struct timeval alarm_period = {0, 250*1000000};

static fsm_t* code_fsm;
static const struct timeval code_period = {0, 50*1000000};


//REACTOR EVENTS
//Process key task and update timers state
static void kbd_task (struct event_handler_t* this) {
    fsm_fire(kbd_fsm);

    //Update timers
    update_code_timer();
    update_switch_timer();

    timeval_add (&this->next_activation, &this->next_activation, &kbd_period);
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

    //Initialize events
    EventHandler task_kbd, task_alarm, task_code, task_switch;
    reactor_init();

    /*
    * Finite States Machine
    * { OriginState, Trigger, DestinationState, Actions }
    */
    kbd_fsm = fsm_new_kbd();
    switch_fsm = fsm_new_switch();
    alarm_fsm = fsm_new_alarm();
    code_fsm = fsm_new_code();

    //WELCOME MESSAGE
    printf("\r\n-----------------------------------------------------------------------------------\n");
    printf("\rWELCOME!!! \n\n");
    printf("\rControls: \n");
    printf("\r'v' or 'b' -> Turn light on (turned off automatically). \n");
    printf("\r    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("\r    'p'    -> Trigger presence sensor (PIR). \n");
    printf("\r    'q'    -> Exit program. \n");
    printf("\r-----------------------------------------------------------------------------------\n\n");

    //Initialize and add tasks
    event_handler_init (&task_kbd, 4, kbd_task);
    reactor_add_handler (&task_kbd);

    event_handler_init (&task_code, 3, code_task);
    reactor_add_handler (&task_code);

    event_handler_init (&task_alarm, 2, alarm_task);
    reactor_add_handler (&task_alarm);

    event_handler_init (&task_switch, 1, switch_task);
    reactor_add_handler (&task_switch);

    //Start handling them
    while (1) {
        reactor_handle_events ();
    }

}