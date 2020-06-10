
#include "stdio.h"

#include "termios.h"
#include "sys/select.h"
#include "sys/time.h"
#include "task.h"

#include "timer.h"

#include "kbd.h"
#include "switch.h"
#include "alarm.h"
#include "code.h"

static fsm_t* kbd_fsm;
static fsm_t* alarm_fsm;
static fsm_t* code_fsm;
static fsm_t* switch_fsm;


//THREADS
//Process key task and update timers state
static void* kbd_task (void* arg) {
    kbd_fsm = fsm_new_kbd();
    
    struct timeval *period = task_get_period (pthread_self());
    struct timeval next;

    gettimeofday (&next, NULL);

    while (1) {
        timeval_add (&next, &next, period);
        delay_until (&next);

        fsm_fire(kbd_fsm);
    }
}

//Alarm fsm task
static void* alarm_task (void* arg) {
    alarm_fsm = fsm_new_alarm();

    struct timeval *period = task_get_period (pthread_self());
    struct timeval next;

    gettimeofday (&next, NULL);
    while (1) {
        timeval_add (&next, &next, period);
        delay_until (&next);
        
        fsm_fire(alarm_fsm);
    }
}

//Code fsm task
static void* code_task (void* arg) {
    code_fsm = fsm_new_code();

    struct timeval *period = task_get_period (pthread_self());
    struct timeval next;

    gettimeofday (&next, NULL);
    while (1) {
        timeval_add (&next, &next, period);
        delay_until (&next);
        
        fsm_fire(code_fsm);
    }
}

//Switch fsm task
static void* switch_task (void* arg) {
    switch_fsm = fsm_new_switch();

    struct timeval *period = task_get_period (pthread_self());
    struct timeval next;

    gettimeofday (&next, NULL);
    while (1) {
        timeval_add (&next, &next, period);
        delay_until (&next);
        
        fsm_fire(switch_fsm);
    }
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

    //WELCOME MESSAGE
    printf("\r\n---------------------------------------------------------------------------------\n");
    printf("\rWELCOME!!! \n\n");
    printf("\rControls: \n");
    printf("\r'v' or 'b' -> Turn light on (turned off automatically). \n");
    printf("\r    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("\r    'p'    -> Trigger presence sensor (PIR). \n");
    printf("\r    'q'    -> Exit program. \n");
    printf("\r-----------------------------------------------------------------------------------\n\n");

    //Initialize threads
    pthread_t kbd_tid, alarm_tid, code_tid, switch_tid;

    //Create tasks task_new (const char* name, void *(*f)(void *),int period_ms, int deadline_ms, int prio, int stacksize)
    kbd_tid = task_new ("kbd", kbd_task, 50, 50, 4, 2048);
    code_tid = task_new ("code", code_task, 50, 50, 3, 2048);
    alarm_tid = task_new ("alarm", alarm_task, 250, 250, 2, 2048);
    switch_tid = task_new ("switch", switch_task, 500, 500, 1, 2048);

    pthread_join (kbd_tid, NULL);
    pthread_join (code_tid, NULL);
    pthread_join (alarm_tid, NULL);
    pthread_join (switch_tid, NULL);

    return 0;
}