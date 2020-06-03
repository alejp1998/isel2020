

//Timers setup
int T; //Set to 1 when there is a timeout
#define timer int 
#define set(tmr,val) tmr=val; /*printf("\n***** Timeout set to: %d *****\n",val);*/
#define expire(tmr) (tmr==0) /*timeout*/ 
#define tick(tmr) if :: tmr>0 -> /*printf("\n***** Tic Tac: %d *****\n",tmr);*/ tmr=tmr-1 :: else /*printf("\n***** No Tic Tac: %d *****\n",tmr);*/ T=1  fi 

//Timers initialization
timer code_timer;

//Timers durations
#define SWITCH_TMR_PERIOD 10
#define CODE_TMR_PERIOD 1
#define timeout true
active proctype Timers() { 
	do 
	:: timeout -> atomic {
		tick(code_timer);
	} 
	od
}


//ALARM
//Outputs
int alarm_code = 0;

// ALARM FSM MODEL (Simplification, just deactivates the correct code signal when it is set to 1)
active proctype alarm_fsm () {
	do
	:: alarm_code -> alarm_code = 0
    :: !alarm_code -> skip
	od;
}

//CODE
//States
//mtype code_states = {IDLE, COUNT, BUFFER_DIGIT};
#define IDLE  0
#define COUNT 1
#define BUFFER_DIGIT 2
int code_st;

#define COUNT_LIMIT 4 //To decrease verification time, in the implemented model is 10
#define CODE_LENGTH 3

//Inputs
int key = 0;

//Outputs
//int alarm_code = 0;

//Internal variables
byte correct_code[CODE_LENGTH] = {1, 2, 3};
byte current_code[CODE_LENGTH] = {0,0,0};
int i;
int correct;
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
		if 
		:: key ->
			key = 0;

			//Change state
			code_st = COUNT;

			//Increase count
			count++;
		fi;
	}
	::	code_st == COUNT -> atomic {
		if 
		:: key ->
			key = 0;

			//Increase count
			count++;

			//Start timer
			T = 0;
			set(code_timer,CODE_TMR_PERIOD);
		:: T || (COUNT_LIMIT <= count) ->
			key = 0;
			T = 0;

            //Append count to code
            if 
            :: count < COUNT_LIMIT -> 
                current_code[index] = count;
            :: else -> 
                current_code[index] = 0;
            fi

			//Change state
			code_st = BUFFER_DIGIT;

			//Increase index and reset count
			count = 0;
			index++;
		fi;
	}
	::	code_st == BUFFER_DIGIT -> atomic {
		if 
		:: key & (index < CODE_LENGTH) ->
			key = 0;
			
			//Change state
			code_st = COUNT;

			//Increase count
			count++;

			//Start timer
			T = 0;
			set(code_timer,CODE_TMR_PERIOD);
		:: index >= CODE_LENGTH ->
			key = 0;
            T = 0;

			//Change state
			code_st = IDLE;

			//Correct code signal
            i = 0;
            correct = 1;
            do 
            :: i < CODE_LENGTH -> 
                correct = correct & (current_code[i] == correct_code[i]);
                current_code[i] = 0;
                i++;
            :: else -> break
            od

            if
            :: correct ->
			    alarm_code = 1;
            fi

            //Reset index and count
			index = 0;
			count = 0;

		fi;
	}
	od;
}


// ENVIRONMENT MODEL
active proctype environment () {
	
	printf ("\n---------------------------------------------------------------------------\n");
	printf ("\n CODE FSM   --- ST:%d, T:%d, Key:%d, Count:%d, Index:%d, CodeOK:%d \n", code_st, T, key, count, index, alarm_code);
	printf ("\n---------------------------------------------------------------------------\n");

	do
	::	if
		:: !(key) -> skip
		:: key = 1;
		fi

		printf ("\n---------------------------------------------------------------------------\n");
		printf ("\n CODE FSM   --- ST:%d, T:%d, Key:%d, Count:%d, Index:%d, Code = [%d,%d,%d], CodeOK:%d \n", code_st, T, key, count, index, current_code[0],current_code[1],current_code[2], alarm_code);
		printf ("\n---------------------------------------------------------------------------\n");
	od

}


//SPECIFICATIONS
//Code model
ltl digit_processed_after_count_limit_or_timeout {
	[]( ((T || (count >= COUNT_LIMIT)) && (code_st == COUNT)) -> <>(code_st == BUFFER_DIGIT));
}
ltl three_digits_code_entered_leads_to_idle {
	[](((index+1) > CODE_LENGTH) -> <>(code_st == IDLE));
}
ltl correct_code_activates_alarm_code_signal {
	[](((current_code[0]==correct_code[0]) && (current_code[1]==correct_code[1]) && (current_code[2]==correct_code[2])) -> <>alarm_code);
}
