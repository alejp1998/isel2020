
#include "stdio.h"

#include "termios.h"
#include "sys/select.h"
#include "sys/time.h"
#include "task.h"

#include "timer.h"
#include "kbhit.h"

#include "switch.h"
#include "alarm.h"
#include "code.h"

fsm_t* alarm_fsm;
fsm_t* code_fsm;
fsm_t* switch_fsm;

//CHECK PRESSED KEYS
void processKey()
{
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


//THREADS
//Process key task and update timers state
static void* processKey_task (void* arg) {
    struct timeval *period = task_get_period (pthread_self());
    struct timeval next;

    gettimeofday (&next, NULL);

    while (1) {
        timeval_add (&next, &next, period);
        delay_until (&next);

        processKey();

        update_code_timer();
        update_switch_timer();
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
    printf("\n---------------------------------------------------------------------------------\n");
    printf("WELCOME!!! \n\n");
    printf("Controls: \n");
    printf("'v' or 'b' -> Turn light on (turned off automatically). \n");
    printf("    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("    'p'    -> Trigger presence sensor (PIR). \n");
    printf("    'q'    -> Exit program. \n");
    printf("-----------------------------------------------------------------------------------\n\n");

    //Initialize threads
    pthread_t processKey_tid, alarm_tid, code_tid, switch_tid;

    //mutex_init (&m_temp, 2);

    //Create tasks task_new (const char* name, void *(*f)(void *),int period_ms, int deadline_ms, int prio, int stacksize)
    processKey_tid = task_new ("processKey", processKey_task, 50, 50, 4, 2048);
    code_tid = task_new ("code", code_task, 50, 50, 3, 2048);
    alarm_tid = task_new ("alarm", alarm_task, 250, 250, 2, 2048);
    switch_tid = task_new ("switch", switch_task, 500, 500, 1, 2048);

    pthread_join (processKey_tid, NULL);
    pthread_join (code_tid, NULL);
    pthread_join (alarm_tid, NULL);
    pthread_join (switch_tid, NULL);

    return 0;
}