#include "code.h"

//FSM DEFINITION
fsm_trans_t code[] = {
    { IDLE,  check_pressed_key, COUNT, increase_count },
    { COUNT, check_pressed_key, COUNT, increase_count },
    { COUNT, check_count_limit, BUFFER_DIGIT, process_digit },
    { COUNT, check_code_timer_ended, BUFFER_DIGIT, process_digit },
    { BUFFER_DIGIT, check_pressed_key, COUNT, increase_count },
    { BUFFER_DIGIT, check_code_ended, IDLE, examine_code },
    {-1, NULL, -1, NULL },
};


const int correct_code[CODE_LENGTH] = {1, 2, 3};

static int ticks = 0;
static int key = 0;
static int timer_ended = 0;
static int count = 0;
static int index = 0;
static int current_code[CODE_LENGTH] = {0,0,0};


//INTERRUPTIONS ROUTINES
void key_isr (void) { 
  key = 1;
}

void timer_code_isr (void) { 
    timer_ended = 1;
}


//STATE CHECKING FUNCTIONS
int check_pressed_key (fsm_t* this) { 
  return key & (index < CODE_LENGTH); 
}

int check_count_limit (fsm_t* this) { 
  return COUNT_LIMIT <= count; 
}

int check_code_timer_ended (fsm_t* this) { 
  return timer_ended; 
}

int check_code_ended (fsm_t* this) { 
  return index >= CODE_LENGTH; 
}

int check_code_not_ended (fsm_t* this) { 
  return index < CODE_LENGTH; 
}


//OUTPUT FUNCTIONS
void increase_count (fsm_t* this) {

    key = 0;
    timer_ended = 0;

    count++;
    start_code_timer();

    printf("COUNT INCREASED TO '%d' \n",count);
}

void process_digit (fsm_t* this) { 
    key = 0;
    timer_ended = 0;

    if (count < COUNT_LIMIT) {
        current_code[index] = count;
    } else {
        current_code[index] = 0;
    }

    
    if (index == 0) {
        printf("DIGIT '%d' ENTERED: CURRENT CODE = [%d,X,X]\n",index+1,current_code[0]);
    } else if (index == 1) {
        printf("DIGIT '%d' ENTERED: CURRENT CODE = [%d,%d,X]\n",index+1,current_code[0],current_code[1]);
    } else if (index == 2) {
        printf("DIGIT '%d' ENTERED: CURRENT CODE = [%d,%d,%d]\n",index+1,current_code[0],current_code[1],current_code[2]);
    }
    

    count = 0;
    index++;

    if (index < CODE_LENGTH){
        printf("ENTER DIGIT '%d' \n",index+1);
    }
}

void examine_code (fsm_t* this) { 
    key = 0;
    timer_ended = 0;

    int correct = 1;
    for(int i=0;i<CODE_LENGTH;i++){
        correct &= (current_code[i] == correct_code[i]);
        current_code[i] = 0;
        //printf("'%d' vs '%d' : %d\n",current_code[i],correct_code[i],correct);
    }

    if(correct){
        printf("ENTERED CODE IS CORRECT. \n");
        alarm_code_isr();
    }else{
        printf("ENTERED CODE IS INCORRECT. TRY AGAIN. \n");
    }

    index = 0;
    current_code[CODE_LENGTH] = {0,0,0};
}


//TIMER UPDATE
void start_code_timer(){
  ticks = 0;
}

void update_code_timer(){
  if ((ticks > -1) & (ticks < CODE_TMR_TICKS)){
    //Increase ticks
    ticks++;
  } else if (ticks >= CODE_TMR_TICKS) {
    //Set ticks to -1
    ticks = -1;
    //Trigger function
    timer_code_isr();
  }
  //printf("%d",ticks);
}