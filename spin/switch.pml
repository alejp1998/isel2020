

//Timers setup
int T; //Set to 1 when there is a timeout
#define timer int 
#define set(tmr,val) tmr=val; /*printf("\n***** Timeout set to: %d *****\n",val);*/
#define expire(tmr) (tmr==0) /*timeout*/ 
#define tick(tmr) if :: tmr>0 -> /*printf("\n***** Tic Tac: %d *****\n",tmr);*/ tmr=tmr-1 :: else /*printf("\n***** No Tic Tac: %d *****\n",tmr);*/ T=1  fi 

//Timers initialization
timer switch_timer;

//Timers durations
#define SWITCH_TMR_PERIOD 10
#define CODE_TMR_PERIOD 1
#define timeout true
active proctype Timers() { 
	do 
	:: timeout -> atomic { 
		tick(switch_timer);
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
		if 
		:: (button1 || button2) ->
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
		if 
		:: (button1 || button2) ->
			button1 = 0;
			button2 = 0;

			//Set timeout to 0 and start timer
			T = 0;
			set(switch_timer,SWITCH_TMR_PERIOD);
		:: T ->
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


// ENVIRONMENT MODEL
active proctype environment () {
	
	printf ("\n---------------------------------------------------------------------------\n");
	printf ("\n SWITCH FSM --- ST:%d, T:%d, B1:%d, B2:%d, Light:%d \n", switch_st, T, button1, button2, light);
	printf ("\n---------------------------------------------------------------------------\n");

	do
	::	if
		:: !(button1 || button2) -> skip
		:: button1 = 1;
		:: button2 = 1;
		fi

		printf ("\n---------------------------------------------------------------------------\n");
		printf ("\n SWITCH FSM --- ST:%d, T:%d, B1:%d, B2:%d, Light:%d \n", switch_st, T, button1, button2, light);
		printf ("\n---------------------------------------------------------------------------\n");
	od

}


//SPECIFICATIONS
//Switch model
ltl button_is_followed_by_light {
	[]((button1 || button2) -> <>light);
}
ltl light_on_until_timer {
	[](light -> <>(T -> <> !light);
}
