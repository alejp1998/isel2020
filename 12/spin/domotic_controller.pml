

//SPECIFICATIONS


//Timers setup
int T; //Set to 1 when there is a timeout
#define timer int 
#define set(tmr,val) tmr=val; /*printf("\n***** Timeout set to: %d *****\n",val);*/
#define expire(tmr) (tmr==0) /*timeout*/ 
#define tick(tmr) if :: tmr>0 -> /*printf("\n***** Tic Tac: %d *****\n",tmr);*/ tmr=tmr-1 :: else /*printf("\n***** No Tic Tac: %d *****\n",tmr);*/ T=1  fi 

//#define delay(tmr,val) set(tmr,val); expire(tmr);

//TIMERS DURATION
#define SWITCH_TMR_PERIOD 10
#define CODE_TMR_PERIOD 1

timer switch_timer;
timer code_timer;

#define timeout true
active proctype Timers() { 
	do 
	:: timeout -> atomic { 
		tick(switch_timer);
		tick(code_timer);
	} 
	od
}

//SWITCH
//States
//mtype switch_states = {ON, OFF};
#define OFF  0
#define ON   1
int switch_st;

//Inputs
int button1 = 0;
int button2 = 0;

//Outputs
int light = 0;


// SWITCH FSM MODEL
active proctype switch_fsm () {
	//Initial state
	switch_st = OFF;
	T = 0;
	light = 0;

	do
	::	switch_st == OFF -> atomic {
		if :: (button1 || button2) ->
			button1 = 0;
			button2 = 0;

			//Change state
			switch_st = ON;
			
			//Turn light on
			light = 1;

			//Set timeout to 0 and start timer
			T = 0;
			set(switch_timer,SWITCH_TMR_PERIOD);
		fi;
	}
	::	switch_st == ON -> atomic {
		if :: (button1 || button2) ->
			button1 = 0;
			button2 = 0;

			//Set timeout to 0 and start timer
			T = 0;
			set(switch_timer,SWITCH_TMR_PERIOD);
		fi;
		if :: T ->
			T = 0;
			button1 = 0;
			button2 = 0;

			//Change state
			switch_st = OFF;
			
			//Turn light off
			light = 0;
		fi;
	}
	od
}


//ALARM
//States
//mtype switch_states = {INACTIVE, ACTIVE, TRIGGERED};
#define INACTIVE  0
#define ACTIVE 1
#define TRIGGERED 2
int alarm_st;

//Inputs
int pir_sensor = 0;
int alarm_code = 0;

//Outputs
int led = 0;
int buzzer = 0;


// ALARM FSM MODEL
active proctype alarm_fsm () {
	//Initial state
	switch_st = OFF;
	led = 0;
	buzzer = 0;

	do
	::	alarm_st == INACTIVE -> atomic {
		if :: alarm_code ->
			pir_sensor = 0;

			//Change state
			alarm_st = ACTIVE;
		fi;
	}
	::	alarm_st == ACTIVE -> atomic {
		if :: alarm_code ->
			pir_sensor = 0;

			//Change state
			alarm_st = INACTIVE;
		fi;
		if :: pir_sensor ->
			pir_sensor = 0;

			//Change state
			alarm_st = TRIGGERED;

			//Turn led and buzzer on 
			led = 1;
			buzzer = 1;
		fi;
	}
	::	alarm_st == TRIGGERED -> atomic {
		if :: alarm_code ->
			pir_sensor = 0;

			//Change state
			alarm_st = INACTIVE;

			//Turn led and buzzer off 
			led = 0;
			buzzer = 0;
		fi;
	}
	od
}


//CODE
//States
//mtype code_states = {IDLE, COUNT, BUFFER_DIGIT};
#define IDLE  0
#define COUNT 1
#define BUFFER_DIGIT 2
int code_st;

//Inputs
int key = 0;

//Outputs
//int alarm_code = 0;

//Internal variables
/* The code is abstracted and suppossed to be always correct
after entering the three digits */
#define COUNT_LIMIT 10
#define CODE_LENGTH 3

int count = 0;
int index = 0;


// CODE FSM MODEL
active proctype code_fsm () {
	//Initial state
	code_st = IDLE;
	alarm_code = 0;
	count = 0;
	index = 0;

	do
	::	code_st == IDLE -> atomic {
		if :: key ->
			key = 0;

			//Change state
			code_st = COUNT;

			//Increase count
			count++;
		fi;
	}
	::	code_st == COUNT -> atomic {
		if :: key ->
			key = 0;

			//Increase count
			count++;

			//Start timer
			T = 0;
			set(code_timer,CODE_TMR_PERIOD);
		fi;
		if :: T || (COUNT_LIMIT <= count) ->
			key = 0;
			T = 0;

			//Change state
			code_st = BUFFER_DIGIT;

			//Increase index and reset count
			count = 0;
			index++;
		fi;
	}
	::	code_st == BUFFER_DIGIT -> atomic {
		if :: key & (index < CODE_LENGTH) ->
			key = 0;

			//Change state
			code_st = COUNT;

			//Increase count
			count++;

			//Start timer
			T = 0;
			set(code_timer,CODE_TMR_PERIOD);
		fi;
		if :: index >= CODE_LENGTH ->
			key = 0;

			//Change state
			code_st = IDLE;

			//Reset index and count
			index = 0;
			count = 0;

			//Correct code signal
			alarm_code = 1;
		fi;
	}
	od
}



// ENVIRONMENT MODEL
active proctype environment () {
	printf ("\n---------------------------------------------------------------------------\n");
	printf ("\n SWITCH FSM --- ST:%d, T:%d, B1:%d, B2:%d, Light:%d \n", switch_st, T, button1, button2, light);
	printf ("\n CODE FSM   --- ST:%d, T:%d, Key:%d, Count:%d, Index:%d, CodeOK:%d \n", code_st, T, key, count, index, alarm_code);
	printf ("\n ALARM FSM  --- ST:%d, CodeOK:%d, PIR:%d, Led:%d, Buzzer:%d \n", code_st, alarm_code, pir_sensor, led, buzzer);
	printf ("\n---------------------------------------------------------------------------\n");
	do
	::	if
		:: skip
		:: button1 = 1;
		:: button2 = 1;
		:: key = 1;
		:: pir_sensor = 1;
		fi;

		printf ("\n---------------------------------------------------------------------------\n");
		printf ("\n SWITCH FSM --- ST:%d, T:%d, B1:%d, B2:%d, Light:%d \n", switch_st, T, button1, button2, light);
		printf ("\n CODE FSM   --- ST:%d, T:%d, Key:%d, Count:%d, Index:%d, CodeOK:%d \n", code_st, T, key, count, index, alarm_code);
		printf ("\n ALARM FSM  --- ST:%d, CodeOK:%d, PIR:%d, Led:%d, Buzzer:%d \n", code_st, alarm_code, pir_sensor, led, buzzer);
		printf ("\n---------------------------------------------------------------------------\n");
	od
}


//run switch_fsm();
//run code_fsm();
//run alarm_fsm();