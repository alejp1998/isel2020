

#include "timer.h"
#include <stdlib.h>

//FSM TIME MANAGEMENT
void timespec_sub (struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec - b->tv_sec;
	res->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (res->tv_nsec < 0) {
		--res->tv_sec;
		res->tv_nsec += 1000000000;
	}
}

/* res = a + b */
void timespec_add (struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec + b->tv_sec;
	res->tv_nsec = a->tv_nsec + b->tv_nsec;
	if (res->tv_nsec > 1000000000) {
		++res->tv_sec;
		res->tv_nsec -= 1000000000;
	}
}

/* a < b */
int timespec_less (struct timespec *a, struct timespec *b)
{
  return (a->tv_sec < b->tv_sec) ||
    ((a->tv_sec == b->tv_sec) && (a->tv_nsec < b->tv_nsec));
}

/* wait until next_activation (absolute time) */
void delay_until (struct timespec* next_activation)
{
	struct timespec resto, now;
	clock_gettime(CLOCK_REALTIME, &now);
	timespec_sub(&resto, next_activation, &now);
	while (nanosleep(&resto,&resto)>0);
}


//TIMERS
tmr_t*
tmr_new (notify_func_t isr)
{
    tmr_t* this = (tmr_t*) malloc (sizeof (tmr_t));
    tmr_init (this, isr);
    return this;

}

void
tmr_init (tmr_t* this, notify_func_t isr) {
    this->se.sigev_notify = SIGEV_THREAD;
    this->se.sigev_value.sival_ptr = &(this->timerid);
    this->se.sigev_notify_function = isr;
    this->se.sigev_notify_attributes = NULL;
    timer_create(CLOCK_REALTIME, &(this->se), &(this->timerid));  /* o CLOCK_MONOTONIC si se soporta */
}

void
tmr_destroy(tmr_t* this)
{
    tmr_stop (this);
    free(this);
}

void
tmr_startms(tmr_t* this, int ms) {
    this->spec.it_value.tv_sec = ms / 1000; //Cuando cuente los ms del efecto ejecutara el thread
    this->spec.it_value.tv_nsec = (ms % 1000) * 1000000;
    this->spec.it_interval.tv_sec = 0;
    this->spec.it_interval.tv_nsec = 0;
    timer_settime (this->timerid, 0, &(this->spec), NULL);

}

void
tmr_stop (tmr_t* this) {
    timer_delete (this->timerid);
}