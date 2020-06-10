
#include "time.h"
#include "stdio.h"
#include "pthread.h"

#include "timer.h"
#include "ttyraw.h"
#include "termios.h"

#include "switch.h"
#include "alarm.h"
#include "code.h"
#include "kbd.h"

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
	struct timespec T = {0, 0.050*1000000000}; //T = 50ms (Hs) 

    //Initialze input and output pins
    //initializePins()

    /*
    * Finite States Machine
    * { OriginState, Trigger, DestinationState, Actions }
    */
    fsm_t* kbd_fsm = fsm_new_kbd();
    fsm_t* switch_fsm = fsm_new_switch();
    fsm_t* alarm_fsm = fsm_new_alarm();
    fsm_t* code_fsm = fsm_new_code();

    //WELCOME MESSAGE
    printf("\r\n---------------------------------------------------------------------------------\n");
    printf("\rWELCOME!!! \n\n");
    printf("\rControls: \n");
    printf("\r'v' or 'b' -> Turn light on (turned off automatically). \n");
    printf("\r    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("\r    'p'    -> Trigger presence sensor (PIR). \n");
    printf("\r    'q'    -> Exit program. \n");
    printf("\r-----------------------------------------------------------------------------------\n\n");

    int count = 0;
    int max_count = 1000;

    int kbd_exec_time = 0;
    int switch_exec_time = 0;
    int code_exec_time = 0;
    int alarm_exec_time = 0;

    int max_kbd_exec_time = 0;
    int max_switch_exec_time = 0;
    int max_code_exec_time = 0;
    int max_alarm_exec_time = 0;

    struct timespec t1;
    struct timespec t2;
    struct timespec exec_time;

    while (count < max_count) {
        /* PERIODS AND DEADLINES
            KBD_FSM    -> T = D = 50ms
            CODE_FSM   -> T = D = 50ms
            ALARM_FSM  -> T = D = 250ms
            SWITCH_FSM -> T = D = 500ms
        */

        //MEASURE EXECUTION TIMES

        //KBD
        clock_gettime(CLOCK_REALTIME, &t1);
        fsm_fire (kbd_fsm);
        clock_gettime(CLOCK_REALTIME, &t2);
        timespec_sub(&exec_time,&t2,&t1);
        if ((&exec_time)->tv_nsec > max_kbd_exec_time) {
            max_kbd_exec_time = (&exec_time)->tv_nsec;
        }
        kbd_exec_time += (&exec_time)->tv_nsec;
        //printf("\r[%d] KBD EXEC TIME: '%d' ns \n",count,kbd_exec_time);


        //CODE
        clock_gettime(CLOCK_REALTIME, &t1);
        fsm_fire (code_fsm);
        clock_gettime(CLOCK_REALTIME, &t2);
        timespec_sub(&exec_time,&t2,&t1);
        if ((&exec_time)->tv_nsec > max_code_exec_time) {
            max_code_exec_time = (&exec_time)->tv_nsec;
        }
        code_exec_time += (&exec_time)->tv_nsec;
        //printf("\r[%d] CODE EXEC TIME: '%d' ns \n",count,code_exec_time);

        //ALARM
        clock_gettime(CLOCK_REALTIME, &t1);
        fsm_fire (alarm_fsm);
        clock_gettime(CLOCK_REALTIME, &t2);
        timespec_sub(&exec_time,&t2,&t1);
        if ((&exec_time)->tv_nsec > max_alarm_exec_time) {
            max_alarm_exec_time = (&exec_time)->tv_nsec;
        }
        alarm_exec_time += (&exec_time)->tv_nsec;
        //printf("\r[%d] ALARM EXEC TIME: '%d' ns \n",count,alarm_exec_time);

        //SWITCH
        clock_gettime(CLOCK_REALTIME, &t1);
        fsm_fire (switch_fsm);
        clock_gettime(CLOCK_REALTIME, &t2);
        timespec_sub(&exec_time,&t2,&t1);
        if ((&exec_time)->tv_nsec > max_switch_exec_time) {
            max_switch_exec_time = (&exec_time)->tv_nsec;
        }
        switch_exec_time += (&exec_time)->tv_nsec;
        //printf("\r[%d] SWITCH EXEC TIME: '%d' ns \n",count,switch_exec_time);


        //Delay next execution
        timespec_add(&next,&next,&T);
        delay_until (&next);

        //Increase executions count
        count++;
    }

    //CALC MEAN EXECUTION TIMES (C)
    int mean_kbd_exec_time;
    int mean_code_exec_time;
    int mean_alarm_exec_time;
    int mean_switch_exec_time;

    mean_kbd_exec_time = kbd_exec_time/max_count;
    mean_code_exec_time = code_exec_time/max_count;
    mean_alarm_exec_time = alarm_exec_time/max_count;
    mean_switch_exec_time = switch_exec_time/max_count;

    printf("\r[%d] KBD    EXEC TIME -> | MEAN: %dns | | MAX: %dns |\n",count,mean_kbd_exec_time,max_kbd_exec_time);
    printf("\r[%d] CODE   EXEC TIME -> | MEAN: %dns | | MAX: %dns |\n",count,mean_code_exec_time,max_code_exec_time);
    printf("\r[%d] ALARM  EXEC TIME -> | MEAN: %dns | | MAX: %dns |\n",count,mean_alarm_exec_time,max_alarm_exec_time);
    printf("\r[%d] SWITCH EXEC TIME -> | MEAN: %dns | | MAX: %dns |\n",count,mean_switch_exec_time,max_switch_exec_time);
}