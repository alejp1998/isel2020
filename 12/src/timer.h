

#ifndef _TMR_H_
#define _TMR_H_

#include <signal.h>
#include <time.h>

//FSM Time Management functions
void timespec_sub (struct timespec *res, struct timespec *a, struct timespec *b);
void timespec_add (struct timespec *res, struct timespec *a, struct timespec *b);
int timespec_less (struct timespec *a, struct timespec *b);
void delay_until (struct timespec* next_activation);

//Timer functions
struct tmr_t {
    timer_t timerid;
    struct itimerspec spec;
    struct sigevent se;
};
typedef struct tmr_t tmr_t;

typedef void (*notify_func_t) (union sigval);

tmr_t* tmr_new (notify_func_t isr);
void tmr_init (tmr_t* this, notify_func_t isr);
void tmr_destroy(tmr_t* this);
void tmr_startms(tmr_t* this, int ms);
void tmr_stop (tmr_t* this);

#endif