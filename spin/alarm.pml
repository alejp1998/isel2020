

//CODE
//Outputs
int alarm_code = 0;

// CODE FSM MODEL (Simplification, just activates the correct code signal eventually)
active proctype code_fsm () {
	do
	:: alarm_code == 1
    :: alarm_code -> skip
	od;
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
//int alarm_code = 0;

//Outputs
int led = 0;
int buzzer = 0;


// ALARM FSM MODEL
active proctype alarm_fsm () {
	//Initial state
	alarm_st = INACTIVE;
	led = 0;
	buzzer = 0;

	do
	::	alarm_st == INACTIVE -> atomic {
		if 
		:: alarm_code ->
			pir_sensor = 0;

			//Change state
			alarm_st = ACTIVE;
		fi;
	}
	::	alarm_st == ACTIVE -> atomic {
		if 
		:: alarm_code ->
			pir_sensor = 0;

			//Change state
			alarm_st = INACTIVE; 
		:: pir_sensor ->
			pir_sensor = 0;

			//Change state
			alarm_st = TRIGGERED;

			//Turn led and buzzer on 
			led = 1;
			buzzer = 1;
		fi;
	}
	::	alarm_st == TRIGGERED -> atomic {
		if 
		:: alarm_code ->
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


// ENVIRONMENT MODEL
active proctype environment () {
	
	printf ("\n---------------------------------------------------------------------------\n");
	printf ("\n CODE FSM   --- CodeOK:%d \n", alarm_code);
	printf ("\n ALARM FSM  --- ST:%d, CodeOK:%d, PIR:%d, Led:%d, Buzzer:%d \n", alarm_st, alarm_code, pir_sensor, led, buzzer);
	printf ("\n---------------------------------------------------------------------------\n");

	do
	::	if
		:: !(pir_sensor) -> skip
		:: pir_sensor = 1;
		fi

		printf ("\n---------------------------------------------------------------------------\n");
		printf ("\n CODE FSM   --- CodeOK:%d \n", alarm_code);
		printf ("\n ALARM FSM  --- ST:%d, CodeOK:%d, PIR:%d, Led:%d, Buzzer:%d \n", alarm_st, alarm_code, pir_sensor, led, buzzer);
		printf ("\n---------------------------------------------------------------------------\n");
	od
    
}


//SPECIFICATIONS
//Alarm code
ltl alarm_deactivation_after_code_entered {
	[]( (((alarm_st==TRIGGERED) || (alarm_st==ACTIVE)) && alarm_code) -> <>((alarm_st==INACTIVE)) && !led && !buzzer);
}
ltl alarm_triggered_after_pir_if_active {
	[](((alarm_st==ACTIVE) && pir_sensor) -> <>((alarm_st==TRIGGERED) && led && buzzer));
}