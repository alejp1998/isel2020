
#include "time.h"
#include "stdio.h"

#include "termios.h"
#include "sys/select.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "kbhit.h"

#include "switch.h"
#include "alarm.h"
#include "code.h"

//FSM Declarations
fsm_t* switch_fsm;
fsm_t* alarm_fsm;
fsm_t* code_fsm;

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


//FREERTOS 
//Process key task and update timers state
static void processKey_task (void* ignore) {
    portTickType period =  50 /portTICK_RATE_MS;

    portTickType last = xTaskGetTickCount();

    start_code_timer();
    start_switch_timer();

    while (1) {
        processKey();

        update_code_timer();
        update_switch_timer();

        vTaskDelayUntil (&last, period);
    }
}

//Alarm fsm task
static void alarm_task (void* ignore) {
    portTickType period =  250 /portTICK_RATE_MS;

    portTickType last = xTaskGetTickCount();

    while (1) {
        fsm_fire(alarm_fsm);
        vTaskDelayUntil (&last, period);
    }
}

//Code fsm task
static void code_task (void* ignore) {
    portTickType period =  50 /portTICK_RATE_MS;

    portTickType last = xTaskGetTickCount();

    while (1) {
        fsm_fire(code_fsm);
        vTaskDelayUntil (&last, period);
    }
}

//Switch fsm task
static void switch_task (void* ignore) {
    portTickType period =  500 /portTICK_RATE_MS;

    portTickType last = xTaskGetTickCount();

    while (1) {
        fsm_fire(switch_fsm);
        vTaskDelayUntil (&last, period);
    }
}

void user_init (void) {
    xTaskHandle task_processKey, task_alarm, task_code, task_switch;
    xTaskCreate (processKey_task, (const signed char*) "processKey", 2048, NULL, 4, &task_processKey);
    xTaskCreate (code_task, (const signed char*) "code", 2048, NULL, 3, &task_code);
    xTaskCreate (alarm_task, (const signed char*) "alarm", 2048, NULL, 2, &task_alarm);
    xTaskCreate (switch_task, (const signed char*) "switch", 2048, NULL, 1, &task_switch);
}

void vApplicationIdleHook (void) {}
void vMainQueueSendPassed (void) {}


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
    fsm_t* code_fsm = fsm_new (code);

    //WELCOME MESSAGE
    printf("\n---------------------------------------------------------------------------------\n");
    printf("WELCOME!!! \n\n");
    printf("Controls: \n");
    printf("'v' or 'b' -> Turn light on (turned off automatically). \n");
    printf("    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("    'p'    -> Trigger presence sensor (PIR). \n");
    printf("    'q'    -> Exit program. \n");
    printf("-----------------------------------------------------------------------------------\n\n");

    //Initialize user
    user_init();

    //Start tasks scheduling
    vTaskStartScheduler();

}