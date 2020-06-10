#include "code.h"

#define DEBOUNCE_TIME 50
#define TIMER_PERIOD_SEG 1 //1seg
#define COUNT_LIMIT 10
#define CODE_LENGTH 3

static const int correct_code[CODE_LENGTH] = {1, 2, 3};

static int key = 0;
static int timer_ended = 0;
static int count = 0;
static int index = 0;
static int current_code[CODE_LENGTH] = {0,0,0};

static struct timespec now;
static struct timespec end;
static struct timespec T = {TIMER_PERIOD_SEG, 0};

static int debounceTime = DEBOUNCE_TIME;

//INTERRUPTIONS ROUTINES
void key_isr (void) { 

  //Debouncing procedure
  if (millis() < debounceTime){
    debounceTime = millis() + DEBOUNCE_TIME;
    return;
  }

  key = 1;

  debounceTime = millis() + DEBOUNCE_TIME;
}

void timer_code_isr (void) { 
    timer_ended = 1;
}


//TIMER START
static void start_code_timer(){
  //Start time
  clock_gettime(CLOCK_REALTIME, &end);
  //End time
  timespec_add(&end,&end,&T);
}


//STATE CHECKING FUNCTIONS
static int check_pressed_key (fsm_t* this) { 
  return key & (index < CODE_LENGTH); 
}

static int check_count_limit (fsm_t* this) { 
  return COUNT_LIMIT <= count; 
}

static int check_code_timer_ended (fsm_t* this) { 
  clock_gettime(CLOCK_REALTIME, &now);
  return timespec_less (&end, &now); //True if end < now
}

static int check_code_ended (fsm_t* this) { 
  return index >= CODE_LENGTH; 
}


//OUTPUT FUNCTIONS
static void increase_count (fsm_t* this) {
    key = 0;
    timer_ended = 0;

    count++;
    start_code_timer();

    printf("\rCOUNT INCREASED TO '%d' \n",count);
}

static void process_digit (fsm_t* this) { 
    key = 0;
    timer_ended = 0;

    if (count < COUNT_LIMIT) {
        current_code[index] = count;
    } else {
        current_code[index] = 0;
    }

    
    if (index == 0) {
        printf("\rDIGIT '%d' ENTERED: CURRENT CODE = [%d,X,X]\n",index+1,current_code[0]);
    } else if (index == 1) {
        printf("\rDIGIT '%d' ENTERED: CURRENT CODE = [%d,%d,X]\n",index+1,current_code[0],current_code[1]);
    } else if (index == 2) {
        printf("\rDIGIT '%d' ENTERED: CURRENT CODE = [%d,%d,%d]\n",index+1,current_code[0],current_code[1],current_code[2]);
    }
    

    count = 0;
    index++;

    if (index < CODE_LENGTH){
        printf("\rENTER DIGIT '%d' \n",index+1);
    }
}

static void examine_code (fsm_t* this) { 
    key = 0;
    timer_ended = 0;

    int correct = 1;
    for(int i=0;i<CODE_LENGTH;i++){
        correct &= (current_code[i] == correct_code[i]);
        current_code[i] = 0;
        //printf("'%d' vs '%d' : %d\n",current_code[i],correct_code[i],correct);
    }

    if(correct){
        printf("\rENTERED CODE IS CORRECT. \n");
        alarm_code_isr();
    }else{
        printf("\rENTERED CODE IS INCORRECT. TRY AGAIN. \n");
    }

    index = 0;
}


//FSM CREATION
fsm_t* fsm_new_code (void) {
	static fsm_trans_t tt[] = {
    { IDLE,  check_pressed_key, COUNT, increase_count },
    { COUNT, check_pressed_key, COUNT, increase_count },
    { COUNT, check_count_limit, BUFFER_DIGIT, process_digit },
    { COUNT, check_code_timer_ended, BUFFER_DIGIT, process_digit },
    { BUFFER_DIGIT, check_pressed_key, COUNT, increase_count },
    { BUFFER_DIGIT, check_code_ended, IDLE, examine_code },
    {-1, NULL, -1, NULL },
  };
	return fsm_new(tt);
}